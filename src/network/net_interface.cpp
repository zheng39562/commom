/**********************************************************
 *  \!file net_interface.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "net_interface.h"

#include "network/net_connection.h"

using namespace Network;

bool net_connect(const string &ip, const long &port){
	return SingleNetClient::getInstance()->run(ip, port);
}

void net_send_C(const PackerPtr &pPacker){
	SingleNetClient::getInstance()->sendPacker(pPacker);
}

void net_sendAll_C(const Network::PackerPtr &pPacker){
	SingleNetClient::getInstance()->sendAll(pPacker);
}

bool net_recv_C(PackerPtr &pPacker){
	pPacker = SingleNetClient::getInstance()->recvPacker();
	return pPacker != NULL;
}

bool net_listen(const string &ip, const long &port){
	return SingleNetServer::getInstance()->run(ip, port);
}

void net_send_S(const PackerPtr &pPacker){
	SingleNetClient::getInstance()->sendPacker(pPacker);
}

void net_sendAll_S(const Network::PackerPtr &pPacker){
	SingleNetClient::getInstance()->sendAll(pPacker);
}

bool net_recv_S(PackerPtr &pPacker){
	pPacker = SingleNetClient::getInstance()->recvPacker();
	return pPacker != NULL;
}

