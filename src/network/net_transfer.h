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
#include "network/net_struct.h"
#include "network/net_msg_struct.h"

namespace Network{
	extern const int READ_BUFFER_SIZE;
	extern const int WRITE_BUFFER_SIZE;

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
	//! 
	//! \note	功能：
	//! \note	1	监听端口，维护TCP连接，连接关闭通知。
	//! \note	2	读写功能：通过自定义的数据包，进行接收和写入。拥有读通道和写通道。
	//! \note		* 读通道通过读取msg信息，输出packer包
	//! \note		* 写通道通过读取packer包，输出msg信息
	//!
	//! \todo	可以再扩展IO/文件等传输类。可能需要对类名进行一定修正。
	//! \todo	PS：可以用过在外连接好socket的方式来当作客户端使用。后期会在此类基础上做一个简易的客户端版本。
	class NetTransfer : public Transfer {
		public:
			NetTransfer();
			virtual ~NetTransfer();
		public:
			virtual bool run(const std::string &ip, const long &port)=0;
			//! \brief	注销连接。
			//! \brief	写通道
			bool emptyW(ConnectKey connectKey);
			void pushPacker(const PackerPtr &pPacker);
			MsgPtr popMsg(ConnectKey connectKey);

			//! \brief	读通道
			bool emptyR();
			void pushMsg(const MsgPtr &pMsg);
			PackerPtr popPacker();
		protected:
			int addSocket(const int &socket, eSocketRWOpt socketRWOpt = socketRWOpt_RW);
		private:
			//! \brief	写 回调函数。
			static void writeBack(ConnectKey connectKey, void *ctx);
			//! \brief	读 回调函数。
			static void readBack(ConnectKey connectKey, void *ctx);
			//! \brief	报错 回调函数。
			static void errorBack(ConnectKey connectKey, short events, void *ctx);

			//! \brief	注册连接。
			void registerConnect(const ConnectKey &connectKey);
			void unregisterConnect(const ConnectKey &connectKey);
		public:
			static char* m_s_ReadBuf;

			event_base* m_EventBase;

			MsgCache m_MsgCache;				//! 缓存类需要自身带锁。
			PackerCache m_PackerCache;			//! 缓存类需要自身带锁。
			std::map<ConnectKey, std::string> m_IncompleteMsg;
	};
}

#endif 

