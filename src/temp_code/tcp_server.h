/*
 * author zheng39562@163.com
 * date 2016/10/26
 * version v.1.0
 */
#ifndef _tcp_server_H
#define _tcp_server_H

#include <iostream>
#include "tcp_link.h"

//! \brief	tcp	服务类
//! \note	职责：
//! \note		* 监听端口。
//! \note		* 收发数据包。保证数据包的完整性和正确性。
//! \note	继承于 CTcpPullServerListener
//! \todo	还未增加数据验证，如果需要增加私有协议，需要和客户端协商验证结构。
class FrTcpServer : public FrTcpLinker, public CTcpPullServerListener{
	public:
		FrTcpServer();
		~FrTcpServer();
	public:
		virtual bool start(const std::string &ip, unsigned int port);
		virtual bool stop();

		//! \brief	断开某个连接。
		bool disconnect(SocketId socketId);

		//! \brief	发送数据。
		//! \note	消息发送不需要会在内部封包（当前为3字节的头）。
		bool send(SocketId socketId, const Universal::BinaryMemory &binary);
		//! \brief	根据socketId群发。
		bool sendGroup(const std::vector<SocketId> &sockets, const Universal::BinaryMemory &binary);
		//! \brief	广播
		bool sendAll(const Universal::BinaryMemory &binary);
	private:
		//! \brief	回调函数都是基于HPSocket的，具体机制请查看HPSocket文档。
		virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen);
		virtual EnHandleResult OnAccept(ITcpServer* pSender, SocketId socketId, SOCKET soClient);
		virtual EnHandleResult OnSend(ITcpServer* pSender, SocketId socketId, const BYTE* pData, int iLength);
		virtual EnHandleResult OnReceive(ITcpServer* pSender, SocketId socketId, int iLength);
		virtual EnHandleResult OnClose(ITcpServer* pSender, SocketId socketId, EnSocketOperation enOperation, int iErrorCode);
		virtual EnHandleResult OnShutdown(ITcpServer* pSender);
	private:
		CTcpPullServerPtr m_pServer;
};

#endif


