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

#include "fr_public/pub_timer.h"

using namespace std;
using namespace Universal;

FrTcpClient::FrTcpClient(uint32 threadNum, uint32 _maxBufferSize)
	:FrTcpLinker(threadNum, _maxBufferSize),
	 m_ConnectSocket(UNKNOW_SOCKET)
{ ; }

FrTcpClient::~FrTcpClient(){ ; }

bool FrTcpClient::run(const std::string &ip, uint32 port){
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
	address.sin_port = htons(port);

	m_ConnectSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(::connect(m_ConnectSocket, (struct sockaddr *)&address, sizeof(address)) == 0){
		push(PushMsg(m_ConnectSocket, eSocketEventType_Connect));
	}
	else{
		DEBUG_E("链接失败。");
	}

	return true;
}

bool FrTcpClient::send(const Universal::BinaryMemoryPtr &pBuffer){
	return FrTcpLinker::send(m_ConnectSocket, pBuffer);
}

