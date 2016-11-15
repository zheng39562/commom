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
class FrTcpClient{
	public:
		FrTcpClient();
		~FrTcpClient();
	public:

};

#endif 

