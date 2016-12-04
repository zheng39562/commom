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
#include <iostream>

#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "fr_public/pub_test.h"
#include "fr_tcp/tcp_test.h"

using namespace std;
using namespace Universal;

class ThreadA : public FrThread{
	public:
		ThreadA():bS(true){}
		virtual ~ThreadA(){
			bS = false;
		}
	private:
		virtual void execute(){
			while(bS){
				cout << " start sleep" << endl;
				frSleep(1000);
			}
		}
		bool bS;
};

bool doEpollTest(std::string arg0 = "", std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "");

int main( int agrc, char **argv ){
	DEBUG_INIT("./log", "test");

	doTcpTest();
//	doEpollTest();

	return 0;
}

bool doEpollTest(string arg0, string arg1, string arg2, string arg3, string arg4){
	string ip("192.168.2.104");
	uint port = 5555;
	Socket m_EpollSocket = epoll_create(100);

	if(m_EpollSocket > 0){
		// init addres
		struct sockaddr_in address;
		bzero(&address, sizeof(address));
		address.sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
		address.sin_port = htons(port);

		Socket m_ListenSocket = socket(PF_INET, SOCK_STREAM, 0);
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
		event.events = EPOLLIN | EPOLLET | EPOLLOUT;
		epoll_ctl(m_EpollSocket, EPOLL_CTL_ADD, m_ListenSocket, &event);

		uint32 maxEvent = 10;
		epoll_event* events = (epoll_event*)calloc(maxEvent, sizeof(epoll_event));
		eSocketEventType socketEventType(eSocketEventType_Invalid);
		DEBUG_E("开始等待链接进入");
		uint32 time = 1000;
		while(time--){
			/*
			DEBUG_E("链接可读");
			struct sockaddr_in address;
			socklen_t len = sizeof(address);
			bzero(&address, sizeof(address));
			Socket socketRecv = accept(m_ListenSocket, (sockaddr*)&address, &len);
			if(socketRecv > 0){
				DEBUG_D("接受到链接");
			}
			*/
			int32 eventNum = epoll_wait(m_EpollSocket, events, maxEvent, 50); // 50ms timeout
			DEBUG_D("eventNum " << eventNum);
			for(int32 index = 0; index < eventNum; ++index){
				Socket socket = events[index].data.fd;
				socketEventType = eSocketEventType_Invalid;
				// error and disconnect
				if((events[index].events & EPOLLHUP) || (events[index].events & EPOLLERR)){
					DEBUG_E("链接报错");
				}
				// read : 普通数据和带外数据事件一致（暂时）
				if((events[index].events & EPOLLIN) || (events[index].events & EPOLLPRI)){ 
					DEBUG_E("链接可读");
					struct sockaddr_in address;
					socklen_t len = sizeof(address);
					bzero(&address, sizeof(address));
					Socket socketRecv = accept(socket, (sockaddr*)&address, &len);
					if(socketRecv > 0){
						DEBUG_D("接受到链接");
					}
				}
				// write
				if(events[index].events & EPOLLOUT){
					DEBUG_E("链接可写");
				}
			}
			frSleep(2000);
		}

	}
	return true;
}



