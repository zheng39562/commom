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
#include "fr_public/pub_thread.h"
#include "tcp_link.h"
#include "tcp_struct.h"
#include "tcp_server_thread.h"

//! \brief	TCP client
//! \note	职责：
//				* 主动连接
//				* 发送和接收数据包。保证数据包的完整性和正确性。
//! \note	其他：
//				* 协议，处理逻辑与server完全一致。缺陷，bug和注意点请查看server(避免维护两份注视出现不一致的问题)
class FrTcpClient : public FrTcpLinker {
	public:
		FrTcpClient(uint32 threadNum = 2, uint32 _maxBufferSize = 0xFFFF);
		virtual ~FrTcpClient();
	public:
		virtual bool run(const std::string &ip, uint32 port);
};

#endif 

