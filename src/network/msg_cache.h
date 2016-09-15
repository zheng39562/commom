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
#include "boost/shared_ptr.hpp"
#include "event2/bufferevent.h"
#include "tool/lock_queue.hpp"
#include "tool/single_mode.hpp"
#include "tool/common_mutex.h"
#include "network/net_protocol_convert.h"
#include "tool/single_mode.hpp"

namespace Network{
	class ConnectKey;
	//! \brief	消息中间件，用于分隔应用层和传输层。
	//! \note	类似tcp，拥有一条读通道一条写通道。
	//! \note	读通道通过读取msg信息，输出packer包
	//! \note	写通道通过读取packer包，输出msg信息
	//! \attetion	注意不建议直接使用该类。使用下面typedef的Single类更加合适。
	//! \todo	可以考虑对读写通道分离。
	//! \todo	更高抽象：当前是从应用视角来理解该类，在传输和通信视角会感到困惑。
	class MsgServer{
		public:
			MsgServer();
			~MsgServer();
		public:
			//! 写通道
			bool emptyW(ConnectKey connectKey);
			void pushPacker(const PackerPtr &pPacker);
			MsgPtr popMsg(ConnectKey connectKey);

			//! 读通道
			bool emptyR();
			void pushMsg(const MsgPtr &pMsg);
			PackerPtr popPacker();
		protected:
			MsgCache m_MsgCache;				//! 缓存类需要自身带锁。

			PackerCache m_PackerCache;			//! 缓存类需要自身带锁。
			std::map<ConnectKey, std::string> m_IncompleteMsg;
	};
	typedef DesignMode::SingleMode<MsgServer> SingleMsgServer;
}
#endif 

