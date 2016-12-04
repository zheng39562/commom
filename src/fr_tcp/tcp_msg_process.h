/**********************************************************
 *  !file tcp_msg_process.h
 *  !brief
 * 
 * !version 
 * * !author zheng39562@163.com
**********************************************************/
#ifndef _tcp_msg_process_H
#define _tcp_msg_process_H

#include "fr_public/pub_memory.h"
#include "fr_public/pub_thread.h"
#include "tcp_define.h"
#include "tcp_struct.h"

//! \brief	连接成功的回调函数指针
typedef void (*fp_connect_cb)(Socket socket, void* etc);
//! \brief	发送数据成功。
typedef void (*fp_disconnect_cb)(Socket socket, void* etc);
//! \brief	发送数据成功。
typedef void (*fp_send_cb)(Socket socket, void* etc);
//! \brief	连接数据的回调函数指针
typedef void (*fp_receive_cb)(Socket socket, const Universal::BinaryMemoryPtr &pBinary, void* etc);

//! \brief	消息处理类（暂命名）
//! \note	职责：
//				* 根据触发事件处理消息，并调用外界的回调函数。
//					* 已有事件：connect, disconnet, push, send, recv
//					* 已有回调：connect, disconnet, send, recv
//				* 缓存未处理的消息内容：read/write
//! \note	细节：
//				* 发送数据实际上是写入数据到缓存。实际发送存在延时（由底层触发决定）
class FrTcpMsgProcess{
	public:
		FrTcpMsgProcess(Socket _epollSocket);
		FrTcpMsgProcess(const FrTcpMsgProcess &ref);
		virtual ~FrTcpMsgProcess();
	public:
		//! \brief	推送消息包到队列中等待处理.
		//! \note	调用该函数之前，要保证链接正常。
		bool push(FrTcpCachePtr pTcpCache, Universal::BinaryMemoryPtr pPacket);
		//! \brief	发送消息。
		void send(FrTcpCachePtr pTcpCache);
		//! \brief	接受数据。如有完整包，则调用回调函数。
		void recv(FrTcpCachePtr pTcpCache);
		//! \brief	新连接处理。
		void connect(FrTcpCachePtr pTcpCache);
		//! \brief	链接断开。
		void disconnect(FrTcpCachePtr pTcpCache);
		//! \brief	回调函数以及对应的公共数据指针。
		//! \note	已有：具体回调参考typedef的解释。 
		//! \param[in] etc 公共数据指针。
		void setCallBack(fp_connect_cb connect_cb, fp_disconnect_cb disconn_cb, fp_send_cb send_cb, fp_receive_cb receive_cb, void* etc);
	protected:
		void execConnectCB(Socket socket);
		void execDisconnectCB(Socket socket);
		void execSendCB(Socket socket);
		void execReceiveCB(Socket socket, const Universal::BinaryMemoryPtr &pBinary);

		//! \biref	更新写状态。
		void updateEpollStatus(Socket socket);
	private:
		//! \brief	或者完整的包。
		void recvPackets(Socket socket, Universal::BinaryMemory &binary);
	protected:
		Socket m_EpollSocket;
	private:
		void* m_pETC;
		fp_connect_cb m_pConnectCB;
		fp_disconnect_cb m_pDisconnectCB;
		fp_send_cb m_pSendCB;
		fp_receive_cb m_pReceiveCB;
};


#endif 
 

