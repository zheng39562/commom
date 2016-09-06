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
#include "event2/buffer.h"
#include "event2/bufferevent.h"
#include "tool/single_mode.hpp"

using namespace std;

namespace Network{
	NetMsgTransfer::NetMsgTransfer()
		:m_EventBase(NULL),
		 m_MsgList()
	{
		m_EventBase = event_base_new();
	}
	NetMsgTransfer::~NetMsgTransfer(){
		;
	}

	int NetMsgTransfer::addSocket(const int &socket, eSocketRWOpt socketRWOpt){
		bufferevent *bev = bufferevent_socket_new(m_EventBase, socket, BEV_OPT_CLOSE_ON_FREE);
		if(bev == NULL){
			cout << "new bufferevent failed." << endl;
		}

		//bufferevent_setcb(bev, NetMsgTransfer::readBack, NetMsgTransfer::writeBack, NULL, NULL);
		bufferevent_setcb(bev, NULL, NULL, NULL, NULL);
		bufferevent_enable(bev, EV_READ|EV_WRITE);
	}

	int NetMsgTransfer::removeSocket(const int &socket){
		return 0;
	}

	int NetMsgTransfer::pushMsg(const NetMsg &msg){
		bufferevent_write(msg.m_BufferEvent, msg.m_Data.c_str(), msg.m_Data.size() );
		return 0;
	}

	NetMsgPtr NetMsgTransfer::popMsg(){
		return NetMsgPtr();
	}

	void NetMsgTransfer::writeBack(bufferevent* bev, void *ctx){
		cout << " writeBack " << endl;
		evbuffer* output = bufferevent_get_output(bev);
		unsigned long len(0);
		char* retLine = evbuffer_readln(output, &len, EVBUFFER_EOL_CRLF);

		if(retLine == NULL){
			cout << "retLine is null " << endl;
			return;
		}

		cout << " writeBack : " << retLine << endl;
	}
	void NetMsgTransfer::readBack(bufferevent* bev, void *ctx){
		cout << " readBack " << endl;
		evbuffer* output = bufferevent_get_input(bev);
		unsigned long len(0);
		char* retLine = evbuffer_readln(output, &len, EVBUFFER_EOL_CRLF);

		if(retLine == NULL){
			cout << "retLine is null " << endl;
			return;
		}

		cout << " readBack : " << retLine << endl;


		NetMsgPtr pMsg(new NetMsg());
		pMsg->m_BufferEvent = bev;
		pMsg->m_Data = string(retLine);
		DesignMode::SingleMode<NetMsgTransfer>::getInstance()->m_MsgList.push_back(pMsg);
	}

	void NetMsgTransfer::run(){
		event_base_dispatch(m_EventBase);
	}
}

