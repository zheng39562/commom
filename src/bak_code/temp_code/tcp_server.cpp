#include "tcp_server.h"

#include "protocol.h"
#include "fr_public/pub_memory.h"

using namespace std;
using namespace Universal;

FrTcpServer::FrTcpServer()
	:m_pServer(this)
{ ; }

FrTcpServer::~FrTcpServer(){ ; }

bool FrTcpServer::start(const string &ip, unsigned int port){
	if(m_pServer->Start(ip.c_str(), port)){
		DEBUG_I("启动端口监听 [" << ip << ":" << port << "]");
		return true;
	}
	else{
		DEBUG_I("监听失败");
		return false;
	}
	return true;
}

bool FrTcpServer::stop(){
	if(m_pServer->Stop()){
		DEBUG_I("停止server。");
		return true;
	}
	return false;
}

bool FrTcpServer::disconnect(SocketId socketId){
	return m_pServer->Disconnect(socketId);
}

bool FrTcpServer::send(SocketId socketId, const BinaryMemory &binary){
	BinaryMemory packet(binary);
	if(proto_pack(packet)){
		return m_pServer->Send(socketId, (const Byte*)packet.getBuffer(), packet.getBufferSize());
	}
	return false;
}

bool FrTcpServer::sendGroup(const vector<SocketId> &sockets, const BinaryMemory &binary){
	bool bRet(true);
	BinaryMemory packet(binary);
	if(proto_pack(packet)){
		for(vector<SocketId>::const_iterator citer = sockets.begin(); citer != sockets.end(); ++citer){
			if(!m_pServer->Send(*citer, (const Byte*)packet.getBuffer(), packet.getBufferSize())){
				DEBUG_W("向[" << *citer << "]发送消息失败。请检查连接是否可用。");
				bRet = false;
			}
		}
	}
	else{
		DEBUG_E("打包出错。");
		bRet = false;
	}
	return bRet;
}

bool FrTcpServer::sendAll(const Universal::BinaryMemory &binary){
	SocketId* pSockets = new SocketId[m_pServer->GetConnectionCount()];
	uint64 count(0);
	if(m_pServer->GetAllConnectionIDs(pSockets, count)){
		BinaryMemory packet(binary);
		if(proto_pack(packet)){
			for(uint64 i=0; i<count; ++i){
				if(!m_pServer->Send(pSockets[i], (const Byte*)packet.getBuffer(), packet.getBufferSize())){
					DEBUG_W("向[" << pSockets[i] << "]发送消息失败。请检查连接是否可用。");
				}
			}
		}
		else{
			DEBUG_E("打包出错。");
			return false;
		}
	}
	return true;
}

EnHandleResult FrTcpServer::OnPrepareListen(ITcpServer* pSender, SOCKET soListen){
	char sAddress[40];
	int iAddressLen = sizeof(sAddress) / sizeof(char);
	unsigned short port;
			
	pSender->GetListenAddress(sAddress, iAddressLen, port);
	DEBUG_I("开始监听端口 [" << string(sAddress, iAddressLen) << ":" << port << "]");
	return HR_OK;
}

EnHandleResult FrTcpServer::OnAccept(ITcpServer* pSender, SocketId socketId, SOCKET soClient){
	char sAddress[40];
	int iAddressLen = sizeof(sAddress) / sizeof(char);
	unsigned short port;

	pSender->GetRemoteAddress(socketId, sAddress, iAddressLen, port);
	DEBUG_I("接收客户端连接请求。 [" << string(sAddress, iAddressLen) << ":" << port << "]");

	execConnectCB(socketId);;
	return HR_OK;
}

EnHandleResult FrTcpServer::OnSend(ITcpServer* pSender, SocketId socketId, const BYTE* pData, int iLength){
	DEBUG_D("服务端发送数据成功。数据内容[" << string((const char*)pData, iLength) << "]");
	execSendCB(socketId);
	return HR_OK;
}

EnHandleResult FrTcpServer::OnReceive(ITcpServer* pSender, SocketId socketId, int iLength){
	if((unsigned int)iLength >= PROTO_HEAD_SIZE){
		ITcpPullServer* pServer	= ITcpPullServer::FromS(pSender);
		if(pServer != NULL){
			Byte* pHead = new Byte[PROTO_HEAD_SIZE];
			Byte* pBody(NULL);
			while(HR_OK == pServer->Peek(socketId, pHead, PROTO_HEAD_SIZE)){
				size_t size = proto_bypeToSize(pHead) + PROTO_HEAD_SIZE; 
				if(size <= (size_t)iLength){
					if(pBody == NULL){
						pBody = new Byte[iLength];
					}

					if(HR_OK == pServer->Fetch(socketId, pBody, size)){
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
				else{
					break;
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

EnHandleResult FrTcpServer::OnClose(ITcpServer* pSender, SocketId socketId, EnSocketOperation enOperation, int iErrorCode){
	DEBUG_E(12321);
	char sAddress[40];
	int iAddressLen = sizeof(sAddress) / sizeof(char);
	unsigned short port;

	pSender->GetRemoteAddress(socketId, sAddress, iAddressLen, port);
	DEBUG_I("服务端[" << string(sAddress, iAddressLen) << ":" << port << "]连接断开");
	execDisconnectCB(socketId);
	return HR_OK;
}

EnHandleResult FrTcpServer::OnShutdown(ITcpServer* pSender){
	DEBUG_I("连接关闭");
	return HR_OK;
}



