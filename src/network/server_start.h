/**********************************************************
 * \file server_start.h
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _server_start_H
#define _server_start_H

/*
 * 1 server 和 client 完成 : 基于libevent 要求多线程。
 * 2 pack和unpack包的基本类
 * 3 测试。
 */

namespace ServiceSpace{
	enum eServerMode{
		serverMode_FirstInvalid = -1,
		serverMode_ = 0,
		serverMode_,
		serverMode_LastInvalid
	};
	class Server{
		public:
			Server();
			~Server();
		public:
			void start();
	};
}


#endif 

