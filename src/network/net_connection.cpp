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
#include "template/single_mode.hpp"
#include "network/net_protocol.h"

using namespace std;
using namespace Universal;

namespace Network{
	const int READ_BUFFER_SIZE = 1000;
	const int WRITE_BUFFER_SIZE = 1000;

	char* NetTransfer::m_s_pRecvBuffer = new char[READ_BUFFER_SIZE];

	NetTransfer::NetTransfer()
		:m_EventBase(NULL),
		 m_MSend(),
		 m_MRecv(),
		 m_MsgCache()
	{ 
		m_EventBase = event_base_new();
	}
	NetTransfer::~NetTransfer(){
		if(m_EventBase != NULL){
			event_base_free(m_EventBase);
		}
	}

	void NetTransfer::sendPacker(const PackerPtr &pPacker){
		ConnectKey connectKey = pPacker->getConnectKey();
		m_MSend.lock();
		MsgCache::iterator iterMsgCache = m_MsgCache.find(connectKey); 
		if(iterMsgCache != m_MsgCache.end()){
			BinaryMemory& buffer = iterMsgCache->second.buffer;
			if(buffer.getBufferSize() > WRITE_BUFFER_SIZE * 2){
				usleep(100*1000);
			}

			convertPackerToMsg(pPacker, buffer);
			if(iterMsgCache->second.allowWrite()){
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
		m_MSend.unlock();
	}

	void NetTransfer::sendAll(const PackerPtr &pPacker){
		sendAll(BinaryMemory(pPacker->getBuffer(), pPacker->getBufferSize()));
	}
	void NetTransfer::sendAll(const Universal::BinaryMemory &buffer){
		m_MSend.lock();

		DEBUG_D("开始广播发送消息 : [" << string((char*)buffer.getBuffer(), buffer.getBufferSize()) << "] size " << buffer.getBufferSize());
		list<ConnectKey>::iterator iterConnectKey = m_ConnectKeyList.begin();
		while(iterConnectKey != m_ConnectKeyList.end()){
			if(m_UnconnectSet.find(*iterConnectKey) != m_UnconnectSet.end()){
				DEBUG_D("已关闭链接buffer，从广播队列中删除");
				m_UnconnectSet.erase(m_UnconnectSet.find(*iterConnectKey));
				iterConnectKey = m_ConnectKeyList.erase(iterConnectKey);
				continue;
			}
			else{
				MsgCache::iterator iterMsgCache = m_MsgCache.find(*iterConnectKey); 
				if(iterMsgCache != m_MsgCache.end()){
					BinaryMemory& bufferRef = iterMsgCache->second.buffer;
					PackerPtr pPacker(new Packer(*iterConnectKey));
					pPacker->setBuffer(buffer);

					convertPackerToMsg(pPacker, bufferRef);
					if(iterMsgCache->second.allowWrite()){
						if(bufferRef.getBufferSize() > WRITE_BUFFER_SIZE){
							DEBUG_D("发送数据 : " << string((char*)bufferRef.getBuffer(), WRITE_BUFFER_SIZE));
							bufferevent_write(*iterConnectKey, bufferRef.getBuffer(), WRITE_BUFFER_SIZE);
							bufferRef.delBuffer(0, WRITE_BUFFER_SIZE);
						}
						else{
							DEBUG_D("发送数据 : " << string((char*)bufferRef.getBuffer(), bufferRef.getBufferSize()));
							bufferevent_write(*iterConnectKey, bufferRef.getBuffer(), bufferRef.getBufferSize());
							bufferRef.clearBuffer();
						}

						iterMsgCache->second.isAlready = false;
					}
					else{
						DEBUG_D("写未就绪");
					}
				}
				else{
					DEBUG_D("未注册该连接：连接查找不到。");
				}
			}

			++iterConnectKey;
		}
		m_MSend.unlock();
	}

	PackerPtr NetTransfer::recvPacker(){
		return m_PackerCache.pop();
	}

	void NetTransfer::recvMsg(const ConnectKey &connectKey, const char *pMsg, size_t size){
		if(m_IncompleteMsg.find(connectKey) != m_IncompleteMsg.end()){
			m_IncompleteMsg.find(connectKey)->second.addBuffer(pMsg, size);
			convertMsgToPacker(connectKey, m_IncompleteMsg.find(connectKey)->second, m_PackerCache);
		}
		else{
			DEBUG_E("该链接未注册。请先注册。");
		}

	}

	void NetTransfer::enableWrite(const ConnectKey &connectKey){
		m_MSend.lock();
		MsgCache::iterator iterMsgCache = m_MsgCache.find(connectKey); 
		if(iterMsgCache != m_MsgCache.end() ){
			iterMsgCache->second.isAlready = true;
			DEBUG_D("激活写标志。");
			if(iterMsgCache->second.allowWrite()){
				BinaryMemory& buffer = iterMsgCache->second.buffer;
				if(buffer.getBufferSize() > WRITE_BUFFER_SIZE){
					DEBUG_D("发送数据 : " << string((char*)buffer.getBuffer(), WRITE_BUFFER_SIZE));
					bufferevent_write(connectKey, buffer.getBuffer(), WRITE_BUFFER_SIZE);
					buffer.delBuffer(0, WRITE_BUFFER_SIZE);
				}
				else{
					DEBUG_D("发送数据 : " << string((char*)buffer.getBuffer(), buffer.getBufferSize()));
					bufferevent_write(connectKey, buffer.getBuffer(), buffer.getBufferSize());
					buffer.clearBuffer();
				}

				iterMsgCache->second.isAlready = false;
			}
			else{
				DEBUG_D("没有数据。");
			}
		}
		else{
			DEBUG_E("该连接未注册，请先注册");
		}
		m_MSend.unlock();
	}

	void NetTransfer::writeBack(ConnectKey connectKey, void *ctx){
		DEBUG_D("链接写就绪。");
		NetTransfer* pTransfer = static_cast<NetTransfer*>(ctx);
		pTransfer->enableWrite(connectKey);
	}
	void NetTransfer::readBack(ConnectKey connectKey, void *ctx){
		DEBUG_D("链路上有数据发送到端，正在接收数据。");

		NetTransfer* pTransfer = static_cast<NetTransfer*>(ctx);
		evbuffer* output = bufferevent_get_input(connectKey);

		memset(m_s_pRecvBuffer, 0, READ_BUFFER_SIZE);
		size_t recvSize = bufferevent_read(connectKey, m_s_pRecvBuffer, READ_BUFFER_SIZE);
		if(recvSize > 0){
			DEBUG_D("接收数据：" << string(m_s_pRecvBuffer, recvSize));
			pTransfer->recvMsg(connectKey, m_s_pRecvBuffer, recvSize);
		}
	}
	void NetTransfer::errorBack(ConnectKey connectKey, short events, void *ctx){
		DEBUG_E("连接异常");
		NetTransfer* pTransfer = static_cast<NetTransfer*>(ctx);
		bufferevent_free(connectKey);
		pTransfer->unregisterConnect(connectKey);
	}

	void NetTransfer::addSocket(int socket, eSocketRWOpt socketRWOpt){
		if(m_EventBase == NULL){
			DEBUG_E("Event base 未初始化或初始化失败。");
			return;
		}

		ConnectKey connectKey = bufferevent_socket_new(m_EventBase, socket, BEV_OPT_CLOSE_ON_FREE);
		if(connectKey == NULL){
			DEBUG_E("new bufferevent failed.");
			return;
		}

		DEBUG_D("设置回调函数。");
		bufferevent_setcb(connectKey, NetTransfer::readBack, NetTransfer::writeBack, NetTransfer::errorBack, this);
		DEBUG_D("设置链接为可读可写。");
		bufferevent_enable(connectKey, EV_READ|EV_WRITE);

		if(EV_READ & bufferevent_get_enabled(connectKey)){
			DEBUG_D("缓存写入已启用。");
		}
		if(EV_WRITE & bufferevent_get_enabled(connectKey)){
			DEBUG_D("缓存读入已启用。");
		}

		registerConnect(connectKey);
	}

	void NetTransfer::registerConnect(const ConnectKey &connectKey){
		m_MSend.lock();
		if(m_MsgCache.find(connectKey) == m_MsgCache.end()){
			m_MsgCache.insert(MsgCache::value_type(connectKey, MsgStruct()));
		}
		m_MSend.unlock();

		m_ConnectKeyList.push_back(connectKey);

		m_IncompleteMsg.insert(pair<ConnectKey, BinaryMemory>(connectKey, BinaryMemory()));
		
	}

	void NetTransfer::unregisterConnect(const ConnectKey &connectKey){
		m_MSend.lock();
		if(m_MsgCache.find(connectKey) != m_MsgCache.end()){
			m_MsgCache.erase(m_MsgCache.find(connectKey));
		}
		m_MSend.unlock();

		m_UnconnectSet.insert(connectKey);

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

	void NetClient::execute(){
		event_base_dispatch(m_EventBase);

		event_base_free(m_EventBase);
	}

	bool NetClient::run(const string &ip, const long &port){
		if(m_ConnectSocket >= 0){
			DEBUG_E("already connect socket is [" << m_ConnectSocket << "]");
		}

		struct sockaddr_in address;
		bzero(&address, sizeof(address));
		address.sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
		address.sin_port = htons(port);

		evutil_socket_t m_ConnectSocket = socket(PF_INET, SOCK_STREAM, 0);
		if(m_ConnectSocket < 0){
			DEBUG_E("未生成正确的socker");
			return false;
		}

		DEBUG_D("开始注册套接字。");

		if(connect(m_ConnectSocket, (struct sockaddr *) &address, sizeof(address)) != 0){
			DEBUG_E("connect failed");
		}

		addSocket(m_ConnectSocket);

		return start();
	}
	void NetClient::stop(){ 
		DEBUG_D("socket client is closing.");
		close(m_ConnectSocket); 
	}
}

namespace Network{

	NetServer::NetServer()
	{ ; }
	NetServer::~NetServer(){ 
	}

	void NetServer::execute(){
		event_base_dispatch(m_EventBase);

		evconnlistener_free(m_Listener);
		event_base_free(m_EventBase);
	}

	bool NetServer::run(const string &ip, const long &port){
		struct sockaddr_in address;
		bzero(&address, sizeof(address));
		address.sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
		address.sin_port = htons(port);

		if (!m_EventBase) {
			DEBUG_E("Could not initialize libevent!");
			return false;
		}

		m_Listener = evconnlistener_new_bind(m_EventBase, listener_cb, (void *)this, LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr*)&address, sizeof(address));

		if (!m_Listener) {
			DEBUG_E("Could not create a m_Listener!");
			return false;
		}
		return start();
	}

	void NetServer::listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *ctx){
		NetTransfer* pTransfer = static_cast<NetTransfer*>(ctx);
		pTransfer->addSocket(fd);
	}
}


