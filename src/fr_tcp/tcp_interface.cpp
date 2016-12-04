/**********************************************************
 *  \!file tcp_interface.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "tcp_interface.h"

using namespace std;

/*
bool tcp_listen(const char* ip, uint32 port){
	SingleFrTcpServer::getInstance()->listen(string(ip), port);
	return true;
}

bool tcp_server_close(){
	SingleFrTcpServer::getInstance()->stop();
	return true;
}

bool tcp_server_cb(fp_connect_cb connect_cb, fp_disconnect_cb disconn_cb, fp_send_cb send_cb, fp_receive_cb receive_cb, void* etc){
	SingleFrTcpServer::getInstance()->setCallBack(connect_cb, disconn_cb, send_cb, receive_cb, etc);
	return true;
}

*/

