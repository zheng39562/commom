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

FrTcpServerThread::FrTcpServerThread(TcpCache* _pTcpCache, FrMutex* _pSocketMutex)
	:m_pSocketMutex(_pSocketMutex),
	 m_pTcpCache(_pTcpCache),
	 m_CurSocket(0),
	 m_eSocketEventType(eSocketEventType_Invalid),
	 m_Ready(false),
	 m_pBufferTmp(NULL)
{
	m_pBufferTmp = new Byte[MAX_BUFFER_SIZE];
	memset(m_pBufferTmp, 0, MAX_BUFFER_SIZE);

	FrThread::start();
}

FrTcpServerThread::~FrTcpServerThread(){ 
	FrThread::stop();

	if(m_pBufferTmp != NULL){
		delete m_pBufferTmp; m_pBufferTmp = NULL;
	}
}

bool FrTcpServerThread::active(Socket socket, eSocketEventType eventType){
	if(m_pTcpCache != NULL && m_Ready){
		m_Ready = false;
		m_CurSocket = socket;
		m_eSocketEventType = eventType;
		resume();
		return true;
	}
	return false;
}

void FrTcpServerThread::execute(){
	while(m_Running){
		pause();
		switch(m_eSocketEventType){
			case eSocketEventType_Recv:
				send(m_CurSocket);
				break;
			case eSocketEventType_Send:
				recv(m_CurSocket);
				break;
			case eSocketEventType_Connect:
				connect(m_CurSocket);
				break;
			case eSocketEventType_Disconnect:
				disconnect(m_CurSocket);
				break;
			default:
				break;
		}

		m_Ready = true;
	}
}

void FrTcpServerThread::send(Socket socket){
	auto iterCache = m_pTcpCache->find(socket);
	if(iterCache != m_pTcpCache->end()){
		mutex::scoped_lock scopedLock(iterCache->second.mutexWrite);
		size_t sendSize = ::send(socket, iterCache->second.bufferWrite.getBuffer(), iterCache->second.bufferWrite.getBufferSize(), MSG_DONTWAIT);
		iterCache->second.bufferWrite.delBuffer(0, sendSize);
		
		execSendCB(socket);
	}
}

void FrTcpServerThread::recv(Socket socket){
	auto iterCache = m_pTcpCache->find(socket);
	if(iterCache != m_pTcpCache->end()){
		mutex::scoped_lock scopedLock(iterCache->second.mutexRead);
		size_t recvSize(0);
		while((recvSize = ::recv(socket, m_pBufferTmp, MAX_BUFFER_SIZE, MSG_DONTWAIT)) > 0){
			iterCache->second.bufferRead.addBuffer(m_pBufferTmp, recvSize);
			memset(m_pBufferTmp, 0, recvSize);
		}

		recvPackets(socket, iterCache->second.bufferRead);
	}
}

void FrTcpServerThread::connect(Socket socket){
	execConnectCB(socket);
}

void FrTcpServerThread::disconnect(Socket socket){
	execDisconnectCB(socket);
}

void FrTcpServerThread::recvPackets(Socket socket, BinaryMemory &binary){
	proto_size size(0);
	while((size = *(proto_size*)binary.getBuffer()) <= (proto_size)binary.getBufferSize()){
		// 直接裁剪掉包头
		BinaryMemoryPtr pBinary(new BinaryMemory((Byte*)binary.getBuffer() + sizeof(proto_size), size - sizeof(proto_size)));
		binary.delBuffer(0, size);
		execReceiveCB(socket, pBinary);
	}
}

