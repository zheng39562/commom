/**********************************************************
 *  \!file tcp_client.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _tcp_client_H
#define _tcp_client_H

#include "tcp_link.h"

//! \brief	TCP client
//! \note	职责：
//				* 主动连接
//				* 发送和接收数据包。保证数据包的完整性和正确性。
//	\note	基于HPSocket的client和server分离 无法公共继承。
class FrTcpClient : public FrTcpLinker, public CTcpPullClientListener{
	public:
		FrTcpClient();
		~FrTcpClient();
	public:
		virtual bool start(const std::string &ip, unsigned int port);
		virtual bool stop();

		//! \brief	发送数据。
		bool send(const Universal::BinaryMemory &binary);
	private:
		//! \brief	回调函数都是基于HPSocket的，具体机制请查看HPSocket文档。
		virtual EnHandleResult OnConnect(ITcpClient* pSender, SocketId socketId);
		virtual EnHandleResult OnSend(ITcpClient* pSender, SocketId socketId, const BYTE* pData, int iLength);
		virtual EnHandleResult OnReceive(ITcpClient* pSender, SocketId socketId, int iLength);
		virtual EnHandleResult OnClose(ITcpClient* pSender, SocketId socketId, EnSocketOperation enOperation, int iErrorCode);
	private:
		CTcpPullClientPtr m_pClient;
};

#endif 

