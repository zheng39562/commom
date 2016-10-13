/**********************************************************
 *  \!file net_connection.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _net_connection_H
#define _net_connection_H

#include "event.h"
#include "event2/listener.h"
#include "boost/shared_ptr.hpp"
#include "common/common_memory.h"
#include "common/common_thread.h"
#include "network/net_define.h"
#include "network/net_packer.h"
#include "network/net_struct.h"

namespace Network{
	extern const int READ_BUFFER_SIZE;
	extern const int WRITE_BUFFER_SIZE;

	enum eSocketRWOpt{
		socketRWOpt_Read,
		socketRWOpt_Write,
		socketRWOpt_RW,
	};
	class MsgStruct{
		public:
			MsgStruct():isAlready(false),buffer(){;}
			MsgStruct(const MsgStruct &ref){
				isAlready = ref.isAlready;
				buffer = ref.buffer;
			}
		public:
			bool isAlready;
			Universal::BinaryMemory buffer;
		public:
			inline bool allowWrite()const{ 
				DEBUG_D("buffer size" << buffer.getBufferSize() << string((char*)buffer.getBuffer(), buffer.getBufferSize()));
				return isAlready && !buffer.empty(); 
			} 
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
	class NetTransfer : public Transfer, public Universal::PThread{
		public:
			typedef map<ConnectKey, MsgStruct> MsgCache;
		public:
			NetTransfer();
			virtual ~NetTransfer();
		public:
			//! \todo	增加读写的限制功能
			void addSocket(int socket, eSocketRWOpt socketRWOpt = socketRWOpt_RW);

			virtual bool run(const std::string &ip, const long &port)=0;
			//! \brief	发送数据包。
			//! \note	数据包中包含发送地址。该功能只支持端到端的发送。
			void sendPacker(const PackerPtr &pPacker);
			//! \brief	想所有维持的连接中发送消息。
			//! \note	函数会忽略包中的保存地址——无论包中的链接地址是否存在。
			void sendAll(const PackerPtr &pPacker);
			//! \brief	想所有维持的连接中发送消息。
			void sendAll(const Universal::BinaryMemory &buffer);

			void recvMsg(const ConnectKey &connectKey, const char *pMsg, size_t size);
			PackerPtr recvPacker();

			void enableWrite(const ConnectKey &connectKey);
		//private:
		protected:
			//! \brief	写 回调函数。
			static void writeBack(ConnectKey connectKey, void *ctx);
			//! \brief	读 回调函数。
			static void readBack(ConnectKey connectKey, void *ctx);
			//! \brief	报错 回调函数。
			static void errorBack(ConnectKey connectKey, short events, void *ctx);

			//! \brief	注册连接。
			void registerConnect(const ConnectKey &connectKey);
			void unregisterConnect(const ConnectKey &connectKey);
		protected:
			event_base* m_EventBase;
		private:
			static char* m_s_pRecvBuffer;

			Universal::PMutex m_MSend;  //! \todo 需要考虑是否可以减小锁粒度。
			Universal::PMutex m_MRecv;
			MsgCache m_MsgCache;
			PackerCache m_PackerCache;			//! 缓存类需要自身带锁。
			std::map<ConnectKey, Universal::BinaryMemory> m_IncompleteMsg; //!

			std::list<ConnectKey> m_ConnectKeyList;  //! 当前所有链接。
			std::set<ConnectKey> m_UnconnectSet; //! 已断开但未处理链接。
	};

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
			virtual bool run(const std::string &ip, const long &port);

			void stop();
		protected:
			virtual void execute();
		private:
			evutil_socket_t m_ConnectSocket;
	};
	typedef DesignMode::SingleMode<NetClient> SingleNetClient;

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
			virtual bool run(const std::string &ip, const long &port);
		protected:
			virtual void execute();
		private:
			static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *ctx);
		private:
			evconnlistener* m_Listener;
	};
	typedef DesignMode::SingleMode<NetServer> SingleNetServer;
}

#endif 

