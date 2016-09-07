/**********************************************************
 * \file main.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "common_define.h"

#include <iostream>
#include "template_function.hpp"
#include "sql_operator.h"
#include "boost/scoped_ptr.hpp"
#include "network/socket_simple.h"
#include "network/net_transfer.h"
#include "tool/string_util.h"
#include "tool/single_mode.hpp"

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
using namespace Universal;
using namespace MyTest;
using namespace Network;

int main( int agrc, char **argv ){
	if( agrc >= 2 ){
		string option( argv[1] );
		string ip( "127.0.0.1" );
		string port( "6000" );

		sockaddr_in address;
		setAddress( ip, Inside::strToInt(port), address );
		int sock = socket( PF_INET, SOCK_STREAM, 0 );
		int connfd(-1);
		assert( sock >= 0 );
		if( option == "c" || option == "client" || option == "-c" ){
			cout << " run socket client. " << endl;
			socket_client( ip, Inside::strToInt(port) );
		}
		else{
			cout << " run socket server. " << endl;
			//socket_server( ip, Inside::strToInt(port) );

			int on(0);
			setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on) );

			int ret = bind( sock, ( struct sockaddr* ) &address,  sizeof( address ) );
			cout << errno << endl;
			assert( ret != -1 ); 
			ret = listen( sock, 5 );
			assert( ret != -1 ); 

			struct sockaddr_in client;
			socklen_t client_addrlength = sizeof( client );

			connfd = accept( sock, ( struct sockaddr * ) &client, &client_addrlength );

			char remote[1000];
			printf( "connected with ip : %s and port : %d\n", inet_ntop( AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN ), ntohs( client.sin_port ) );

			if( connfd >= 0 ){
				DesignMode::SingleMode<NetMsgTransfer>::getInstance()->addSocket( connfd );
			}
		}

		DesignMode::SingleMode<NetMsgTransfer>::getInstance()->run();

		cout << " connect is closing " << endl; close( connfd );
		cout << " server will close. " << endl; close( sock );
	}
	else{
		cout << " param size is wrong. size [" << agrc << "]" << endl;
	}
	return 0;
}

