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

class FrTestClient : public FrTcpClient{
	public:
		virtual eEventResult onConnect(Socket socket){
			DEBUG_D("链接成功。");
			return eEventResult_OK;
		}
	private:
		static uint32 count;
};
uint32 FrTestClient::count = 0;

class FrTestServer : public FrTcpServer{
	public:
		virtual eEventResult onReceive(Socket socket, Universal::BinaryMemoryPtr pBinary){
			count += pBinary->getBufferSize();
			DEBUG_D("接受到消息。当次消息尺寸[" << pBinary->getBufferSize() << "]已收到消息[" << count << "]");
			return eEventResult_OK;
		}
		virtual eEventResult onConnect(Socket socket){
			DEBUG_D("接受到链接。");
			return eEventResult_OK;
		}
		virtual eEventResult onDisconnect(Socket socket){
			DEBUG_D("链接[" << socket << "]断开。");
			return eEventResult_OK;
		}
	private:
		static uint32 count;
};
uint32 FrTestServer::count = 0;

void doTcpTest(string arg0, string arg1, string arg2, string arg3, string arg4){
	// 测试客户端和服务端消息的连通性。
	// 封数据包进程：64B 1K 63K 测试服务功能。 单进行50线程  开10个进程。
	FrTestServer* pTcpServer = new FrTestServer();
	uint32 port = 5555;
	if(pTcpServer->run("192.168.2.104", port)){
		DEBUG_D("开始监听端口" << port);
		while(1){
			frSleep(1000);
		}
	}

	DEBUG_D("测试结束.");
	DEBUG_D("等待输入任意字符后结束.");
	int i; cin >> i;
}

