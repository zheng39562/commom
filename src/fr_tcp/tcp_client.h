/**********************************************************
 *  \!file tcp_client.h
 *  \!brief	tcp client 类
 *  \!note	连接单一性，可以考虑直接阻塞获取数据。 =.=
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _tcp_client_H
#define _tcp_client_H

#include "tcp_define.h"

//! \brief	TCP client
//! \note	职责：
//				* 主动连接
//				* 发送和接收数据包。保证数据包的完整性和正确性。
//! \note	其他：
//				* 协议，处理逻辑与server完全一致。缺陷，bug和注意点请查看server(避免维护两份注视出现不一致的问题)
class FrTcpClient : public FrTcpLinker, public FrThread{
	public:
		FrTcpClient(uint32 threadNum = 2, uint32 _writeBufferSize = 0xFFFF, uint32 _readBufferSize = 0xFFFF);
		virtual ~FrTcpClient();
	public:
		bool connect(const std::string &ip, unint32 port);
		bool stop();
	private:
		//! \ntoe	 断开和错误暂时不细分。
		//! \attetion	程序关闭时，没有依次关闭所有连接。不确定底层是否会自动关闭。需要测试。
		//				但从实际使用上，这类问题应该不太大。后续如果有这类问题，再解决。
		virtual void execute();
	private:
		//! \brief	处理事件。
		void dealEvent(Socket socket, eSocketEventType eEventType, Universal::BinaryMemoryPtr pPacket = Universal::BinaryMemoryPtr());
	private:
		std::list<FrTcpServerThread*> m_ServerThreads;
		FrTcpCachePtr m_pTcpCache;
};

#endif 

