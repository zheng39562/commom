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
#include "network/net_msg_struct.h"
#include "network/net_protocol_struct.h"
#include "network/net_packer.h"

//! \brief	协议内容详见net_protocol_struct.h
namespace Network{
	//! \brief	协议结构。
	//! \todo	
	class ProtocolMsg{
		public:
			ProtocolMsg()
				:size(0),
				 flags(0),
				 dataFormat(eProtocolDataFormat_String),
				 msg()
			{;}
			~ProtocolMsg(){;}
		public:
			static ProtocolMsgPtr getProtocolMsg(string &msg);
			static string convertMsgToString(const ProtocolMsgPtr pProtocolMsg);
		private:
			static ProtocolSize StrToSize(const string &msg);
			static string SizeToStr(ProtocolSize size);
		public:
			ProtocolSize size;
			char flags;
			eProtocolDataFormat dataFormat;
			string msg;
	};
	//! \brief	将msg转换成packer
	//! \param[in,out] msg 消息字符串，已完成的消息会被打包。剩余消息通过该参数返回。
	//! \todo	MD5验证
	void convertMsgToPacker(const ConnectKey &key, std::string &msg, MPackerPtrQueue &packerPtrQueue);
	//! \brief	将packer转换成msg
	//! \todo	MD5验证
	void convertPackerToMsg(const ConstPackerPtr &pPacker, MMsgPtrQueue &msgQueue);

}
#endif 

