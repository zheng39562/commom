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


FrTcpServer::FrTcpServer(uint32 threadNum, uint32 _maxBufferSize)
	:FrTcpLinker(threadNum, _maxBufferSize)
{ 
}

FrTcpServer::~FrTcpServer(){ ; }

bool FrTcpServer::run(const string &ip, unsigned int port){
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

		addSocketToEpoll(m_ListenSocket);
		return true;
	}

	return false;
}

bool FrTcpServer::disconnect(Socket socket){
	close(socket);
	return true;
}

void FrTcpServer::dealConnect(Socket socket){
	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	bzero(&address, sizeof(address));
	Socket socketRecv = accept(socket, (sockaddr*)&address, &len);
	while(socketRecv > 0){
		FrTcpLinker::dealConnect(socketRecv);
		socketRecv = accept(socket, (sockaddr*)&address, &len);
	}
}

void FrTcpServer::dealRecv(Socket socket){
	if(m_ListenSocket == socket){
		dealConnect(socket);
	}
	else{
		FrTcpLinker::dealRecv(socket);
	}
}
