/**********************************************************
 *  \!file tcp_client.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "tcp_client.h"

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

using namespace std;
using namespace Universal;

FrTcpClient::FrTcpClient(uint32 threadNum, uint32 _writeBufferSize, uint32 _readBufferSize)
	:m_pTcpCache(new TcpCacheTree())
{ 
	while(threadNum--){
		FrTcpServerThread* pThread = new FrTcpServerThread(&m_TcpMsgProcess);
		m_ServerThreads.push_back(pThread);
	}

	m_pTcpCache->socket = SOCKET_UNKNOW_VALUE;
	m_pTcpCache->connect = false;
	m_pTcpCache->bufferTmp.reserve(m_MaxBufferSize);
	m_pTcpCache->bufferTmp.setMaxLimit(m_MaxBufferSize);
	m_pTcpCache->bufferWrite.setMaxLimit(m_MaxBufferSize);
	m_pTcpCache->bufferRead.setMaxLimit(m_MaxBufferSize);
}

FrTcpClient::~FrTcpClient(){
	for(auto iterThread = m_ServerThreads.begin(); iterThread != m_ServerThreads.end(); ++iterThread){
		if(*iterThread != NULL){
			delete *iterThread; *iterThread = NULL;
		};
	}
}

bool FrTcpClient::connect(const std::string &ip, unint32 port){
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
	address.sin_port = htons(port);

	m_RunSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(connect(m_RunSocket, (struct sockaddr *)&address, sizeof(address)) == 0){
		m_EpollSocket = epoll_create(maxListenNum); // linux 2.6.8 后忽略maxListenNum参数
		epoll_event event;
		event.data.fd = m_RunSocket;
		event.events = EPOLLIN | EPOLLET;
		if(epoll_ctl(m_EpollSocket, EPOLL_CTL_ADD, m_RunSocket, &event) == -1){
			DEBUG_E("监听端口增加epoll触发失败。");
			return false;
		}
	}
	else{
		DEBUG_E("链接失败。");
	}

	return true;
}

bool FrTcpClient::stop(){
	epoll_ctl(m_EpollSocket, EPOLL_CTL_DEL, m_RunSocket);
	close(m_EpollSocket);
	return close(m_RunSocket) == 0;
}

void FrTcpClient::execute(){
	while(m_Running){
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

			uint32 eventNum = epoll_wait(m_EpollSocket, events, maxEvent, -1);
			for(uint32 index = 0; index < eventNum; ++idnex){
				if(m_RunSocket = events[index].data.fd){
					// error and disconnect
					if((events[index].events & EPOLLHUP) || (events[index].events & EPOLLERR)){
						close(m_RunSocket);
						dealEvent(m_RunSocket, eSocketEventType_Disconnect);
					}
					// read : 普通数据和带外数据事件一致（暂时）
					if((events[index].events & EPOLLIN) || (events[index].events & EPOLLPRI)){ 
						dealEvent(m_RunSocket, eSocketEventType_Recv);
					}
					// write
					if(events[index].events & EPOLLOUT){
						dealEvent(m_RunSocket, eSocketEventType_Send);
					}
				}
				else{
					DEBUG_E("未知socket.");
				}
			}
		}
	}
}


void FrTcpClient::dealEvent(Socket socket, eSocketEventType eEventType, Universal::BinaryMemoryPtr pPacket = Universal::BinaryMemoryPtr()){
	uint32 sleepTime(0);
	FrTcpServerThread* pServerThreadTmp(NULL);
	while(pServerThreadTmp == NULL){
		pServerThreadTmp = getReadyThread();
		frSleep(++sleepTime);
	}
		
	if(!pServerThreadTmp->active(m_pTcpCache, socketEventType, pPacket)){
		DEBUG_E("处理事件失败：激活处理线程失败。");
	}
}

