/**********************************************************
 *  !file tcp_server_thread.h
 *  !brief
 * 
 * !version 
 * * !author zheng39562@163.com
**********************************************************/
#include "tcp_server.h"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "fr_public/pub_memory.h"
#include "fr_public/pub_tool.h"

using namespace std;
using namespace boost;
using namespace Universal;

void server_send_cb(Socket socket, void* etc){
	FrTcpServer* pServer = (FrTcpServer*)etc;
	//eEventResult result = pServer->onSend(socket);
	// 后续可以根据result来报错。
}
void server_recv_cb(Socket socket, const Universal::BinaryMemoryPtr &pBinary, void* etc){
	FrTcpServer* pServer = (FrTcpServer*)etc;
	eEventResult result = pServer->onReceive(socket, pBinary);
}
void server_connect_cb(Socket socket, void* etc){
	FrTcpServer* pServer = (FrTcpServer*)etc;
	eEventResult result = pServer->onConnect(socket);
}
void server_disconnect_cb(Socket socket, void* etc){
	FrTcpServer* pServer = (FrTcpServer*)etc;
	eEventResult result = pServer->onDisconnect(socket);
}

FrTcpServer::FrTcpServer(uint32 threadNum, uint32 _writeBufferSize, uint32 _readBufferSize)
	:FrTcpLinker(_writeBufferSize, _readBufferSize),
	 m_ServerThreads(),
	 m_TcpCacheTree()
{ 
	m_TcpMsgProcess.setCallBack(server_connect_cb, server_disconnect_cb, server_send_cb, server_recv_cb, this);
}

FrTcpServer::~FrTcpServer(){ ; }

bool FrTcpServer::listen(const string &ip, unsigned int port, size_t maxListenNum){
	if(m_EpollSocket > 0){
		DEBUG_E("服务已开启，请关闭后再重新监听。");
		return false; 
	}
	m_EpollSocket = epoll_create(maxListenNum); // linux 2.6.8 后忽略maxListenNum参数
	if(m_EpollSocket > 0){
		// init addres
		struct sockaddr_in address;
		bzero(&address, sizeof(address));
		address.sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
		address.sin_port = htons(port);

		m_RunSocket = socket(PF_INET, SOCK_STREAM, 0);
		if(m_RunSocket < 0){ return false; }

		int on(1);
		setsockopt(m_RunSocket,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

		int flags = fcntl(m_RunSocket,F_GETFL,0);
		fcntl(m_RunSocket, F_SETFL, flags|O_NONBLOCK);

		int ret = ::bind(m_RunSocket, (struct sockaddr*)&address, sizeof(address)); 
		if(ret == -1){ 
			DEBUG_E("errno " << errno); 
			return false;
		}
		
		// listen 30 :TCP模块允许的已完成三次握手过程(TCP模块完成)但还没来得及被应用程序accept的最大链接数.
		ret = ::listen(m_RunSocket, 30);
		if(ret == -1){ 
			DEBUG_E("errno " << errno); 
			return false;
		}

		epoll_event event;
		event.data.fd = m_RunSocket;
		event.events = EPOLLIN | EPOLLET;
		if(epoll_ctl(m_EpollSocket, EPOLL_CTL_ADD, m_RunSocket, &event) == -1){
			DEBUG_E("监听端口增加epoll触发失败。");
			return false;
		}

		FrThread::start();
		return true;
	}

	return false;
}

bool FrTcpServer::stop(){
	// 关闭所有处理线程。保证情况缓存不会被操作。
	FrThread::stop();
	for(auto iterThread = m_ServerThreads.begin(); iterThread != m_ServerThreads.end(); ++iterThread){
		(*iterThread)->stop();
	}

	for(auto iterCache = m_TcpCache.begin(); iterCache != m_TcpCache.end(); ++iterCache){
		close(iterCache->first);
	}
	// 停止所有线程后，可以保证缓存区没有任何人操作。所以可以进行清空行为。
	m_TcpCacheTree.clear();

	close(m_EpollSocket);
	close(m_RunSocket);
	m_EpollSocket = SOCKET_UN_INIT_VALUE;
	m_RunSocket = SOCKET_UN_INIT_VALUE;

	return true;
}

bool FrTcpServer::disconnect(Socket socket){
	close(socket);
	return true;
}

void FrTcpServer::execute(){
	uint32 maxEvent;
	epoll_event* events = (epoll_event*)calloc(maxEvent, sizeof(epoll_event));
	eSocketEventType socketEventType(eSocketEventType_Invalid);

	queue<PushMsg> msgQueue;
	while(m_Running){
		// 待发送的队列处理。
		if(!msgQueue.empty() || m_MsgQueue.swap(msgQueue)){
			while(!msgQueue.empty()){
				PushMsg pushMsg = msgQueue.front();
				dealEvent(pushMsg.first, eSocketEventType_Push, pushMsg.second);
				msgQueue.pop();
			}
		}

		uint32 eventNum = epoll_wait(m_EpollSocket, events, maxEvent, 50); // 50ms timeout
		for(uint32 index = 0; index < eventNum; ++idnex){
			Socket socket = events[index].data.fd;
			socketEventType = eSocketEventType_Invalid;
			// error and disconnect
			if((events[index].events & EPOLLHUP) || (events[index].events & EPOLLERR)){
				// 注意增加 socket的 close函数 和 epoll的 delete操作。
				dealDisconnectReq(socket);
				dealEvent(socket, eSocketEventType_Disconnect);
			}
			// read : 普通数据和带外数据事件一致（暂时）
			if((events[index].events & EPOLLIN) || (events[index].events & EPOLLPRI)){ 
				if(m_RunSocket == socket){
					socket = dealConnectReq();
					if(m_TcpMsgProcess.addSocket(socket)){
						dealEvent(socket, eSocketEventType_Connect);
					}
				}
				else{
					dealEvent(socket, eSocketEventType_Recv);
				}
			}
			// write
			if(events[index].events & EPOLLOUT){
				dealEvent(socket, eSocketEventType_Send);
			}
		}
	}
}

FrTcpServerThread* FrTcpServer::getReadyThread(){
	for(auto iterThread = m_ServerThreads.begin(); iterThread != m_ServerThreads.end(); ++iterThread){
		if((*iterThread)->ready()){
			return *iterThread;
		}
	}
	return NULL;
}

Socket FrTcpServer::dealConnectReq(){
	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	bzero(&address, sizeof(address));
	Socket socket = accept(m_RunSocket, (sockaddr*)&address, &len);
	if(socket > 0){
		epoll_event event;
		event.data.fd = socket;
		event.events = EPOLLIN | EPOLLET | EPOLLOUT;
		epoll_ctl(m_EpollSocket, EPOLL_CTL_ADD, socket, &event);

		FrTcpCachePtr pCache(new TcpCacheTree());
		pCache->socket = socket;
		pCache->connect = true;
		pCache->bufferTmp.reserve(m_MaxBufferSize);
		pCache->bufferTmp.setMaxLimit(m_MaxBufferSize);
		pCache->bufferWrite.setMaxLimit(m_MaxBufferSize);
		pCache->bufferRead.setMaxLimit(m_MaxBufferSize);
		m_TcpCacheTree.insert(make_pair(socket, pCache));
	}
	return socket;
}

void FrTcpServer::dealDisconnectReq(Socket socket){
	epoll_ctl(m_EpollSocket, EPOLL_CTL_DEL, socket, NULL);
	m_TcpCacheTree.erase(socket);
}

void FrTcpServer::dealEvent(Socket socket, eSocketEventType eEventType, BinaryMemoryPtr pPacket){
	auto iterCache = m_TcpCacheTree.find(socket);
	if(iterCache != m_TcpCacheTree.end()){
		uint32 sleepTime(0);
		FrTcpServerThread* pServerThreadTmp(NULL);
		while(pServerThreadTmp == NULL){
			pServerThreadTmp = getReadyThread();
			frSleep(++sleepTime);
		}
		
		if(!pServerThreadTmp->active(iterCache->second, socketEventType, pPacket)){
			DEBUG_E("处理事件失败：激活处理线程失败。");
		}
	}
	else{
		DEBUG_I("该链接不存在 或 已断开。");
	}
}


