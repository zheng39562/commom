/**********************************************************
 *  \!file net_struct.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _net_struct_H
#define _net_struct_H

#include <string>
#include <list>
#include <map>
#include "event2/bufferevent.h"
#include "tool/single_mode.hpp"


namespace Network{
	typedef bufferevent* ConnectKey;
	const ConnectKey NET_CACHEKEY_DEFAULT = NULL;

}
#endif 

