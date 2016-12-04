/**********************************************************
 *  \file tcp_test.h
 *  \brief
 *  \note	TCP目录下测试总集：用于单元测试，回归测试。
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _tcp_test_H
#define _tcp_test_H

#include <string>
#include "tcp_define.h"
#include "tcp_interface.h"
#include "tcp_link.h"
#include "tcp_msg_process.h"
#include "tcp_server.h"
#include "tcp_server_thread.h"
#include "tcp_struct.h"

void doTcpTest(std::string arg0 = "", std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "");

#endif 

