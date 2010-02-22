/**********************************************************
 *  \!file net_client.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "net_client.h"

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

	NetClient::NetClient()
		:m_ConnectSocket(-1)
	{
		;
	}
	NetClient::~NetClient(){ 
		stop();
	}

	void NetClient::run(const string &ip, const long &port){
		if(m_ConnectSocket >= 0){
			cout << "already connect socket is [" << m_ConnectSocket << "]" << endl;
		}

		struct sockaddr_in address;
		bzero( &address, sizeof( address ) );
		address.sin_family = AF_INET;
		inet_pton( AF_INET, ip.c_str(), &address.sin_addr );
		address.sin_port = htons( port );

		int m_ConnectSocket = socket( PF_INET, SOCK_STREAM, 0 );
		assert( m_ConnectSocket>= 0 ); 

		if( connect( m_ConnectSocket, (struct sockaddr * ) &address, sizeof( address ) ) == 0 ){
			addSocket(m_ConnectSocket);
		}
		else{
			cout << "connect failed" << endl;
		}

	}
	void NetClient::stop(){ 
		cout << "socket client is closing." << endl;
		close(m_ConnectSocket); 
	}
}
