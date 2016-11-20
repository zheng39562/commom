/**********************************************************
 *  !file tcp_msg_process.cpp
 *  !brief
 * 
 * !version 
 * * !author zheng39562@163.com
**********************************************************/
#include "tcp_msg_process.h"

#include <sys/epoll.h>

using namespace std;
using namespace Universal;
using namespace boost;

FrTcpMsgProcess::FrTcpMsgProcess(Socket _epollSocket)
	:m_EpollSocket(_epollSocket),
	 m_pETC(NULL),
	 m_pConnectCB(NULL),
	 m_pDisconnectCB(NULL),
	 m_pSendCB(NULL),
	 m_pReceiveCB(NULL),
{
}

FrTcpMsgProcess::FrTcpMsgProcess(const LockCache &ref){ ; }

FrTcpMsgProcess::~FrTcpMsgProcess(){ ; }

void FrTcpMsgProcess::push(FrTcpCachePtr pTcpCache, Universal::BinaryMemoryPtr pPacket){
	FrTcpCache& refTcpCache = *pTcpCache;
	mutex::scoped_lock local(refTcpCache.mutexWrite);

	uint32 waitTimes(3);
	while(refTcpCache.bufferWrite.maxLimit() < pPacket->curSize() + refTcpCache.bufferWrite.curSize()){
		if(waitTimes--){
			DEBUG_E("缓存没有足够的内存容纳该消息包。请增加缓存，或检查发送是否存在异常（如链接已断开）");
			return;
		}
		frSleep(10);
	}

	refTcpCache.bufferWrite.add((Byte*)&size, sizeof(proto_size));
	refTcpCache.bufferWrite.add(*pPacket);

	if(refTcpCache.activeWrite){
		sint32 sendSize = ::send(refTcpCache.socket, refTcpCache.bufferWrite.buffer(), refTcpCache.bufferWrite.curSize(), MSG_DONTWAIT);
		if(sendSize > 0){ refTcpCache.bufferWrite.del(0, sendSize); }
		updateEpollStatus(refTcpCache.socket);
		refTcpCache.activeWrite = false;
	}
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
		refTcpCache.activeWrite = true;
	}
	
	execSendCB(socket);
}

void FrTcpMsgProcess::recv(FrTcpCachePtr pTcpCache){
	auto iterCache = m_pTcpCache->find(socket);
	if(iterCache != m_pTcpCache->end()){
		mutex::scoped_lock scopedLock(refTcpCache.mutexRead);
		FrTcpCache refTcpCache = *refTcpCache;
		size_t recvSize(0);
		size_t maxRecvSize = refTcpCache.bufferRead.maxLimit() - refTcpCache.bufferRead.curSize();
		refTcpCache.bufferTmp.reserve(maxRecvSize);
		while((recvSize = ::recv(socket, refTcpCache.bufferTmp, maxRecvSize, MSG_DONTWAIT)) > 0){
			refTcpCache.bufferRead.add(refTcpCache.bufferTmp.bufferPtr(), recvSize);
			recvPackets(socket, refTcpCache.bufferRead);

			maxRecvSize = refTcpCache.bufferRead.maxLimit() - refTcpCache.bufferRead.curSize();
			refTcpCache.bufferTmp.clear();
		}

	}
}

void FrTcpMsgProcess::connect(FrTcpCachePtr pTcpCache){
	execConnectCB(pTcpCache->socket);
}

void FrTcpMsgProcess::disconnect(FrTcpCachePtr pTcpCache){
	execDisconnectCB(socket);
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

void FrTcpMsgProcess::recvPackets(FrTcpCachePtr pTcpCache, Universal::BinaryMemory &binary){
	proto_size size(0);
	while((size = *(proto_size*)binary.buffer()) <= (proto_size)binary.curSize()){
		// 直接裁剪掉包头
		BinaryMemoryPtr pBinary(new BinaryMemory((Byte*)binary.buffer() + sizeof(proto_size), size - sizeof(proto_size)));
		binary.del(0, size);
		execReceiveCB(socket, pBinary);
	}
}

