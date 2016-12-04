/**********************************************************
 *  \file tcp_link.h
 *  \brief
 *  \note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _tcp_link_H
#define _tcp_link_H

#include "tcp_define.h"
#include "fr_public/pub_thread.h"
#include "fr_public/pub_memory.h"
#include "fr_template/lock_queue.hpp"
#include "tcp_server_thread.h"
#include "tcp_struct.h"

enum eEventResult{
	eEventResult_OK = 0
};

//! \brief	tcp连接基类。
//! \note	使用：该类为抽象类无法直接使用。
//! \note	继承须知：
//				* 如果需要进行回调，则必须对所有回调指针进行初始化。否则行为异常。
//				* 抽象了start和stop。server中的listen 和 client中的connect建议直接重载start and stop。主要，需要继承该类的实现。
//! \note	细节
//				* 回调指针每个对象自行维护。
//					* 考虑过作为静态类。但这导致所有的继承类都用公共的回调。对应client和server的公共继承将无法实现。
class FrTcpLinker : public Universal::FrThread {
	public:
		typedef std::pair<Socket, Universal::BinaryMemoryPtr> PushMsg;
	public:
		FrTcpLinker(uint32 threadNum, uint32 _maxBufferSize);
		virtual ~FrTcpLinker();
	public:
		//! \brief	运行套接字
		virtual bool run(const std::string &ip, uint32 port)=0;
		//! \brief	关闭
		virtual bool stop();
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		// 回调函数。由派生类自行实现。
		//
		//! \brief	发送成功回调
		virtual eEventResult onSend(Socket socket);
		//! \brief	接收回调
		//! \note	返回数据指针。避免数据的反复copy
		virtual eEventResult onReceive(Socket socket, Universal::BinaryMemoryPtr pBinary);
		//! \brief	链接成功回调(成功)
		virtual eEventResult onConnect(Socket socket);
		//! \brief	链接断开回调(成功)
		virtual eEventResult onDisconnect(Socket socket);
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		//! \brief	发送数据。
		//! \note	组播和广播有大致实现相同，不服用send是为了减少锁开销。
		bool send(Socket socket, const Universal::BinaryMemoryPtr &pBuffer);
	protected:
		//! \brief	epoll事件处理。
		virtual void execute();
		//! \breif	处理连接事件。
		virtual void dealConnect(Socket socket);
		virtual void dealDisconnect(Socket socket);
		virtual void dealSend(Socket socket);
		virtual void dealRecv(Socket socket);

		//! \brief	添加socket到epoll中
		void addSocketToEpoll(Socket socket);
	private:
		//! \brief	获取空闲线程。
		FrTcpServerThread* getReadyThread();
		void dealEvent(Socket socket, eSocketEventType eEventType);
		void dealEvent(Socket socket, eSocketEventType eEventType, Universal::BinaryMemoryPtr pPacket);
	protected:
		Socket m_EpollSocket;
	private:
		Universal::LockQueue<PushMsg> m_MsgQueue;  // 外界发送的消息队列。
		std::list<FrTcpServerThread*> m_ServerThreads;
		FrTcpMsgProcess* m_pTcpMsgProcess;
		uint32 m_MaxBufferSize;
		TcpCacheTree m_TcpCacheTree;
};


#endif 

