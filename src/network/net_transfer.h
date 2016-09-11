/**********************************************************
 *  \!file net_transfer.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _net_transfer_H
#define _net_transfer_H

#include <string>
#include <list>
#include "event2/event.h"
#include "boost/shared_ptr.hpp"
#include "network/net_packer.h"

namespace Network{
	enum eSocketRWOpt{
		socketRWOpt_Read,
		socketRWOpt_Write,
		socketRWOpt_RW,
	};
	//! \brief	暂无更多设计，仅作为基类存在。
	class Transfer{
		public:
			Transfer(){;}
			virtual ~Transfer(){;}
	};
	//! \brief	网络传输类。
	//! \note	基于TCP libevent socket 结构完成。不排除未来支持UDP
	//! \note	套接字，通过自定义的数据包，进行接收和写入。
	//! \todo	可以再扩展IO/文件等传输类。可能需要对类名进行一定修正。
	//! \todo	PS：可以用过在外连接好socket的方式来当作客户端使用。后期会在此类基础上做一个简易的客户端版本。
	class NetMsgTransfer : public Transfer {
		public:
			NetMsgTransfer();
			virtual ~NetMsgTransfer();
		public:
			int addSocket(const int &socket, eSocketRWOpt socketRWOpt = socketRWOpt_RW);
			int removeSocket(const int &socket);
			void run();
		private:
			static void writeBack(bufferevent* bev, void *ctx);
			static void readBack(bufferevent* bev, void *ctx);
			static void errorBack(bufferevent* bev, short events, void *ctx);
		public:
			event_base* m_EventBase;
			//MsgCache m_MsgCache;
	};
}

#endif 

