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

namespace Network{
	//! \todo 复制构造函数，赋值构造函数等
	class Msg{
		public:
			Msg(bufferevent *_bev, const string &msg);
			~Msg();
		private:
			bufferevent *m_Bev;
			std::string m_Msg;
	}
	typedef boost::shared_ptr<Msg> MsgPtr;

	//! \brief	信息缓存和处理类。
	//! \note	读取：从缓存中读取数据，打包成msg结构。如已接收到完整的包，则打包，否则放入缓存。
	//! \note	写入：将包分解成msg结构，等待写入事件激活。
	class MsgCache{
		public:
			MsgCache();
			~MsgCache();
		public:
			int sendPack(PackerConstPtr pPacker);
			void recvMsg(MsgPtr msg);
			void activeWrite(bufferevent *bev);
			MsgPtr popPacker();
		private:
			LockQueue<PackerPtr> m_PackerQueue;
			std::map<bufferevent*, std::string> m_ReadCache;
			std::map<bufferevent*, LockQueue<MsgPtr>> m_WriteCache;
			std::list<MsgPtr> m_WriteMsgList;
	};
}
#endif 

