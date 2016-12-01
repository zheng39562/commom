/**********************************************************
 *  \!file tcp_server_thread.h
 *  \!brief
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _tcp_server_thread_H
#define _tcp_server_thread_H

#include "fr_public/pub_thread.h"
#include "tcp_define.h"
#include "tcp_struct.h"
#include "tcp_msg_process.h"

//! \brief	TCPServer的处理线程。
//! \note	职责
//				* 接受主线程的消息，调用对应函数执行处理。
//! \note	细节
//				* 本身不含有处理程序，仅用于开辟线程用来执行函数。
//! \note	使用：
//				* 该类不建议单独使用。如果要使用tcp模块 请使用FrTcpServer类。
class FrTcpServerThread : public Universal::FrThread{
	public:
		FrTcpServerThread(FrTcpMsgProcess* _pTcpMsgProcess);
		virtual ~FrTcpServerThread();
	public:
		//! \brief	唤醒线程执行操作。
		//! \note	保证该线程已执行完才可以再次唤醒。
		//! \retval false 表示唤醒失败，基本可以视为该线程还有操作正在执行。
		bool active(FrTcpCachePtr pCache, eSocketEventType eventType, Universal::BinaryMemoryPtr pPacket = Universal::BinaryMemoryPtr());

		//! \brief	该线程是否就绪(休眠)。
		//! \note	考虑过加锁，但感觉没有必要。
		//! \todo	可以考虑修改成回调。
		inline bool ready(){ return m_Ready; }
	private:
		//! \note	实际处理函数。当执行结束后会挂起。等待下一次激活。
		virtual void execute();
	private:
		FrTcpMsgProcess* m_pTcpMsgProcess;
		FrTcpCachePtr m_pCurCache;
		eSocketEventType m_eSocketEventType;
		Universal::BinaryMemoryPtr m_pCurPacket;
		bool m_Ready;
};

#endif 

