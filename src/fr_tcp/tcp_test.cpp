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


class FrTestServer : FrTcpServer{
	public:
		virtual eEventResult onReceive(Socket socket, Universal::BinaryMemoryPtr pBinary){
			DEBUG_D("接受到消息。");
			pBinary->print();
			return eEventResult_OK;
		}
		virtual eEventResult onConnect(Socket socket){
			DEBUG_D("接受到消息。");
			return eEventResult_OK;
		}
		virtual eEventResult onDisconnect(Socket socket){
			DEBUG_D("接受到消息。");
			return eEventResult_OK;
		}
}

void doTcpTest(string arg0, string arg1, string arg2, string arg3, string arg4){
	// 测试客户端和服务端消息的连通性。
	// 封数据包进程：64B 1K 63K 测试服务功能。 单进行50线程  开10个进程。

	DEBUG_D("测试结束.");
	DEBUG_D("等待输入任意字符后结束.");
	int i; cin >> i;
}




