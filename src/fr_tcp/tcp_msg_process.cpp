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
	 m_fpConnectCB(NULL),
	 m_fpDisconnectCB(NULL),
	 m_fpSendCB(NULL),
	 m_fpReceiveCB(NULL),
	 m_fpPushMsg(NULL)
{ ; }

FrTcpMsgProcess::FrTcpMsgProcess(const FrTcpMsgProcess &ref){ ; }

FrTcpMsgProcess::~FrTcpMsgProcess(){ ; }

bool FrTcpMsgProcess::push(FrTcpCachePtr pTcpCache, Universal::BinaryMemoryPtr pPacket){
	if(pPacket == NULL){
		return true;
	}

	FrTcpCache& refTcpCache = *pTcpCache;
	mutex::scoped_lock local(refTcpCache.mutexWrite);

	if(refTcpCache.bufferWrite.empty()){
		sint32 sendSize(0);
		while(!pPacket->empty()){
			sendSize = ::send(pTcpCache->socket, pPacket->buffer(), pPacket->curSize(), MSG_DONTWAIT);
			pPacket->del(0, sendSize);
			if(sendSize <= 0){
				checkConnect(pTcpCache->socket);
				break;
			}
		}
	}

	if(!pPacket->empty()){
		proto_size size(pPacket->curSize());
		refTcpCache.bufferWrite.add((Byte*)&size, sizeof(size));
		refTcpCache.bufferWrite.add(*pPacket);
	}

	return true;
}

void FrTcpMsgProcess::send(FrTcpCachePtr pTcpCache){
	FrTcpCache& refTcpCache = *pTcpCache;
	mutex::scoped_lock scopedLock(refTcpCache.mutexWrite);
	while(!refTcpCache.bufferWrite.empty()){
		sint32 sendSize = ::send(refTcpCache.socket, refTcpCache.bufferWrite.buffer(), refTcpCache.bufferWrite.curSize(), MSG_DONTWAIT);
		refTcpCache.bufferWrite.del(0, sendSize);
		if(sendSize <= 0){
			checkConnect(pTcpCache->socket);
			break;
		}
	}
	
	updateEpollStatus(refTcpCache.socket);
	execSendCB(pTcpCache->socket);
}

void FrTcpMsgProcess::recv(FrTcpCachePtr pTcpCache){
	FrTcpCache& refTcpCache = *pTcpCache;
	mutex::scoped_lock scopedLock(refTcpCache.mutexRead);
	int recvSize(0);
	size_t maxRecvSize = refTcpCache.bufferRead.maxLimit() - refTcpCache.bufferRead.curSize();
	refTcpCache.bufferTmp.reserve(maxRecvSize);
	if((recvSize = ::recv(pTcpCache->socket, refTcpCache.bufferTmp.buffer(), maxRecvSize, MSG_DONTWAIT)) > 0){
		refTcpCache.bufferRead.add(refTcpCache.bufferTmp.buffer(), recvSize);
		DEBUG_D("接受到字节流 [" << recvSize << "] 接受后缓存大小 [" << refTcpCache.bufferRead.curSize() << "]");
		recvPackets(pTcpCache->socket, refTcpCache.bufferRead);

		maxRecvSize = refTcpCache.bufferRead.maxLimit() - refTcpCache.bufferRead.curSize();
		refTcpCache.bufferTmp.clear();

		updateEpollStatus(refTcpCache.socket);
	}
	else{
		checkConnect(pTcpCache->socket);
	}
}

void FrTcpMsgProcess::connect(FrTcpCachePtr pTcpCache){
	execConnectCB(pTcpCache->socket);
}

void FrTcpMsgProcess::disconnect(FrTcpCachePtr pTcpCache){
	execDisconnectCB(pTcpCache->socket);
}

void FrTcpMsgProcess::execConnectCB(Socket socket){
	if(m_fpConnectCB != NULL){
		(*m_fpConnectCB)(socket, m_pETC);
	}
}

void FrTcpMsgProcess::execDisconnectCB(Socket socket){
	if(m_fpDisconnectCB != NULL){
		(*m_fpDisconnectCB)(socket, m_pETC);
	}
}

void FrTcpMsgProcess::execSendCB(Socket socket){
	if(m_fpSendCB != NULL){
		(*m_fpSendCB)(socket, m_pETC);
	}
}

void FrTcpMsgProcess::execReceiveCB(Socket socket, const Universal::BinaryMemoryPtr &pBinary){
	if(m_fpReceiveCB != NULL){
		(*m_fpReceiveCB)(socket, pBinary, m_pETC);
	}
}
void FrTcpMsgProcess::execPushMsg(Socket socket, eSocketEventType eventType, Universal::BinaryMemoryPtr pBinary){
	if(m_fpPushMsg != NULL){
		(*m_fpPushMsg)(PushMsg(socket, eventType, pBinary), m_pETC);
	}
}

void FrTcpMsgProcess::setCallBack(fp_connect_cb connect_cb, fp_disconnect_cb disconn_cb, fp_send_cb send_cb, fp_receive_cb receive_cb, fp_push_msg push_msg, void* etc){
	m_fpConnectCB = connect_cb;
	m_fpDisconnectCB = disconn_cb;
	m_fpSendCB = send_cb;
	m_fpReceiveCB = receive_cb;
	m_fpPushMsg = push_msg;
	m_pETC = etc;
}

void FrTcpMsgProcess::updateEpollStatus(Socket socket){
	epoll_event event;
	event.data.fd = socket;
	event.events = EPOLLIN | EPOLLOUT | EPOLLET;
	if(epoll_ctl(m_EpollSocket, EPOLL_CTL_MOD, socket, &event) == -1){
		DEBUG_C("更新epoll状态失败。");
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

void FrTcpMsgProcess::checkConnect(Socket socket){
	if(errno != EAGAIN && errno != EINTR && errno != EWOULDBLOCK){
		DEBUG_I("接受or发送消息失败，并且errno反馈异常.推送断开链接消息给主线程");
		execPushMsg(socket, eSocketEventType_Disconnect);
	}
}

