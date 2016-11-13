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

#include "network/net_define.h"
#include "boost/shared_ptr.hpp"
#include "event2/bufferevent.h"
#include "template/single_mode.hpp"

//! \brief	未分类的通用结构。
namespace Network{
	typedef bufferevent* ConnectKey;
	extern const ConnectKey NET_CACHEKEY_DEFAULT;
}


//! \breif	本协议完全由本人(zheng39562@163.com)胡编乱造，不排除后期参考其他协议修正。
//! \note	协议主要应用领域是非常频繁的小数据包。
//				* 每秒数据包1W-100W。
//				* 包大小通常在100以内
//! \note	协议头：协议头不可缺少。
//				0-1 包大小。
//					* 考虑过只使用1位保存包尺寸。但考虑254的大小（减去包和扩展位）的应用局限性过大，只能增加一位。
//					* 极端情况下（例如发包10W，每个包只有10B。）优化包大小会增加效率。在考虑对系统整体的影响下，可以对协议进行调整。
//				2 扩展标志位
//					* 低四位 包的数据格式
//					* 高四位 其他
//! \note	协议尾：尾部数据全为可选，由协议头确定是否存在
//				倒数32字节 MD5校验值
//! \note	扩展标志位：0-7位，0表示最低位
//				0 是否校验 
//				1 2 3 4 5 6 7 
//! \note	
//! \todo	协议头尺寸可选功能。
namespace Network{
	//! \brief	扩展标志位
	extern const char PROTOCOL_EXPAND_FILTER_OTHER;
	extern const char PROTOCOL_EXPAND_FILTER_FORMAT;
	extern const char PROTOCOL_EXPAND_DATA_VERIFY;
	extern const char PROTOCOL_EXPAND_;

	//! \brief	数据格式类型。
	//! \note	最多支持16种类的数据格式。
	//! \note	协议层只能增加通用性质的格式。
	enum eProtocolDataFormat{
		eProtocolDataFormat_FirstInvalid = -1,
		eProtocolDataFormat_Memory = 0,
		eProtocolDataFormat_File,
		eProtocolDataFormat_Json,
		eProtocolDataFormat_Xml,
		eProtocolDataFormat_,
		eProtocolDataFormat_LastInvalid = 17
	};
	extern const char PROTOCOL_EXPAND_FILTER_FORMAT;

	class ProtocolMsg;
	typedef boost::shared_ptr<ProtocolMsg> ProtocolMsgPtr;

	extern const int PROTOCOL_MSG_SIZE_BYTE;
	extern const int PROTOCOL_HEAD_SIZE;

	extern const int PROTOCOL_INDEX_DATA_SIZE;
	extern const int PROTOCOL_INDEX_EXPAND;
	extern const int PROTOCOL_INDEX_MSG;
}

#endif 

