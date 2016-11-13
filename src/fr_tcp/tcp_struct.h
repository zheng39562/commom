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

extern const Socket SOCKET_UN_INIT_VALUE;

//! \brief	给内存带上锁。
class LockCache{
	public:
		LockCache();
		LockCache(const LockCache &ref);
		~LockCache()=default;
	public:
		Universal::FrMutex mutexRead;
		Universal::FrMutex mutexWrite;
		Universal::BinaryMemory bufferRead;
		Universal::BinaryMemory bufferWrite;
};

typedef std::map<Socket, LockCache> TcpCache;

enum eSocketEventType{
	eSocketEventType_Invalid,
	eSocketEventType_Recv,
	eSocketEventType_Send,
	eSocketEventType_Connect,
	eSocketEventType_Disconnect
};

#endif 

