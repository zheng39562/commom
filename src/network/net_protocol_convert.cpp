/**********************************************************
 *  \!file net_protocol_convert.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "net_protocol_convert.h"

#include "network/net_define.h"
#include "network/net_transfer.h"

namespace Network{
	ProtocolMsgPtr ProtocolMsg::getProtocolMsg(string &msg){
		ProtocolMsgPtr pProtocolMsg;

		ProtocolSize size = StrToSize(msg);
		if( size != NET_ERROR_NO && size <= msg.size()){
			pProtocolMsg = ProtocolMsgPtr(new ProtocolMsg());
			pProtocolMsg->size = size;
			pProtocolMsg->flags = msg[PROTOCOL_INDEX_FLAG];
			pProtocolMsg->dataFormat = (eProtocolDataFormat)msg[PROTOCOL_INDEX_DATA_FORMAT];
			pProtocolMsg->msg = msg.substr(PROTOCOL_INDEX_MSG, size - PROTOCOL_INDEX_MSG + 1);
			msg = msg.substr(PROTOCOL_INDEX_MSG + 1);
		}
		return pProtocolMsg;

	}
	string ProtocolMsg::convertMsgToString(const ProtocolMsgPtr pProtocolMsg){
		string msg("");
		if(!pProtocolMsg->msg.empty()){
			pProtocolMsg->size = PROTOCOL_HEAD_SIZE + pProtocolMsg->msg.size();

			msg += SizeToStr(pProtocolMsg->size);
			msg += (char)(pProtocolMsg->flags);
			msg += (char)(pProtocolMsg->dataFormat);
			msg += pProtocolMsg->msg;
		}
		return msg;
	}

	ProtocolSize ProtocolMsg::StrToSize(const string &msg){
		if(msg.size() >= PROTOCOL_MSG_SIZE_BYTE){
			ProtocolSize size(0);
			for(int i=0; i<PROTOCOL_MSG_SIZE_BYTE; ++i){
				size = size << 8;
				size += (ProtocolSize)msg[i];
			}
		}
		return NET_ERROR_NO;
	}
	string ProtocolMsg::SizeToStr(ProtocolSize size){
		string msg;
		for(int i=PROTOCOL_MSG_SIZE_BYTE-1; i>=0; --i){
			msg[i] = size & 0xFF;
			size = size >> 8;
		}
		return msg;
	}
}


namespace Network{

	void convertMsgToPacker(const ConnectKey &key, std::string &msg, MPackerPtrQueue &packerPtrQueue){
		ProtocolMsgPtr pProtocolMsg = ProtocolMsg::getProtocolMsg(msg);
		if(pProtocolMsg != NULL){
			PackerPtr pPacker(new Packer(key));
			pPacker->parseMsg(pProtocolMsg->msg, pProtocolMsg->dataFormat);

			ProtocolMsgPtr pProtocolMsg = ProtocolMsg::getProtocolMsg(msg);
		}
	}

	void convertPackerToMsg(const ConstPackerPtr &pPacker, MMsgPtrQueue &msgQueue){
		ProtocolMsgPtr pProtocolMsg(new ProtocolMsg());
		pProtocolMsg->flags = '\0';
		pProtocolMsg->dataFormat = pPacker->getDataFormat();
		pProtocolMsg->msg = pPacker->getPackerStr();
		string strMsg = ProtocolMsg::convertMsgToString(pProtocolMsg);
	
		while(!strMsg.empty()){
			msgQueue.push(MsgPtr(new Msg(pPacker->getConnectKey(), strMsg.substr(0, WRITE_BUFFER_SIZE))));
			strMsg.erase(0, WRITE_BUFFER_SIZE);
		}
	}
}
