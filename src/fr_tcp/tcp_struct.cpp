/**********************************************************
 *  \!file tcp_struct.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "tcp_struct.h"

using namespace Universal;

const Socket SOCKET_UN_INIT_VALUE = -1;

FrTcpCache::FrTcpCache()
	:socket(SOCKET_UN_INIT_VALUE),
	 connect(false),
	 writeActive(false),
	 bufferWrite(),
	 bufferRead(),
	 bufferTmp(),
	 mutexRead(),
	 mutexWrite()
{ ; }

