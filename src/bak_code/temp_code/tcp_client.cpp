/**********************************************************
 *  \!file tcp_client.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "tcp_client.h"

#include "protocol.h"

using namespace std;
using namespace Universal;

FrTcpClient::FrTcpClient()
	:m_pClient(this)
{ ; }

FrTcpClient::~FrTcpClient(){
	;
}

bool FrTcpClient::start(const string &ip, unsigned int port){
	bool bAsynConn(true);
	return m_pClient->Start(ip.c_str(), port, bAsynConn);
}

bool FrTcpClient::stop(){
	return m_pClient->Stop();
}

bool FrTcpClient::send(const BinaryMemory &binary){
	BinaryMemory packet(binary);
	if(proto_pack(packet)){
		return m_pClient->Send((const Byte*)packet.getBuffer(), packet.getBufferSize());
	}
	return false;
}

EnHandleResult FrTcpClient::OnConnect(ITcpClient* pSender, SocketId socketId){
	char sAddress[40];
	int iAddressLen = sizeof(sAddress) / sizeof(char);
	unsigned short port;

	pSender->GetRemoteHost(sAddress, iAddressLen, port);
	DEBUG_I("连接服务器成功。 [" << string(sAddress, iAddressLen) << ":" << port << "]");

	execConnectCB(socketId);
	return HR_OK;
}

EnHandleResult FrTcpClient::OnSend(ITcpClient* pSender, SocketId socketId, const BYTE* pData, int iLength){
	DEBUG_D("客户端：数据发送成功.");
	execSendCB(socketId);
	return HR_OK;
}

EnHandleResult FrTcpClient::OnReceive(ITcpClient* pSender, SocketId socketId, int iLength){
	if((unsigned int)iLength >= PROTO_HEAD_SIZE){
		ITcpPullClient* pClient	= ITcpPullClient::FromS(pSender);
		if(pClient != NULL){
			byte* pHead = new byte[PROTO_HEAD_SIZE];
			Byte* pBody(NULL);
			while(HR_OK == pClient->Peek(pHead, PROTO_HEAD_SIZE)){
				size_t size = proto_bypeToSize(pHead) + PROTO_HEAD_SIZE; 
				if(size <= (size_t)iLength){
					if(pBody == NULL){
						pBody = new Byte[iLength];
					}

					if (HR_OK == pClient->Fetch(pBody, size)){
						iLength -= size;
						BinaryMemoryPtr pBinary(new BinaryMemory(pBody, size));
						if(proto_unpack(*pBinary)){
							execReceiveCB(socketId, pBinary);
						}
						else{
							DEBUG_E("数据包解包失败。请检查包的完成性。");
							pBinary->print();
						}
					}
				}
			}

			if (pBody != NULL){
				delete pBody; pBody = NULL;
			}
			if (pHead != NULL){
				delete pHead; pHead = NULL;
			}
		}
	}
	return HR_OK;
}

EnHandleResult FrTcpClient::OnClose(ITcpClient* pSender, SocketId socketId, EnSocketOperation enOperation, int iErrorCode){
	char sAddress[40];
	int iAddressLen = sizeof(sAddress) / sizeof(char);
	unsigned short port;

	pSender->GetRemoteHost(sAddress, iAddressLen, port);
	DEBUG_I("客户端[" << string(sAddress, iAddressLen) << ":" << port << "]连接断开");
	execDisconnectCB(socketId);
	return HR_OK;
}

