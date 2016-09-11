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


namespace Network{
	Msg::Msg(bufferevent *_bev)
		:m_Bev(_bev)
	{
		;
	}
	Msg::~Msg(){
		;
	}
	bool Msg::doPack(const std::string &msg, ePackerError error_no){
		DEBUG_D("add " << msg);
	}
};

namespace Network{
	MsgCache::MsgCache()
		:m_MsgQueue(),
		 m_ReadCache(),
		 m_WriteCache()
	{
		;
	}
	MsgCache::~MsgCache(){
		;
	}

	int MsgCache::sendPack(PackerConstPtr pPacker){
		;
	}
	void MsgCache::recvMsg(MsgPtr msg){
		;
	}
	void MsgCache::activeWrite(bufferevent *bev){
		;
	}
	MsgPtr MsgCache::popPacker(){
		;
	}
};
