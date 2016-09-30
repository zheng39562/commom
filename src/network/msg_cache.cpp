/**********************************************************
 *  \!file msg_cache.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "msg_cache.h"

#include "net_protocol_convert.h"

namespace Network{
	using namespace std;

	MsgServer::MsgServer()
		:m_MsgCache(),
		 m_PackerCache(),
		 m_IncompleteMsg()
	{
		;
	}
	MsgServer::~MsgServer(){ ; }

	bool MsgServer::emptyW(ConnectKey connectKey){
		if(m_MsgCache.find(connectKey) != m_MsgCache.end()){
			return m_MsgCache.find(connectKey)->second.empty();
		}
		return false;
	}

	void MsgServer::pushPacker(const PackerPtr &pPacker){
		ConnectKey connectKey = pPacker->getConnectKey();
		if(m_MsgCache.find(connectKey) == m_MsgCache.end()){
			m_MsgCache.insert(MsgCache::value_type(connectKey, MMsgPtrQueue()));
		}
		convertPackerToMsg(pPacker, m_MsgCache.find(connectKey)->second);
	}

	MsgPtr MsgServer::popMsg(ConnectKey connectKey){
		MsgPtr pMsg;
		if(m_MsgCache.find(connectKey) != m_MsgCache.end()){
			if(!m_MsgCache.find(connectKey)->second.empty()){
				pMsg = m_MsgCache.find(connectKey)->second.pop();
			}
		}
		return pMsg;
	}

	bool MsgServer::emptyR(){ return m_PackerCache.empty(); }

	void MsgServer::pushMsg(const MsgPtr &pMsg){
		ConnectKey connectKey = pMsg->m_ConnectKey;
		if(m_IncompleteMsg.find(connectKey) == m_IncompleteMsg.end()){
			m_IncompleteMsg.insert(pair<ConnectKey, std::string>(connectKey, std::string()));
		}
		m_IncompleteMsg.find(connectKey)->second += pMsg->m_Msg;

		convertMsgToPacker(connectKey, m_IncompleteMsg.find(connectKey)->second, m_PackerCache);
	}

	PackerPtr MsgServer::popPacker(){
		PackerPtr pPacker;
		if(!emptyR()){
			pPacker = m_PackerCache.pop();
		}
		return pPacker;
	}
}

