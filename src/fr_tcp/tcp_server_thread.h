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
#include "tcp_link.h"

//! \brief	TCPServer的处理线程。
//! \note	职责
//				* 接受主线程的消息，执行对应的处理。
//! \note	细节
//				* 单个socket是否并发由主线程决定,主线程需要保证除缓存外的其他顺序性和一致性。
//				* 只需要考虑缓存锁.
//				* 缓存和锁等不当作静态：避免限制类使用范围。代价仅是增加2个指针变量。
//! \note	使用：
//				1 调用函数：FrTcpLinker::setCallBack and setCache. 进行初始化。
//					* 不进行初始化会导致行为异常。
//					* 在线程运行中更改回调和缓存，也有一定几率导致异常(多线程情景下)。
//				* 
class FrTcpServerThread : public Universal::FrThread, public FrTcpLinker{
	public:
		FrTcpServerThread(TcpCache* pTcpCache, Universal::FrMutex* pSocketMutex);
		virtual ~FrTcpServerThread();
	public:
		//! \brief	唤醒线程执行操作。
		//! \note	保证该线程已执行完才可以再次唤醒。
		//! \retval false 表示唤醒失败，基本可以视为该线程还有操作正在执行。
		bool active(Socket socket, eSocketEventType eventType);

		//! \brief	该线程是否就绪(休眠)。
		//! \note	考虑过加锁，但感觉没有必要。
		//! \todo	可以考虑修改成回调。
		inline bool ready(){ return m_Ready; }
	private:

		virtual void execute();

		void send(Socket socket);
		void recv(Socket socket);
		void connect(Socket socket);
		void disconnect(Socket socket);

		//! \brief	或者完整的包。
		void recvPackets(Socket socket, Universal::BinaryMemory &binary);
	private:
		Universal::FrMutex* m_pSocketMutex;
		TcpCache* m_pTcpCache;

		Byte* m_pBufferTmp;

		Socket m_CurSocket;
		eSocketEventType m_eSocketEventType;
		bool m_Ready;

};

#endif 

