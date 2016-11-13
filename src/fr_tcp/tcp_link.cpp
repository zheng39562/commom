/**********************************************************
 *  \file tcp_link.cpp
 *  \brief
 *  \note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#include "tcp_link.h"

#include "fr_public/pub_memory.h"

#ifndef _WIN32
#include <sys/epoll.h>
#endif

using namespace std;
using namespace boost;
using namespace Universal;

FrTcpLinker::FrTcpLinker()
	:m_pETC(NULL),
	 m_pConnectCB(NULL),
	 m_pDisconnectCB(NULL),
	 m_pSendCB(NULL),
	 m_pReceiveCB(NULL)
{
	;
}

void FrTcpLinker::execConnectCB(Socket socket){
	if(m_pConnectCB != NULL){
		(*m_pConnectCB)(socket, m_pETC);
	}
}

void FrTcpLinker::execDisconnectCB(Socket socket){
	if(m_pDisconnectCB != NULL){
		(*m_pDisconnectCB)(socket, m_pETC);
	}
}

void FrTcpLinker::execSendCB(Socket socket){
	if(m_pSendCB != NULL){
		(*m_pSendCB)(socket, m_pETC);
	}
}

void FrTcpLinker::execReceiveCB(Socket socket, const Universal::BinaryMemoryPtr &pBinary){
	if(m_pReceiveCB != NULL){
		(*m_pReceiveCB)(socket, pBinary, m_pETC);
	}
}

void FrTcpLinker::setCallBack(fp_connect_cb connect_cb, fp_disconnect_cb disconn_cb, fp_send_cb send_cb, fp_receive_cb receive_cb, void* etc){
	m_pConnectCB = connect_cb;
	m_pDisconnectCB = disconn_cb;
	m_pSendCB = send_cb;
	m_pReceiveCB = receive_cb;
	m_pETC = etc;
}

