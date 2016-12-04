/**********************************************************
 *  !file tcp_msg_process.cpp
 *  !brief
 * 
 * !version 
 * * !author zheng39562@163.com
**********************************************************/
#include "tcp_msg_process.h"

#include <sys/epoll.h>
#include <sys/socket.h>
#include "fr_public/pub_tool.h"

using namespace std;
using namespace Universal;
using namespace boost;

FrTcpMsgProcess::FrTcpMsgProcess(Socket _epollSocket)
	:m_EpollSocket(_epollSocket),
	 m_pETC(NULL),
	 m_pConnectCB(NULL),
	 m_pDisconnectCB(NULL),
	 m_pSendCB(NULL),
	 m_pReceiveCB(NULL)
{ ; }

FrTcpMsgProcess::FrTcpMsgProcess(const FrTcpMsgProcess &ref){ ; }

FrTcpMsgProcess::~FrTcpMsgProcess(){ ; }

bool FrTcpMsgProcess::push(FrTcpCachePtr pTcpCache, Universal::BinaryMemoryPtr pPacket){
	FrTcpCache& refTcpCache = *pTcpCache;
	mutex::scoped_lock local(refTcpCache.mutexWrite);

	uint32 waitTimes(3);
	while(refTcpCache.bufferWrite.maxLimit() < pPacket->curSize() + refTcpCache.bufferWrite.curSize()){
		if(waitTimes--){
			DEBUG_E("缓存没有足够的内存容纳该消息包。请增加缓存，或检查发送是否存在异常（如链接已断开）");
			return false;
		}
		frSleep(10);
	}

	proto_size size(pPacket->curSize());
	refTcpCache.bufferWrite.add((Byte*)&size, sizeof(size));
	refTcpCache.bufferWrite.add(*pPacket);

	if(refTcpCache.writeActive){
		sint32 sendSize = ::send(refTcpCache.socket, refTcpCache.bufferWrite.buffer(), refTcpCache.bufferWrite.curSize(), MSG_DONTWAIT);
		if(sendSize > 0){ refTcpCache.bufferWrite.del(0, sendSize); }
		updateEpollStatus(refTcpCache.socket);
		refTcpCache.writeActive = false;
	}

	return true;
}

void FrTcpMsgProcess::send(FrTcpCachePtr pTcpCache){
	FrTcpCache& refTcpCache = *pTcpCache;
	mutex::scoped_lock scopedLock(refTcpCache.mutexWrite);
	if(refTcpCache.bufferWrite.curSize() != 0){
		sint32 sendSize = ::send(refTcpCache.socket, refTcpCache.bufferWrite.buffer(), refTcpCache.bufferWrite.curSize(), MSG_DONTWAIT);
		if(sendSize > 0){ refTcpCache.bufferWrite.del(0, sendSize); }
		updateEpollStatus(refTcpCache.socket);
	}
	else{
		refTcpCache.writeActive = true;
	}
	
	execSendCB(pTcpCache->socket);
}

void FrTcpMsgProcess::recv(FrTcpCachePtr pTcpCache){
	FrTcpCache& refTcpCache = *pTcpCache;
	mutex::scoped_lock scopedLock(refTcpCache.mutexRead);
	int recvSize(0);
	size_t maxRecvSize = refTcpCache.bufferRead.maxLimit() - refTcpCache.bufferRead.curSize();
	refTcpCache.bufferTmp.reserve(maxRecvSize);
	while((recvSize = ::recv(pTcpCache->socket, refTcpCache.bufferTmp.buffer(), maxRecvSize, MSG_DONTWAIT)) > 0){
		refTcpCache.bufferRead.add(refTcpCache.bufferTmp.buffer(), recvSize);
		DEBUG_D("接受到字节流 [" << recvSize << "] 接受后缓存大小 [" << refTcpCache.bufferRead.curSize() << "]");
		recvPackets(pTcpCache->socket, refTcpCache.bufferRead);

		maxRecvSize = refTcpCache.bufferRead.maxLimit() - refTcpCache.bufferRead.curSize();
		refTcpCache.bufferTmp.clear();
	}
}

void FrTcpMsgProcess::connect(FrTcpCachePtr pTcpCache){
	execConnectCB(pTcpCache->socket);
}

void FrTcpMsgProcess::disconnect(FrTcpCachePtr pTcpCache){
	execDisconnectCB(pTcpCache->socket);
}

void FrTcpMsgProcess::execConnectCB(Socket socket){
	if(m_pConnectCB != NULL){
		(*m_pConnectCB)(socket, m_pETC);
	}
}

void FrTcpMsgProcess::execDisconnectCB(Socket socket){
	if(m_pDisconnectCB != NULL){
		(*m_pDisconnectCB)(socket, m_pETC);
	}
}

void FrTcpMsgProcess::execSendCB(Socket socket){
	if(m_pSendCB != NULL){
		(*m_pSendCB)(socket, m_pETC);
	}
}

void FrTcpMsgProcess::execReceiveCB(Socket socket, const Universal::BinaryMemoryPtr &pBinary){
	if(m_pReceiveCB != NULL){
		(*m_pReceiveCB)(socket, pBinary, m_pETC);
	}
}

void FrTcpMsgProcess::setCallBack(fp_connect_cb connect_cb, fp_disconnect_cb disconn_cb, fp_send_cb send_cb, fp_receive_cb receive_cb, void* etc){
	m_pConnectCB = connect_cb;
	m_pDisconnectCB = disconn_cb;
	m_pSendCB = send_cb;
	m_pReceiveCB = receive_cb;
	m_pETC = etc;
}

void FrTcpMsgProcess::updateEpollStatus(Socket socket){
	epoll_event event;
	event.data.fd = socket;
	event.events = EPOLLOUT | EPOLLET;
	if(epoll_ctl(m_EpollSocket, EPOLL_CTL_MOD, socket, &event) == -1){
		DEBUG_E("更新epoll状态失败。");
	}
}

void FrTcpMsgProcess::recvPackets(Socket socket, Universal::BinaryMemory &binary){
	proto_size size(0);
	DEBUG_D("包内容大小 [" << *(proto_size*)binary.buffer() << "] cur buffer size [" << binary.curSize() << "]" );
	while((size = *(proto_size*)binary.buffer()) <= (proto_size)binary.curSize() && size > 0){
		// 直接裁剪掉包头
		BinaryMemoryPtr pBinary(new BinaryMemory((Byte*)binary.buffer() + sizeof(proto_size), size));
		DEBUG_D("包内容大小 [" << *(proto_size*)binary.buffer() << "] cur buffer size [" << binary.curSize() << "]" );
		binary.del(0, size + sizeof(proto_size));
		DEBUG_D("得到的包内容[" << string((char*)pBinary->buffer(), pBinary->curSize())<< "] 删除后的cur buffer size [" << binary.curSize() << "]" );
		execReceiveCB(socket, pBinary);
	}
}

