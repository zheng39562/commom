/**********************************************************
 *  \file tcp_interface.h
 *  \brief	tcp接口类
 *  \note	暂时只支持源码和静态库。动态库太麻烦了。。
 *  \note	提供server的c形式接口。
 *  \note	client的接口不提供，后续将提供文档指导，如有可能，也会尝试实现接口。
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _tcp_interface_H
#define _tcp_interface_H

#include "tcp_client.h"
#include "tcp_server.h"

#ifdef __cplusplus
extern "C"{
#endif

/*
//! \brief	监听端口
bool tcp_listen(const char* ip, uint32 port);
//! \brief	关闭服务端口
bool tcp_server_close();
*/

#ifdef __cplusplus
}
#endif

#endif 

