/**********************************************************
 *  \!file net_server.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "net_server.h"

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

namespace Network{
	using namespace std;

	NetServer::NetServer()
		:m_IsRunning(false)
	{
		;
	}
	NetServer::~NetServer(){ 
		m_IsRunning = false;
	}

	void NetServer::execute(){
		struct sockaddr_in client;
		socklen_t client_addrlength = sizeof(client);
		int connfd(-1);
		char remote[ INET_ADDRSTRLEN ];
		while(m_IsRunning){
			// 印象里accept是阻塞的。需要确认下。
			connfd = accept(m_ListenSocket, (struct sockaddr *) &client, &client_addrlength);

			printf("connected with ip : %s and port : %d\n", 
					inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN), 
					ntohs(client.sin_port));

			if(connfd >= 0){
				addSocket(connfd);
			}
			else{
				break;
			}
		}

		close(m_ListenSocket);
		m_ListenSocket = -1;
	}

	bool NetServer::run(const string &ip, const long &port){
		m_IsRunning = true;

		struct sockaddr_in address;
		bzero(&address, sizeof(address));
		address.sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
		address.sin_port = htons(port);

		m_ListenSocket = socket(PF_INET, SOCK_STREAM, 0);
		assert(m_ListenSocket >= 0);

		int on(0);
		setsockopt(m_ListenSocket,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

		int ret = bind(m_ListenSocket, (struct sockaddr*) &address,  sizeof(address));
		if(ret == -1){ cout << "errno " << errno << endl; }
		assert(ret != -1); 
		ret = listen(m_ListenSocket, 5);
		if(ret == -1){ cout << "errno " << errno << endl; }
		assert(ret != -1); 

		return start();
	}
}
