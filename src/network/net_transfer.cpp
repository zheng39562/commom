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

using namespace std;

namespace Network{
	const int READ_BUFFER_SIZE = 4000;
	const int WRITE_BUFFER_SIZE = 4000;

	NetMsgTransfer::NetMsgTransfer()
		:m_EventBase(NULL)
	{
		m_EventBase = event_base_new();
	}
	NetMsgTransfer::~NetMsgTransfer(){
		;
	}

	int NetMsgTransfer::addSocket(const int &socket, eSocketRWOpt socketRWOpt){
		bufferevent *bev = bufferevent_socket_new(m_EventBase, socket, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
		if(bev == NULL){
			DEBUG_D("new bufferevent failed.");
		}

		bufferevent_setcb(bev, NetMsgTransfer::readBack, NetMsgTransfer::writeBack, NetMsgTransfer::errorBack, NULL);
		bufferevent_enable(bev, EV_READ|EV_WRITE);

	}

	int NetMsgTransfer::removeSocket(const int &socket){
		return 0;
	}

	void NetMsgTransfer::writeBack(bufferevent* bev, void *ctx){
		DEBUG_D(" write ");
		MsgPtr pMsg = SingleMsgServer::getInstance()->popMsg(bev);
		DEBUG_D(" write " << pMsg->m_Msg);
		bufferevent_write(bev, pMsg->m_Msg.c_str(), pMsg->m_Msg.c_str());
	}
	void NetMsgTransfer::readBack(bufferevent* bev, void *ctx){
		DEBUG_D(" readBack ");
		evbuffer* output = bufferevent_get_input(bev);
		char* retLine = new char(READ_BUFFER_SIZE);
		memset(retLine, '\0', sizeof(char)*READ_BUFFER_SIZE);
		if( bufferevent_read(bev, retLine, READ_BUFFER_SIZE) > 0 ){
			SingleMsgServer::getInstance()->pushMsg(MsgPtr(new Msg(bev, retLine)));
		}
		delete retLine;
	}
	void NetMsgTransfer::errorBack(bufferevent* bev, short events, void *ctx){
		DEBUG_D(" error back ");
		bufferevent_free(bev);
		DEBUG_D(" bev addr : " << bev);
	}

	void NetMsgTransfer::run(){
		event_base_dispatch(m_EventBase);
	}
}

