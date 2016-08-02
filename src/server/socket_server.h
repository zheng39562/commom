/**********************************************************
 * \file socket_server.h
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _socket_server_H
#define _socket_server_H

#include "common_define.h"
#include "event2/event.h"

//
// interface
//
namespace ServerConnection{
	//! \brief	外界开启服务的函数.
	//! \param[in] ip 十进制IP	
	void startServer( const string &ip, const long &port, void );
	//! \brief	外界开启http服务的接口函数.
	void startHttpServer( const string &ip, const long &port );

}  // namespace ServerConnection{


namespace ServerConnection{

	//*******************************************************************************
	//event_base* pBase = event_base_new();
	// event_base_priority_init();
	//event_config
	//event_config_new();
	//event_base_new_config();
	//event_base_free( pBase );
	//
	//
	//*******************************************************************************
	
	//! \brief	套接字接受类。监听当前主机的一个端口，并持续接收对应数据。	
	//! \note	当前基于libevent构建。	
	class DrSocketServer : public DrThread {
		public:
			DrSocketServer( const string &port );
			DrSocketServer( const int &socket );
			virtual ~DrSocketServer();
		private:
			event_base*			m_EventBase;

		private:
			void listen();
		public:
			virtual execute();
	}
} //  namespace ServerConnection{

#endif 

