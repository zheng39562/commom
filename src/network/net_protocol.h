/**********************************************************
 *  \!file net_protocol.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _net_protocol_H
#define _net_protocol_H

#include "common/common_memory.h"
#include "network/net_define.h"
#include "network/net_struct.h"
#include "network/net_packer.h"

//! \brief	协议内容详见net_protocol_struct.h
namespace Network{
	//! \brief	将msg转换成packer
	//! \param[in,out] msg 消息字符串，已完成的消息会被打包。剩余消息通过该参数返回。
	//! \todo	MD5验证
	void convertMsgToPacker(const ConnectKey &key, BinaryMemory &cache, MPackerPtrQueue &packerPtrQueue);
	//! \brief	将packer转换成msg
	//! \todo	MD5验证
	void convertPackerToMsg(const ConstPackerPtr &pPacker, std::string &cache);
}
#endif 

