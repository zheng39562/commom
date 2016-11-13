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
typedef void (*fp_connect_cb)(Socket socket, void* etc);
//! \brief	发送数据成功。
typedef void (*fp_disconnect_cb)(Socket socket, void* etc);
//! \brief	发送数据成功。
typedef void (*fp_send_cb)(Socket socket, void* etc);
//! \brief	连接数据的回调函数指针
typedef void (*fp_receive_cb)(Socket socket, const Universal::BinaryMemoryPtr &pBinary, void* etc);

//! \brief	tcp连接基类。
//! \note	使用：该类为抽象类无法直接使用。
//! \note	继承须知：
//				* 如果需要进行回调，则必须对所有回调指针进行初始化。否则行为异常。
//! \note	细节
//				* 回调指针每个对象自行维护。
//					* 考虑过作为静态类。但这导致所有的继承类都用公共的回调。对应client和server的公共继承将无法实现。
class FrTcpLinker{
	public:
		FrTcpLinker();
		virtual ~FrTcpLinker()=default;
	public:
		//! \brief	回调函数以及对应的公共数据指针。
		//! \note	已有：具体回调参考typedef的解释。 
		//! \param[in] etc 公共数据指针。
		void setCallBack(fp_connect_cb connect_cb, fp_disconnect_cb disconn_cb, fp_send_cb send_cb, fp_receive_cb receive_cb, void* etc);
	protected:
		void execConnectCB(Socket socket);
		void execDisconnectCB(Socket socket);
		void execSendCB(Socket socket);
		void execReceiveCB(Socket socket, const Universal::BinaryMemoryPtr &pBinary);
	protected:
	private:
		void* m_pETC;
		fp_connect_cb m_pConnectCB;
		fp_disconnect_cb m_pDisconnectCB;
		fp_send_cb m_pSendCB;
		fp_receive_cb m_pReceiveCB;
};


#endif 

