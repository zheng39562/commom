/**********************************************************
 *  \!file net_connection.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "network/net_connection.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "template/single_mode.hpp"
#include "network/net_protocol.h"

using namespace std;

namespace Network{
	const int READ_BUFFER_SIZE = 4000;
	const int WRITE_BUFFER_SIZE = 4000;

	NetTransfer::NetTransfer()
		:m_EventBase(NULL),
		 m_MMsgCache(),
		 m_MsgCache(),
		 m_pRecvBuffer(NULL)
	{
		m_EventBase = event_base_new();

		m_pRecvBuffer = new char[READ_BUFFER_SIZE];

		event_base_dispatch(m_EventBase);
	}
	NetTransfer::~NetTransfer(){
		if(m_EventBase != NULL){
			event_base_free(m_EventBase);
		}
		if(m_pRecvBuffer != NULL){
			delete m_pRecvBuffer; m_pRecvBuffer = NULL;
		}
	}

	void NetTransfer::sendPacker(const PackerPtr &pPacker){
		ConnectKey connectKey = pPacker->getConnectKey();
		m_MMsgCache.lock();
		MsgCache::iterator iterMsgCache = m_MsgCache.find(connectKey); 
		if(iterMsgCache != m_MsgCache.end()){
			convertPackerToMsg(pPacker, iterMsgCache->second.buffer);
			if(iterMsgCache->second.allowWrite()){
				BinaryMemory& buffer = iterMsgCache->second.buffer;
				if(buffer.getBufferSize() > WRITE_BUFFER_SIZE){
					bufferevent_write(connectKey, buffer.getBuffer(), WRITE_BUFFER_SIZE);
					buffer.delBuffer(0, WRITE_BUFFER_SIZE);
				}
				else{
					bufferevent_write(connectKey, buffer.getBuffer(), buffer.getBufferSize());
					buffer.clearBuffer();
				}

				iterMsgCache->second.isAlready = false;
			}
		}
		else{
			DEBUG_D("未注册该连接：连接查找不到。");
		}
		m_MMsgCache.unlock();
	}

	PackerPtr NetTransfer::recvPacker(){
		return m_PackerCache.pop();
	}

	void NetTransfer::recvMsg(const ConnectKey &connectKey, const char *pMsg, size_t size){
		if(m_IncompleteMsg.find(connectKey) == m_IncompleteMsg.end()){
			m_IncompleteMsg.insert(make_pair(connectKey, BinaryMemory()));
		}

		m_IncompleteMsg.find(connectKey)->second.addBuffer(pMsg, size);
		convertMsgToPacker(connectKey, m_IncompleteMsg.find(connectKey)->second, m_PackerCache);
	}

	void NetTransfer::enableWrite(const ConnectKey &connectKey){
		m_MMsgCache.lock();
		MsgCache::iterator iterMsgCache = m_MsgCache.find(connectKey); 
		if(iterMsgCache != m_MsgCache.end() ){
			if(iterMsgCache->second.allowWrite()){
				BinaryMemory& buffer = iterMsgCache->second.buffer;
				if(buffer.getBufferSize() > WRITE_BUFFER_SIZE){
					bufferevent_write(connectKey, buffer.getBuffer(), WRITE_BUFFER_SIZE);
					buffer.delBuffer(0, WRITE_BUFFER_SIZE);
				}
				else{
					bufferevent_write(connectKey, buffer.getBuffer(), buffer.getBufferSize());
					buffer.clearBuffer();
				}

				iterMsgCache->second.isAlready = false;
			}
			else{
				iterMsgCache->second.isAlready = true;
			}
		}
		else{
			DEBUG_E("该连接未注册，请先注册");
		}
		m_MMsgCache.unlock();
	}

	int NetTransfer::addSocket(const int &socket, eSocketRWOpt socketRWOpt){
		ConnectKey connectKey = bufferevent_socket_new(m_EventBase, socket, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
		if(connectKey == NULL){
			DEBUG_E("new bufferevent failed.");
		}

		bufferevent_setcb(connectKey, NetTransfer::readBack, NetTransfer::writeBack, NetTransfer::errorBack, this);
		bufferevent_enable(connectKey, EV_READ|EV_WRITE);

		registerConnect(connectKey);
	}

	void NetTransfer::writeBack(ConnectKey connectKey, void *ctx){
		NetTransfer* pTransfer = static_cast<NetTransfer*>(ctx);
		pTransfer->enableWrite(connectKey);
	}
	void NetTransfer::readBack(ConnectKey connectKey, void *ctx){
		NetTransfer* pTransfer = static_cast<NetTransfer*>(ctx);
		evbuffer* output = bufferevent_get_input(connectKey);

		memset(m_pRecvBuffer, 0, READ_BUFFER_SIZE);
		size_t recvSize = bufferevent_read(connectKey, m_pRecvBuffer, READ_BUFFER_SIZE);
		if(recvSize > 0){
			DEBUG_D("接受数据：" << m_pRecvBuffer);
			pTransfer->recvMsg(connectKey, m_pRecvBuffer, recvSize);
		}
	}
	void NetTransfer::errorBack(ConnectKey connectKey, short events, void *ctx){
		DEBUG_E("连接异常");
		NetTransfer* pTransfer = static_cast<NetTransfer*>(ctx);
		bufferevent_free(connectKey);
		pTransfer->unregisterConnect(connectKey);
	}


	void NetTransfer::registerConnect(const ConnectKey &connectKey){
		m_MMsgCache.lock();
		if(m_MsgCache.find(connectKey) == m_MsgCache.end()){
			m_MsgCache.insert(MsgCache::value_type(connectKey, MsgStruct()));
		}
		m_MMsgCache.unlock();

		if(m_IncompleteMsg.find(connectKey) == m_IncompleteMsg.end()){
			m_IncompleteMsg.insert(make_pair(connectKey, string()));
		}
	}

	void NetTransfer::unregisterConnect(const ConnectKey &connectKey){
		m_MMsgCache.lock();
		if(m_MsgCache.find(connectKey) != m_MsgCache.end()){
			m_MsgCache.erase(m_MsgCache.find(connectKey));
		}
		m_MMsgCache.unlock();

		if(m_IncompleteMsg.find(connectKey) != m_IncompleteMsg.end()){
			m_IncompleteMsg.erase(m_IncompleteMsg.find(connectKey));
		}
	}
}

namespace Network{
	NetClient::NetClient()
		:m_ConnectSocket(-1)
	{
		;
	}
	NetClient::~NetClient(){ 
		stop();
	}

	bool NetClient::run(const string &ip, const long &port){
		if(m_ConnectSocket >= 0){
			cout << "already connect socket is [" << m_ConnectSocket << "]" << endl;
		}

		struct sockaddr_in address;
		bzero(&address, sizeof(address));
		address.sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
		address.sin_port = htons(port);

		int m_ConnectSocket = socket(PF_INET, SOCK_STREAM, 0);
		if(m_ConnectSocket < 0){
			DEBUG_E("未生成正确的socker");
			return false;
		}

		if(connect(m_ConnectSocket, (struct sockaddr *) &address, sizeof(address)) == 0){
			addSocket(m_ConnectSocket);
		}
		else{
			cout << "connect failed" << endl;
		}

		return true;
	}
	void NetClient::stop(){ 
		cout << "socket client is closing." << endl;
		close(m_ConnectSocket); 
	}
}

namespace Network{

	NetServer::NetServer()
		:m_IsRunning(false)
	{
		;
	}
	NetServer::~NetServer(){ 
		m_IsRunning = false;
	}

	void NetServer::execute(){
		struct sockaddr_in client;
		socklen_t client_addrlength = sizeof(client);
		int connfd(-1);
		char remote[ INET_ADDRSTRLEN ];
		while(m_IsRunning){
			// 印象里accept是阻塞的。需要确认下。
			connfd = accept(m_ListenSocket, (struct sockaddr *) &client, &client_addrlength);

			printf("connected with ip : %s and port : %d\n", 
					inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN), 
					ntohs(client.sin_port));

			if(connfd >= 0){
				addSocket(connfd);
			}
			else{
				break;
			}
		}

		close(m_ListenSocket);
		m_ListenSocket = -1;
	}

	bool NetServer::run(const string &ip, const long &port){
		m_IsRunning = true;

		struct sockaddr_in address;
		bzero(&address, sizeof(address));
		address.sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
		address.sin_port = htons(port);

		m_ListenSocket = socket(PF_INET, SOCK_STREAM, 0);
		if(m_ListenSocket < 0){
			DEBUG_E("未生成正确的socker");
			return false;
		}

		int on(0);
		setsockopt(m_ListenSocket,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

		int ret = bind(m_ListenSocket, (struct sockaddr*) &address,  sizeof(address));
		if(ret == -1){ DEBUG_E("绑定socket出错。errno :" << errno); return false; }
		ret = listen(m_ListenSocket, 5);
		if(ret == -1){ DEBUG_E("监听socket出错。errno :" << errno); return false; }

		return start();
	}
}

