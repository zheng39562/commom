/**********************************************************
 *  \!file net_protocol_convert.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _net_protocol_convert_H
#define _net_protocol_convert_H

#include "network/net_packer.h"

//! \breif	本协议完全由本人(zheng39562@163.com)胡编乱造，不排除后期参考其他协议修正。
//! \note	协议头：协议头不可缺少。
//				0-3 包大小。4 扩展标志位。5 数据结构类型。
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

	//! \brief	数据结构类型。
	enum eProtocolDataFormat{
		eProtocolDataFormat_FirstInvalid = 0,
		eProtocolDataFormat_String,
		eProtocolDataFormat_File,
		eProtocolDataFormat_Json,
		eProtocolDataFormat_Xml,
		eProtocolDataFormat_,
		eProtocolDataFormat_LastInvalid = 255
	};

	//! \brief	将msg转换成packer
	//! \param[in,out] msg 消息字符串，已完成的消息会被打包。剩余消息通过该参数返回。
	void convertMsgToProtocolData(std::string &msg, MProtocolDataQueue &packerQueue);
	//! \brief	将packer转换成msg
	void convertProtocolDataToMsg(const ConstProtocolDataPtr &pProtocolData, MMsgQueue &msgQueue);
}
#endif 

