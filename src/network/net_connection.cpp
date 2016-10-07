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
#include "net_connection.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "event2/buffer.h"
#include "event2/bufferevent.h"
#include "tool/single_mode.hpp"
#include "network/net_protocol.h"

using namespace std;

namespace Network{
	const int READ_BUFFER_SIZE = 4000;
	const int WRITE_BUFFER_SIZE = 4000;

	char* NetTransfer::m_s_ReadBuf = new char[READ_BUFFER_SIZE];

	NetTransfer::NetTransfer()
		:m_EventBase(NULL)
	{
		m_EventBase = event_base_new();

		event_base_dispatch(m_EventBase);
	}
	NetTransfer::~NetTransfer(){
		if(m_EventBase != NULL){
			event_base_free(m_EventBase);
		}
	}

	bool NetTransfer::emptyW(ConnectKey connectKey){
		if(m_MsgCache.find(connectKey) != m_MsgCache.end()){
			return m_MsgCache.find(connectKey)->second.empty();
		}
		return false;
	}

	void NetTransfer::pushPacker(const PackerPtr &pPacker){
		ConnectKey connectKey = pPacker->getConnectKey();
		if(m_MsgCache.find(connectKey) != m_MsgCache.end()){
			convertPackerToMsg(pPacker, m_MsgCache.find(connectKey)->second);
		}
		else{
			DEBUG_D("未注册该连接：连接查找不到。");
		}
	}

	MsgPtr NetTransfer::popMsg(ConnectKey connectKey){
		MsgPtr pMsg;
		if(m_MsgCache.find(connectKey) != m_MsgCache.end()){
			if(!m_MsgCache.find(connectKey)->second.empty()){
				pMsg = m_MsgCache.find(connectKey)->second.pop();
			}
		}
		return pMsg;
	}

	bool NetTransfer::emptyR(){ return m_PackerCache.empty(); }

	void NetTransfer::pushMsg(const MsgPtr &pMsg){
		ConnectKey connectKey = pMsg->m_ConnectKey;
		if(m_IncompleteMsg.find(connectKey) == m_IncompleteMsg.end()){
			m_IncompleteMsg.insert(pair<ConnectKey, std::string>(connectKey, std::string()));
		}
		m_IncompleteMsg.find(connectKey)->second += pMsg->m_Msg;

		convertMsgToPacker(connectKey, m_IncompleteMsg.find(connectKey)->second, m_PackerCache);
	}

	PackerPtr NetTransfer::popPacker(){
		PackerPtr pPacker;
		if(!emptyR()){
			pPacker = m_PackerCache.pop();
		}
		return pPacker;
	}

	int NetTransfer::addSocket(const int &socket, eSocketRWOpt socketRWOpt){
		ConnectKey connectKey = bufferevent_socket_new(m_EventBase, socket, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
		if(connectKey == NULL){
			DEBUG_D("new bufferevent failed.");
		}

		bufferevent_setcb(connectKey, NetTransfer::readBack, NetTransfer::writeBack, NetTransfer::errorBack, this);
		bufferevent_enable(connectKey, EV_READ|EV_WRITE);

		registerConnect(connectKey);
	}

	void NetTransfer::writeBack(ConnectKey connectKey, void *ctx){
		NetTransfer* pTransfer = static_cast<NetTransfer*>(ctx);
		DEBUG_D(" write ");
		MsgPtr pMsg = pTransfer->popMsg(connectKey);
		DEBUG_D(" write " << pMsg->m_Msg);
		bufferevent_write(connectKey, pMsg->m_Msg.c_str(), pMsg->m_Msg.size());
	}
	void NetTransfer::readBack(ConnectKey connectKey, void *ctx){
		NetTransfer* pTransfer = static_cast<NetTransfer*>(ctx);
		DEBUG_D(" readBack ");
		evbuffer* output = bufferevent_get_input(connectKey);
		memset(m_s_ReadBuf, '\0', sizeof(char)*READ_BUFFER_SIZE);
		if( bufferevent_read(connectKey, m_s_ReadBuf, READ_BUFFER_SIZE) > 0 ){
			pTransfer->pushMsg(MsgPtr(new Msg(connectKey, m_s_ReadBuf)));
		}
	}
	void NetTransfer::errorBack(ConnectKey connectKey, short events, void *ctx){
		NetTransfer* pTransfer = static_cast<NetTransfer*>(ctx);
		DEBUG_D(" error back ");
		bufferevent_free(connectKey);
		pTransfer->unregisterConnect(connectKey);
		DEBUG_D(" connectKey addr : " << connectKey);
	}


	void NetTransfer::registerConnect(const ConnectKey &connectKey){
		if(m_MsgCache.find(connectKey) == m_MsgCache.end()){
			m_MsgCache.insert(MsgCache::value_type(connectKey, MMsgPtrQueue()));
		}
	}

	void NetTransfer::unregisterConnect(const ConnectKey &connectKey){
		if(m_MsgCache.find(connectKey) != m_MsgCache.end()){
			m_MsgCache.erase(m_MsgCache.find(connectKey));
		}
	}
}

namespace Network{
	using namespace std;

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
		bzero( &address, sizeof( address ) );
		address.sin_family = AF_INET;
		inet_pton( AF_INET, ip.c_str(), &address.sin_addr );
		address.sin_port = htons( port );

		int m_ConnectSocket = socket( PF_INET, SOCK_STREAM, 0 );
		if( m_ConnectSocket < 0){
			DEBUG_E("未生成正确的socker");
			return false;
		}

		if( connect( m_ConnectSocket, (struct sockaddr * ) &address, sizeof( address ) ) == 0 ){
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
	using namespace std;

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
		if( m_ListenSocket < 0){
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

