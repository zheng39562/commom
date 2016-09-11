/**********************************************************
 *  \!file net_msg_protocol.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _net_msg_protocol_H
#define _net_msg_protocol_H

// interface
namespace Network{
}

//! \breif	本协议完全由本人(zheng39562@163.com)胡编乱造，不排除后期参考其他协议修正。
//! \note	协议头：协议头不可缺少。
//				0-3 包大小。4 扩展标志位。5 数据类型。
//! \note	协议尾：尾部数据全为可选，由协议头确定是否存在
//				倒数32字节 MD5校验值
//! \note	扩展标志位：0-7位，0表示最低位
//				0 是否校验 
//				1 2 3 4 5 6 7 
//! \note	
//!	\note	理论上可以传输任意尺寸的数据，但数据暂时全部写入内存中，故过大的文件可能导致内存不足。
namespace Network{
	//! \brief	扩展标志位
	const char PROTOCOL_EXPAND_DATA_VERIFY = 0x1;
	const char PROTOCOL_EXPAND_ = 0x0;

	//! \brief	数据类型。
	enum eProtocolDataType{
		eProtocolDataType_FirstInvalid = 0,
		eProtocolDataType_String,
		eProtocolDataType_File,
		eProtocolDataType_Json,
		eProtocolDataType_Xml,
		eProtocolDataType_,
		eProtocolDataType_LastInvalid = 127
	};

}
#endif 

