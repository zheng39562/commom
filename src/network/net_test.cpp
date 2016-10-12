/**********************************************************
 *  \!file net_test.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "net_test.h"

#include <string.h>
#include "net_interface.h"
#include "common/string_util.h"
#include "network/net_connection.h"
#include "network/net_packer.h"

using namespace Network;
using namespace Universal;

namespace NetworkTest{
	/*
bool net_connect(const std::string &ip, const long &port);
//! \note	需要注意未packer的指针类。不支持直接的packer类。
void net_send_C(const Network::PackerPtr &pPacker);
bool net_recv_C(Network::PackerPtr &pPacker);

//! \brief	
//! \note	使用：服务端使用listen链接。成功后，可以直接通过net_send_S/net_recv_S来收发包即可。
//! \todo	不支持多ip/port监听：如果需要多端口，可以使用net_server来完成。具体见文件。
//! \attetion	需要先listen后再进行发送和接受操作。否则行为未定义。
bool net_listen(const std::string &ip, const long &port);
//! \note	需要注意未packer的指针类。不支持直接的packer类。
void net_send_S(const Network::PackerPtr &pPacker);
bool net_recv_S(Network::PackerPtr &pPacker);
*/
string g_TestContext_1 = "1234567890987654321";
string g_TestContext_2 = "fqefqkfejqlkefjqlwekfjlqwekfjqlwkefjqlkfjlk";
string g_TestContext_3 = "daskfvaovuqovqir124190";

#define ARG_SERVER "server"
#define ARG_CLIENT "client"

#define TEST_IP "127.0.0.1"
#define TEST_LISTEN_PORT 12345
#define TEST_CONNECT_PORT TEST_LISTEN_PORT
	void doTest(const string &arg){
		SingleLogServer::getInstance()->InitLog("./log", "network_");

		string argTmp = strToLower(arg);
		DEBUG_D("run doTest : = " << argTmp);

		doLink(argTmp);
		//doMemCpy(argTmp);
		//doPerformance(argTmp);
	}

	void doLink(const string &arg){
		if(arg == ARG_CLIENT){
			bool bRet = net_connect(TEST_IP, TEST_CONNECT_PORT);
			if(!bRet){ DEBUG_E("client 连接失败。"); }

			while(bRet){
				DEBUG_D("链接成功，准备发送数据。");
				PackerPtr pPacker(new Packer(NULL));
				pPacker->setBuffer(g_TestContext_1.c_str(), g_TestContext_1.size());

				DEBUG_D("发送数据:" << g_TestContext_1);
				net_sendAll_C(pPacker);

				sleep(3);
			}
		}
		if(arg == ARG_SERVER){
			bool bRet = net_listen(TEST_IP, TEST_LISTEN_PORT);
			if(!bRet){ DEBUG_E("server 监听失败。"); }

			while(bRet){
				DEBUG_D("开始监听，等待链接和数据。");
				PackerPtr pPacker;
				if(net_recv_S(pPacker)){
					DEBUG_D("成功接收数据。");
					string ret((char*)pPacker->getBuffer(), pPacker->getBufferSize());
					if(g_TestContext_1 == ret){
						DEBUG_D("接收到消息" << ret );
						break;
					}
					else{
						DEBUG_D("接收到的消息和发送消息不一致. " << g_TestContext_1 << " != " << ret);
						break;
					}
				}

				sleep(1);
			}
		}
	}

	void doMemCpy(const string &arg){
		DataStruct dataStruct;
		dataStruct.i = 100;
		dataStruct.d = 100.1;
		dataStruct.c = 'A';
		DataClass dataClass;
		dataClass.i = 100;
		dataClass.d = 100.1;
		dataClass.c = 'A';

		if(arg == ARG_CLIENT){
			if(net_connect(TEST_IP, TEST_CONNECT_PORT));

			PackerPtr pPacker(new Packer(NULL));
			pPacker->setBuffer(&dataStruct, sizeof(dataStruct));
			net_sendAll_C(pPacker);
			pPacker->setBuffer(&dataClass, sizeof(dataClass));
			net_sendAll_C(pPacker);
		}
		if(arg == ARG_SERVER){
			net_listen(TEST_IP, TEST_LISTEN_PORT);
			int recvSize = 0;
			while(1){
				if(recvSize >= 2){
					break;
				}
				
				PackerPtr pPacker;
				if(net_recv_S(pPacker)){
					if(recvSize == 0){
						DataStruct* pDataStruct = new DataStruct();
						memcpy(pDataStruct, pPacker->getBuffer(), pPacker->getBufferSize());
						if(dataStruct.i == pDataStruct->i && dataStruct.d == pDataStruct->d && dataStruct.c == pDataStruct->c ){
							DEBUG_D("Struct 接收到消息");
						}
						else{
							DEBUG_D("接收到的消息和发送消息不一致. ");
						}
						delete pDataStruct; pDataStruct = NULL;
					}
					else{
						DataClass* pDataClass = new DataClass();
						memcpy(pDataClass, pPacker->getBuffer(), pPacker->getBufferSize());
						if(dataClass.i == pDataClass->i && dataClass.d == pDataClass->d && dataClass.c == pDataClass->c ){
							DEBUG_D("Class 接收到消息");
						}
						else{
							DEBUG_D("接收到的消息和发送消息不一致. ");
						}
						delete pDataClass; pDataClass = NULL;
					}
					++recvSize;
				}
			}
		}

	}


#define PERFROMANCE_TIMES  100000   //! 性能测试的次数。

	void doPerformance(const string &arg){
	}

}
