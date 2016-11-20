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
#include "fr_public/pub_memory.h"
#include "tcp_server_thread.h"

//! \brief	tcp连接基类。
//! \note	使用：该类为抽象类无法直接使用。
//! \note	继承须知：
//				* 如果需要进行回调，则必须对所有回调指针进行初始化。否则行为异常。
//! \note	细节
//				* 回调指针每个对象自行维护。
//					* 考虑过作为静态类。但这导致所有的继承类都用公共的回调。对应client和server的公共继承将无法实现。
class FrTcpLinker{
	public:
		FrTcpLinker(uint32 threadNum, uint32 _writeBufferSize, uint32 _readBufferSize);
		virtual ~FrTcpLinker();
	public:
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
		//! \brief	获取空闲线程。
		FrTcpServerThread* getReadyThread();
	protected:
		Socket m_EpollSocket;
		Socket m_RunSocket;
		LockQueue<PushMsg> m_MsgQueue;  // 外界发送的消息队列。
		FrTcpMsgProcess m_TcpMsgProcess;
		uint32 m_WriteBufferSize;
		uint32 m_ReadBufferSize;
		std::list<FrTcpServerThread*> m_ServerThreads;
};


#endif 

