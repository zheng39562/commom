/**********************************************************
 *  \!file net_transfer.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "net_transfer.h"

#include <iostream>
#include <string.h>
#include "event2/buffer.h"
#include "event2/bufferevent.h"
#include "tool/single_mode.hpp"
#include "common_define.h"
#include "network/net_msg_struct.h"
#include "network/net_protocol_convert.h"

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

