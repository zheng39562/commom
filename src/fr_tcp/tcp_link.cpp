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

FrTcpLinker::FrTcpLinker(uint32 _writeBufferSize, uint32 _readBufferSize)
	:m_TcpMsgProcess(),
	 m_MsgQueue(),
	 m_WriteBufferSize(_writeBufferSize),
	 m_WriteBufferSize(_readBufferSize),
	 m_EpollSocket(SOCKET_UN_INIT_VALUE),
	 m_ListenSocket(SOCKET_UN_INIT_VALUE),
	 m_ServerThreads()
{
	while(threadNum--){
		FrTcpServerThread* pThread = new FrTcpServerThread(&m_TcpMsgProcess);
		m_ServerThreads.push_back(pThread);
	}
}

FrTcpLinker::~FrTcpLinker(){
	for(auto iterThread = m_ServerThreads.begin(); iterThread != m_ServerThreads.end(); ++iterThread){
		if(*iterThread != NULL){
			delete *iterThread; *iterThread = NULL;
		};
	}
}

eEventResult FrTcpLinker::onSend(Socket socket){ return eEventResult_OK; }
eEventResult FrTcpLinker::onReceive(Socket socket, BinaryMemoryPtr pBinary){ return eEventResult_OK; }
eEventResult FrTcpLinker::onConnect(Socket socket){ return eEventResult_OK; }
eEventResult FrTcpLinker::onDisconnect(Socket socket){ return eEventResult_OK; }

bool FrTcpLinker::send(Socket socket, const BinaryMemoryPtr &pBinary){
	m_MsgQueue.push(PushMsg(socket, pBinary));
	return true;
}

FrTcpServerThread* FrTcpLinker::getReadyThread(){
	for(auto iterThread = m_ServerThreads.begin(); iterThread != m_ServerThreads.end(); ++iterThread){
		if((*iterThread)->ready()){
			return *iterThread;
		}
	}
	return NULL;
}

