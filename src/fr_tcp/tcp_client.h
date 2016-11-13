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

#include "tcp_define.h"

//! \brief	TCP client
//! \note	职责：
//				* 主动连接
//				* 发送和接收数据包。保证数据包的完整性和正确性。
class FrTcpClient{
	public:
		FrTcpClient();
		~FrTcpClient();
};

#endif 

