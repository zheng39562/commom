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

FrTcpServer::FrTcpServer(uint32 threadNum)
	:m_SocketMutex(),
	 m_TcpCache(),
	 m_ServerThreads(),
	 m_EpollSocket(SOCKET_UN_INIT_VALUE),
	 m_ListenSocket(SOCKET_UN_INIT_VALUE)
{ 
	while(threadNum--){
		m_ServerThreads.push_back(new FrTcpServerThread(&m_TcpCache, &m_SocketMutex));
	}
}

FrTcpServer::~FrTcpServer(){ 
	for(auto iterThread = m_ServerThreads.begin(); iterThread != m_ServerThreads.end(); ++iterThread){
		if(*iterThread != NULL){
			delete *iterThread; *iterThread = NULL;
		};
	}
}

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

		m_ListenSocket = socket(PF_INET, SOCK_STREAM, 0);
		if(m_ListenSocket < 0){ return false; }

		int on(1);
		setsockopt(m_ListenSocket,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

		int flags = fcntl(m_ListenSocket,F_GETFL,0);
		fcntl(m_ListenSocket, F_SETFL, flags|O_NONBLOCK);

		int ret = ::bind(m_ListenSocket, (struct sockaddr*)&address, sizeof(address)); 
		if(ret == -1){ 
			DEBUG_E("errno " << errno); 
			return false;
		}
		// listen 30 :TCP模块允许的已完成三次握手过程(TCP模块完成)但还没来得及被应用程序accept的最大链接数.
		ret = ::listen(m_ListenSocket, 30);
		if(ret == -1){ 
			DEBUG_E("errno " << errno); 
			return false;
		}

		epoll_event event;
		event.data.fd = m_ListenSocket;
		event.events = EPOLLIN | EPOLLET;
		if(epoll_ctl(m_EpollSocket, EPOLL_CTL_ADD, m_ListenSocket, &event) == -1){
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

	{
		mutex::scoped_lock lock(m_SocketMutex);
		for(auto iterCache = m_TcpCache.begin(); iterCache != m_TcpCache.end(); ++iterCache){
			close(iterCache->first);

		}
		// 停止所有线程后，可以保证缓存区没有任何人操作。所以可以进行清空行为。
		m_TcpCache.clear();
	}

	close(m_EpollSocket);
	close(m_ListenSocket);
	m_EpollSocket = SOCKET_UN_INIT_VALUE;
	m_ListenSocket = SOCKET_UN_INIT_VALUE;

	return true;
}

bool FrTcpServer::disconnect(Socket socket){
	return true;
}

bool FrTcpServer::send(Socket socket, const BinaryMemory &binary){
	mutex::scoped_lock lock(m_SocketMutex);
	auto iterCache = m_TcpCache.find(socket);
	if(iterCache != m_TcpCache.end()){
		proto_size size = binary.getBufferSize();
		mutex::scoped_lock(iterCache->second.mutexWrite);
		iterCache->second.bufferWrite.addBuffer((Byte*)&size, sizeof(proto_size));
		iterCache->second.bufferWrite.addBuffer(binary);
		return true;
	}
	else{
		DEBUG_E("该binary连接不存在或已断开，无法发送数据");
	}
	return false;
}

bool FrTcpServer::sendToGroup(const vector<Socket> &sockets, const BinaryMemory &binary){
	if(binary.getBufferSize() == 0){ return true; }

	proto_size size = binary.getBufferSize();

	bool bRet(true);
	mutex::scoped_lock lock(m_SocketMutex);
	for(auto citerSocket = sockets.end(); citerSocket != sockets.end(); ++citerSocket){
		auto iterCache = m_TcpCache.find(*citerSocket);
		if(iterCache != m_TcpCache.end()){
			mutex::scoped_lock(iterCache->second.mutexWrite);
			iterCache->second.bufferWrite.addBuffer((Byte*)&size, sizeof(proto_size));
			iterCache->second.bufferWrite.addBuffer(binary);
		}
		else{
			DEBUG_E("该binary连接不存在或已断开，无法发送数据");
			bRet = false;
		}
	}
	return bRet;
}

bool FrTcpServer::sendAll(const Universal::BinaryMemory &binary){
	if(binary.getBufferSize() == 0){ return true; }

	proto_size size = binary.getBufferSize();

	bool bRet(true);
	mutex::scoped_lock lock(m_SocketMutex);
	for(auto iterCache = m_TcpCache.begin(); iterCache != m_TcpCache.end(); ++iterCache){
		mutex::scoped_lock(iterCache->second.mutexWrite);
		iterCache->second.bufferWrite.addBuffer((Byte*)&size, sizeof(proto_size));
		iterCache->second.bufferWrite.addBuffer(binary);
	}
	return true;
}

void FrTcpServer::setCallBack(fp_connect_cb connect_cb, fp_disconnect_cb disconn_cb, fp_send_cb send_cb, fp_receive_cb receive_cb, void* etc){
	for(auto iterThread = m_ServerThreads.begin(); iterThread != m_ServerThreads.end(); ++iterThread){
		(*iterThread)->setCallBack(connect_cb, disconn_cb, send_cb, receive_cb, etc);
	}
}

void FrTcpServer::execute(){
	uint32 maxEvent;
	epoll_event* events = (epoll_event*)calloc(maxEvent, sizeof(epoll_event));
	eSocketEventType socketEventType(eSocketEventType_Invalid);
	while(m_Running){
		uint32 eventNum = epoll_wait(m_EpollSocket, events, maxEvent, -1);
		while(eventNum--){
			socketEventType = eSocketEventType_Invalid;
			// error and disconnect
			if((events[eventNum].events & EPOLLHUP) || (events[eventNum].events & EPOLLERR)){
				dealDisconnectReq(events[eventNum].data.fd);
				socketEventType = eSocketEventType_Disconnect;
			}
			// read
			else if(events[eventNum].events & EPOLLIN){ 
				if(m_ListenSocket == events[eventNum].data.fd){
					dealConnectReq();
					socketEventType = eSocketEventType_Connect;
				}
				else{
					socketEventType = eSocketEventType_Recv;
				}
			}
			// write
			else if(events[eventNum].events & EPOLLOUT){
				socketEventType = eSocketEventType_Send;
			}
			// read : out of data
			else if(events[eventNum].events & EPOLLPRI){
				// 对带外数据暂不涉及。
				socketEventType = eSocketEventType_Recv;
			}

			uint32 sleepTime(0);
			FrTcpServerThread* pServerThreadTmp(NULL);
			while(pServerThreadTmp == NULL){
				pServerThreadTmp = getReadyThread();
				frSleep(++sleepTime);
			}
			
			if(!pServerThreadTmp->active(events[eventNum].data.fd, socketEventType)){
				DEBUG_E("处理时间失败：激活处理线程失败。");
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

void FrTcpServer::dealConnectReq(){
	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	bzero(&address, sizeof(address));
	Socket socket = accept(m_ListenSocket, (sockaddr*)&address, &len);
	if(socket > 0){
		epoll_event event;
		event.data.fd = socket;
		event.events = EPOLLIN | EPOLLET | EPOLLOUT;
		epoll_ctl(m_EpollSocket, EPOLL_CTL_ADD, socket, &event);
	}

	{
		mutex::scoped_lock lock(m_SocketMutex);
		m_TcpCache.insert(make_pair(socket, LockCache()));
	}
}

void FrTcpServer::dealDisconnectReq(Socket socket){
	close(socket);
	{
		mutex::scoped_lock lock(m_SocketMutex);
		auto iterCache = m_TcpCache.find(socket);
		if(iterCache != m_TcpCache.end()){
			m_TcpCache.erase(iterCache);
		}
	}
}




