/**********************************************************
 *  \!file net_struct.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "net_struct.h"

namespace Network{
	const ConnectKey NET_CACHEKEY_DEFAULT = NULL;
}

namespace Network{
	const char PROTOCOL_EXPAND_FILTER_OTHER = 0xF0;
	const char PROTOCOL_EXPAND_FILTER_FORMAT = 0x0F;

	const char PROTOCOL_EXPAND_DATA_VERIFY = 0x10;
	const char PROTOCOL_EXPAND_ = 0x0;

	const int PROTOCOL_MSG_SIZE_BYTE = 2;
	const int PROTOCOL_HEAD_SIZE = 3;

	const int PROTOCOL_INDEX_DATA_SIZE = 0;
	const int PROTOCOL_INDEX_EXPAND = PROTOCOL_MSG_SIZE_BYTE;
	const int PROTOCOL_INDEX_MSG = PROTOCOL_INDEX_EXPAND + 1;
}

