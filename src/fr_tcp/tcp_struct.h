/**********************************************************
 *  \!file tcp_struct.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _tcp_struct_H
#define _tcp_struct_H

#include "fr_public/pub_thread.h"
#include "fr_public/pub_memory.h"

#include "tcp_define.h"

extern const Socket SOCKET_UNKNOW_VALUE;

//! \brief	缓存类。
//! \note	包含缓存，缓存锁，连接状态，引用计数。
//! \note	考虑过封装为函数行为，因时间问题没有封。
class FrTcpCache{
	public:
		FrTcpCache();
		FrTcpCache(const FrTcpCache &ref)=delete;
		FrTcpCache operator=(const FrTcpCache &ref)=delete;
		~FrTcpCache()=default;
	public:
		Socket socket;
		bool writeActive; // 写是否已经激活。
		bool connect;
		Universal::FrMutex mutexWrite;
		Universal::FrMutex mutexRead;
		Universal::BinaryMemory bufferRead;
		Universal::BinaryMemory bufferTmp;
		Universal::BinaryMemory bufferWrite;
};
typedef boost::shared_ptr<FrTcpCache> FrTcpCachePtr;
typedef std::map<Socket, FrTcpCachePtr> TcpCacheTree;

enum eSocketEventType{
	eSocketEventType_Invalid,
	eSocketEventType_Recv,
	eSocketEventType_Send,
	eSocketEventType_Connect,
	eSocketEventType_Disconnect
};

#endif 

