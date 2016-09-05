/**********************************************************
 * \file bak_code.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
//#include "bak_code.h"

bool setAddress( const string &ip, const string &port, sockaddr_in &address ){
	bzero( &address, sizeof( address ) );
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip.c_str(), &address.sin_addr );
	address.sin_port = htons( strToInt( port ) );

	return true;
}

bool socketReceive( const string &ip, const string &port ){
	struct sockaddr_in address;
	setAddress( ip, port, address );

	int sock = socket( PF_INET, SOCK_STREAM, 0 );
	assert( sock >= 0 );

	int ret = bind( sock, ( struct sockaddr* ) &address,  sizeof( address ) );
	assert( ret != -1 ); 
	ret = listen( sock, 5 );
	assert( ret != -1 ); 

	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof( client );

	int connfd = accept( sock, ( struct sockaddr * ) &client, &client_addrlength );
	if( connfd >= 0 ){
		char remote[ INET_ADDRSTRLEN ];
		printf( "connected with ip : %s and port : %d\n", inet_ntop( AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN ), ntohs( client.sin_port ) );

		// receive data
		long maxDataSize(100);
		char buf[ maxDataSize ];	
		memset( buf, '\0', maxDataSize );
		recv( connfd, buf, maxDataSize-1, MSG_OOB );
		cout << string(buf) << endl;

		close( connfd );
	}
	else{
		cout << "accept socket failed." << endl;
	}

	close( sock );

	return true;
}
bool socketSend( const string &ip, const string &port ){
	struct sockaddr_in address;
	setAddress( ip, port, address );

	int sock = socket( PF_INET, SOCK_STREAM, 0 );
	assert( sock>= 0 ); 

	if( connect( sock, (struct sockaddr * ) &address, sizeof( address ) ) == 0 ){
		string data = "it is a message."; 
		string urgent = "123456!7";
		send( sock, data.c_str(), data.size(), 0 );
		send( sock, urgent.c_str(), urgent.size(), MSG_OOB );
	}
	else{
		cout << "connect failed" << endl;
	}


	return true;
}
