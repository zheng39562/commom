/**********************************************************
 *  \!file net_client.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _net_client_H
#define _net_client_H

#include <map>
#include <string>
#include "tool/single_mode.hpp"
#include "network/net_transfer.h"

namespace Network{
	//! \brief	net端口监听和消息转发类：客户端版。
	//! 
	//! \attetion	注意不建议直接使用该类。使用下面typedef的Single类更加合适。
	class NetClient : public NetTransfer{
		public:
			NetClient();
			virtual ~NetClient();
		public:
			//! \brief	运行服务：连接服务器，并创建对应连接。
			//! \note	集合 和 accpet 的功能
			//! \note	建议单独开辟线程运行。
			virtual void run(const std::string &ip, const long &port);

			void stop();
		private:
			int m_ConnectSocket;
	};
	typedef DesignMode::SingleMode<NetClient> SingleNetClient;
}

#endif 

