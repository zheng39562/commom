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
#include "pub_memory.h"
#include "pub_timer.h"

using namespace std;
using namespace Universal;

FrTcpServer* pServer;
FrTcpServer* pClient;

void cl_CB(CONNID dwConnID){
	DEBUG_D("客户端成功连接到服务端. connid is " << dwConnID);
}

void cr_CB(CONNID dwConnID, const BinaryMemory &binary){
	DEBUG_D("客户端从[" << dwConnID << "]中接收数据.");
	DEBUG_D("(client)开始打印数据.");
	binary.print();
}

void sl_CB(CONNID dwConnID){
	DEBUG_D("服务端成功连接到客户端. connid is " << dwConnID);
}

void sr_CB(CONNID dwConnID, const BinaryMemory &binary){
	DEBUG_D("服务端从[" << dwConnID << "]中接收数据.");
	DEBUG_D("(server)开始打印数据.");
	binary.print();

	DEBUG_D("服务端回发数据.");
	pServer->send(dwConnID, binary);
}

void doTcpTest(string arg0, string arg1, string arg2, string arg3, string arg4){
	DEBUG_D("测试开始.");
	
	string ip("0.0.0.0");
	//string ip("192.168.0.212");
	//unsigned int port(5000);
	unsigned int port(5555);

	pServer = new FrTcpServer();
	pServer->start(ip, port);

	/*
	pClient = new FCTcpClient();
	pClient->setLinkCB(cl_CB);
	pClient->setReceiveCB(cr_CB);
	pClient->start(ip, port);

	string str("123456789abc");
	BinaryMemory binary((void*)str.c_str(), str.size());
	binary.print();
	pClient->send(binary);
	*/
	
	frSleep(200000000);
	//pClient->stop();
	pServer->stop();

	DEBUG_D("测试结束.");
	DEBUG_D("等待输入任意字符后结束.");

	delete pServer; pServer = NULL;
	//delete pClient; pClient = NULL;

	int i;
	cin >> i;

}




