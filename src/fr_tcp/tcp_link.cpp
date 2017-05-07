/**********************************************************
 *  \file tcp_link.cpp
 *  \brief
 *  \note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#include "tcp_link.h"

#include "fr_public/pub_timer.h"
#include "fr_public/pub_memory.h"

#ifndef _WIN32
#include <sys/epoll.h>
#endif

using namespace std;
using namespace boost;
using namespace Universal;

void server_send_cb(Socket socket, void* etc){
	if(etc != NULL){
		FrTcpLinker* pServer = (FrTcpLinker*)etc;
		eEventResult result = pServer->onSend(socket);
	}
}
void server_recv_cb(Socket socket, const Universal::BinaryMemoryPtr &pBinary, void* etc){
	if(etc != NULL){
		FrTcpLinker* pServer = (FrTcpLinker*)etc;
		eEventResult result = pServer->onReceive(socket, pBinary);
	}
}
void server_connect_cb(Socket socket, void* etc){
	if(etc != NULL){
		FrTcpLinker* pServer = (FrTcpLinker*)etc;
		eEventResult result = pServer->onConnect(socket);
	}
}
void server_disconnect_cb(Socket socket, void* etc){
	if(etc != NULL){
		FrTcpLinker* pServer = (FrTcpLinker*)etc;
		eEventResult result = pServer->onDisconnect(socket);
	}
}

void server_push_msg(const PushMsg &msg, void* etc){
	if(etc != NULL){
		FrTcpLinker* pServer = (FrTcpLinker*)etc;
		pServer->push(msg);
	}
}

FrTcpLinker::FrTcpLinker(uint32 threadNum, uint32 _maxBufferSize)
	:m_pTcpMsgProcess(NULL),
	 m_MsgQueue(),
	 m_MaxBufferSize(_maxBufferSize),
	 m_EpollSocket(UNKNOW_SOCKET),
	 m_ServerThreads()
{
	m_EpollSocket = epoll_create(65535);
	if(m_EpollSocket > 0){
		m_pTcpMsgProcess = new FrTcpMsgProcess(m_EpollSocket);
		m_pTcpMsgProcess->setCallBack(server_connect_cb, server_disconnect_cb, server_send_cb, server_recv_cb, server_push_msg, this);

		while(threadNum--){
			FrTcpServerThread* pThread = new FrTcpServerThread(m_pTcpMsgProcess);
			m_ServerThreads.push_back(pThread);
		}

		FrThread::start();
	}
	else{
		DEBUG_C("创建套接字失败。");
		throw;
	}
}

FrTcpLinker::~FrTcpLinker(){
	for(auto iterThread = m_ServerThreads.begin(); iterThread != m_ServerThreads.end(); ++iterThread){
		if(*iterThread != NULL){
			delete *iterThread; *iterThread = NULL;
		};
	}

	if(m_pTcpMsgProcess != NULL){
		delete m_pTcpMsgProcess; m_pTcpMsgProcess = NULL;
	}
}

bool FrTcpLinker::stop(){
	// 关闭所有处理线程。保证情况缓存不会被操作。
	FrThread::stop();
	for(auto iterThread = m_ServerThreads.begin(); iterThread != m_ServerThreads.end(); ++iterThread){
		(*iterThread)->stop();
	}

	for(auto iterCache = m_TcpCacheTree.begin(); iterCache != m_TcpCacheTree.end(); ++iterCache){
		::close(iterCache->first);
	}
	// 停止所有线程后，可以保证缓存区没有任何人操作。所以可以进行清空行为。
	m_TcpCacheTree.clear();

	::close(m_EpollSocket);
	m_EpollSocket = UNKNOW_SOCKET;
}

eEventResult FrTcpLinker::onSend(Socket socket){ return eEventResult_OK; }
eEventResult FrTcpLinker::onReceive(Socket socket, BinaryMemoryPtr pBinary){ return eEventResult_OK; }
eEventResult FrTcpLinker::onConnect(Socket socket){ return eEventResult_OK; }
eEventResult FrTcpLinker::onDisconnect(Socket socket){ return eEventResult_OK; }

bool FrTcpLinker::send(Socket socket, const BinaryMemoryPtr &pBinary){
	push(PushMsg(socket, eSocketEventType_Push, pBinary));
	return true;
}

bool FrTcpLinker::push(const PushMsg &msg){
	m_MsgQueue.push(msg);
	return true;
}

void FrTcpLinker::execute(){
	uint32 maxEvent(30);
	epoll_event* events = (epoll_event*)calloc(maxEvent, sizeof(epoll_event));
	eSocketEventType socketEventType(eSocketEventType_Invalid);

	queue<PushMsg> msgQueue;
	while(isRunningThread()){
		// 待发送的队列处理。
		if(!msgQueue.empty() || m_MsgQueue.swap(msgQueue)){
			while(!msgQueue.empty()){
				PushMsg pushMsg = msgQueue.front();
				switch(pushMsg.eventType){
					case eSocketEventType_Connect:
						m_TcpCacheTree.insert(make_pair(pushMsg.socket, FrTcpCachePtr(new FrTcpCache(pushMsg.socket, m_MaxBufferSize))));
						addSocketToEpoll(pushMsg.socket);
						dealEvent(pushMsg.socket, eSocketEventType_Connect);
						break;
					case eSocketEventType_Disconnect:
						epoll_ctl(m_EpollSocket, EPOLL_CTL_DEL, pushMsg.socket, NULL);
						m_TcpCacheTree.erase(pushMsg.socket);
						close(pushMsg.socket);
						dealEvent(pushMsg.socket, eSocketEventType_Disconnect);
						break;
					default:
						dealEvent(pushMsg.socket, pushMsg.eventType, pushMsg.pBinary);
						break;
				}
				msgQueue.pop();
			}
		}

		int32 eventNum = epoll_wait(m_EpollSocket, events, maxEvent, 50); // 50ms timeout
		for(int32 index = 0; index < eventNum; ++index){
			Socket socket = events[index].data.fd;
			socketEventType = eSocketEventType_Invalid;
			// error and disconnect
			if((events[index].events & EPOLLHUP) || (events[index].events & EPOLLERR)){
				// 注意增加 socket的 close函数 和 epoll的 delete操作。
				dealDisconnect(socket);
			}
			// read : 普通数据和带外数据事件一致（暂时）
			if((events[index].events & EPOLLIN) || (events[index].events & EPOLLPRI)){ 
				activeRecv(socket);
			}
			// write
			if(events[index].events & EPOLLOUT){
				activeSend(socket);
			}
		}

		frSleep(5);
	}
}

void FrTcpLinker::addSocketToEpoll(Socket socket){
	epoll_event event;
	event.data.fd = socket;
	event.events = EPOLLIN | EPOLLET | EPOLLOUT;
	epoll_ctl(m_EpollSocket, EPOLL_CTL_ADD, socket, &event);
}

FrTcpServerThread* FrTcpLinker::getReadyThread(){
	for(auto iterThread = m_ServerThreads.begin(); iterThread != m_ServerThreads.end(); ++iterThread){
		if((*iterThread)->ready()){
			return *iterThread;
		}
	}
	return NULL;
}


void FrTcpLinker::dealEvent(Socket socket, eSocketEventType eEventType){
	dealEvent(socket, eEventType, BinaryMemoryPtr());
}
void FrTcpLinker::dealEvent(Socket socket, eSocketEventType eEventType, BinaryMemoryPtr pPacket){
	auto iterCache = m_TcpCacheTree.find(socket);
	if(iterCache != m_TcpCacheTree.end()){
		uint32 sleepTime(0);
		FrTcpServerThread* pServerThreadTmp(NULL);
		while(pServerThreadTmp == NULL){
			pServerThreadTmp = getReadyThread();
			frSleep(++sleepTime);
		}
		
		if(!pServerThreadTmp->active(iterCache->second, eEventType, pPacket)){
			DEBUG_E("处理事件失败：激活处理线程失败。");
		}
	}
	else{
		DEBUG_I("该链接不存在 或 已断开。");
	}
}


