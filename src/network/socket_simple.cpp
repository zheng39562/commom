/**********************************************************
 *  \!file socket_simple.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "socket_simple.h"

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

using namespace std;
namespace MyTest{
	const int MAX_MSGSIZE = 1000;

	bool setAddress( const string &ip, const int &port, sockaddr_in &address ){
		bzero( &address, sizeof( address ) );
		address.sin_family = AF_INET;
		inet_pton( AF_INET, ip.c_str(), &address.sin_addr );
		address.sin_port = htons( port );
		return true;
	}

	void socket_client( const string &ip, const int &port ){
		struct sockaddr_in address;
		setAddress( ip, port, address );

		int sock = socket( PF_INET, SOCK_STREAM, 0 );
		assert( sock>= 0 ); 

		if( connect( sock, (struct sockaddr * ) &address, sizeof( address ) ) == 0 ){
			string sSendMsg;
			char sRecvBuf[ MAX_MSGSIZE ];
			while( cin >> sSendMsg && !sSendMsg.empty() && sSendMsg != "end" ){
				cout << " SendMsg : [" << sSendMsg << "]" << endl;

				send( sock, sSendMsg.c_str(), sSendMsg.size(), 0 );
				memset( sRecvBuf, '\0', MAX_MSGSIZE );
				recv( sock, sRecvBuf, MAX_MSGSIZE, 0 );
				cout << "Recv Msg [" << string(sRecvBuf) << "]" << endl;
			}

			cout << "socket client is closing." << endl;
			close( sock );
		}
		else{
			cout << "connect failed" << endl;
		}
	}

	void socket_server( const string &ip, const int &port ){
		struct sockaddr_in address;
		setAddress( ip, port, address );

		int sock = socket( PF_INET, SOCK_STREAM, 0 );
		assert( sock >= 0 );

		int on(0);
		setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on) );

		int ret = bind( sock, ( struct sockaddr* ) &address,  sizeof( address ) );
		if( ret == -1 ){ cout << "errno " << errno << endl; }
		assert( ret != -1 ); 
		ret = listen( sock, 5 );
		if( ret == -1 ){ cout << "errno " << errno << endl; }
		assert( ret != -1 ); 

		struct sockaddr_in client;
		socklen_t client_addrlength = sizeof( client );

		int connfd = accept( sock, ( struct sockaddr * ) &client, &client_addrlength );

		char remote[ INET_ADDRSTRLEN ];
		printf( "connected with ip : %s and port : %d\n", inet_ntop( AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN ), ntohs( client.sin_port ) );

		string recvMsg;
		char buf[ MAX_MSGSIZE ];	
		if( connfd >= 0 ){
			// receive data
			memset( buf, '\0', MAX_MSGSIZE );

			int recvIndex(1);
			while( recvIndex != 0 ){
				cout << " recvIndex : " << recvIndex << endl;
				recvIndex = recv( connfd, buf, MAX_MSGSIZE-1, 0 );
				recvMsg = string( buf );
				cout << " recv data [" << recvMsg << "]" << endl;

				send( connfd, recvMsg.c_str(), recvMsg.size(), 0 );	
				cout << " send msg to " << recvMsg << endl;

				recvMsg.clear();
				memset( buf, '\0', MAX_MSGSIZE );

				sleep(1);
			}
		}
		cout << " connect is closing " << endl;
		close( connfd );

		cout << " server will close. " << endl;
		close( sock );
	}
}

