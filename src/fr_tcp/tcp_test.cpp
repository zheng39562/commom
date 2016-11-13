/**********************************************************
 *  \file tcp_test.cpp
 *  \brief
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#include "tcp_test.h"

#include "tcp_server.h"
#include "tcp_client.h"
#include "fr_public/pub_memory.h"
#include "fr_public/pub_timer.h"

using namespace std;
using namespace Universal;

FrTcpServer* pServer;
FrTcpServer* pClient;

void cl_CB(Socket socket){
}

void cr_CB(Socket socket, const BinaryMemory &binary){
}

void sl_CB(Socket socket){
}

void sr_CB(Socket socket, const BinaryMemory &binary){
}

void doTcpTest(string arg0, string arg1, string arg2, string arg3, string arg4){
	DEBUG_D("测试结束.");
	DEBUG_D("等待输入任意字符后结束.");
	int i; cin >> i;
}




