/**********************************************************
 *  \!file msg_cache.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _msg_cache_H
#define _msg_cache_H

#include <map>
#include <string>
#include "tool/single_mode.hpp"
#include "network/net_transfer.h"

namespace Network{
	//! \brief	net端口监听和消息转发类：server版。
	//! 
	//! \attetion	注意不建议直接使用该类。使用下面typedef的Single类更加合适。
	class NetServer : public NetTransfer{
		public:
			NetServer();
			virtual ~NetServer();
		public:
			//! \brief	运行服务：监听端口，并创建对应连接。
			//! \note	集合listen 和 accpet 的功能
			//! \note	该函数使用轮询的方式反复找到链接，建议单独开辟线程运行。
			virtual void run(const std::string &ip, const long &port);
		private:
			bool m_IsRunning;
	};
	typedef DesignMode::SingleMode<NetServer> SingleNetServer;
}
#endif 

