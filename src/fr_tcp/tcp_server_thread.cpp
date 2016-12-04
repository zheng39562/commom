/**********************************************************
 *  \!file tcp_server_thread.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "tcp_server_thread.h"

#include <sys/socket.h>
#include "fr_public/pub_memory.h"

using namespace std;
using namespace boost;
using namespace Universal;

const uint32 MAX_BUFFER_SIZE = 0xFFFF;

FrTcpServerThread::FrTcpServerThread(FrTcpMsgProcess* _pTcpMsgProcess)
	:m_pTcpMsgProcess(_pTcpMsgProcess),
	 m_pCurCache(0),
	 m_eSocketEventType(eSocketEventType_Invalid),
	 m_pCurPacket(),
	 m_Ready(false)
{
	FrThread::start();
}

FrTcpServerThread::~FrTcpServerThread(){ 
	FrThread::stop();
}

bool FrTcpServerThread::active(FrTcpCachePtr pCache, eSocketEventType eventType, BinaryMemoryPtr pPacket){
	if(m_pTcpMsgProcess != NULL && m_Ready){
		m_Ready = false;
		m_pCurCache = pCache;
		m_pCurPacket = pPacket;
		m_eSocketEventType = eventType;
		resume();
		return true;
	}
	return false;
}

void FrTcpServerThread::execute(){
	while(m_Running){
		m_Ready = true;
		pause();
		m_Ready = false;
		if(m_pTcpMsgProcess != NULL){
			switch(m_eSocketEventType){
				case eSocketEventType_Push:
					m_pTcpMsgProcess->push(m_pCurCache, m_pCurPacket);
					break;
				case eSocketEventType_Recv:
					m_pTcpMsgProcess->send(m_pCurCache);
					break;
				case eSocketEventType_Send:
					m_pTcpMsgProcess->recv(m_pCurCache);
					break;
				case eSocketEventType_Connect:
					m_pTcpMsgProcess->connect(m_pCurCache);
					break;
				case eSocketEventType_Disconnect:
					m_pTcpMsgProcess->disconnect(m_pCurCache);
					break;
				default:
					DEBUG_E("未知事件类型。");
					break;
			}
		}
	}
}

