/**********************************************************
 *  \!file net_msg.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _net_msg_H
#define _net_msg_H

#include "network/net_define.h"
#include "boost/shared_ptr.hpp"
#include "event2/bufferevent.h"
#include "tool/single_mode.hpp"
#include "tool/lock_queue.hpp"
#include "network/net_struct.h"

namespace Network{
	//! \todo 复制构造函数，赋值构造函数等
	class Msg{
		public:
			Msg(const ConnectKey &_key, const std::string &_msg)
				:m_ConnectKey(_key),
				 m_Msg()
			{;}
			~Msg(){;}
		public:
			ConnectKey m_ConnectKey;
			std::string m_Msg;
	};
	typedef boost::shared_ptr<Msg> MsgPtr;
	typedef boost::shared_ptr<const Msg> ConstMsgPtr;
	typedef Universal::LockQueue<MsgPtr> MMsgPtrQueue;
	typedef std::map<ConnectKey, MMsgPtrQueue> MsgCache;
}
#endif 

