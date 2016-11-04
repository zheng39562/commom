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

//! \brief	连接成功的回调函数指针
typedef void (*fp_connect_cb)(SocketId socketId, void* etc);
//! \brief	发送数据成功。
typedef void (*fp_disconnect_cb)(SocketId socketId, void* etc);
//! \brief	发送数据成功。
typedef void (*fp_send_cb)(SocketId socketId, void* etc);
//! \brief	连接数据的回调函数指针
typedef void (*fp_receive_cb)(SocketId socketId, const Universal::BinaryMemoryPtr &pBinary, void* etc);
//! \brief	tcp连接基类。
//! \note	确定 server 和client 共有的行为和数据。
class FrTcpLinker{
	public:
		FrTcpLinker();
		virtual ~FrTcpLinker()=default;
	public:
		//! \brief	开始连接：操作意义需要派生类定义
		virtual bool start(const std::string &ip, unsigned int port)=0;
		//! \brief	结束连接：操作意义需要派生类定义(要与start函数对应)
		virtual bool stop()=0;

		//! \brief	回调函数以及对应的公共数据指针。
		//! \note	已有：具体回调参考typedef的解释。 
		//! \param[in] etc 公共数据指针。
		void setCallBack(fp_connect_cb connect_cb, fp_disconnect_cb disconn_cb, fp_send_cb send_cb, fp_receive_cb receive_cb, void* etc);
	protected:
		void execConnectCB(SocketId socketId);
		void execDisconnectCB(SocketId socketId);
		void execSendCB(SocketId socketId);
		void execReceiveCB(SocketId socketId, const Universal::BinaryMemoryPtr &pBinary);
	protected:
		void* m_pETC;
		fp_connect_cb m_pConnectCB;
		fp_disconnect_cb m_pDisconnectCB;
		fp_send_cb m_pSendCB;
		fp_receive_cb m_pReceiveCB;
};


#endif 

