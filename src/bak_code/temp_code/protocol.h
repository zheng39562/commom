/**********************************************************
 *  \file protocol.h
 *  \brief
 * 	\note	包的定义：包含包头和数据的完整数据流。
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _protocol_H
#define _protocol_H

// 前端使用。
#ifndef _CLIENT
#include "fr_public/pub_define.h"
#include "fr_public/pub_memory.h"
#else
#include "common/Common.h"
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include "pub_memory.h"
#endif

extern const unsigned int PROTO_HEAD_SIZE;	//! 包头长度
extern const unsigned int PROTO_HEAD_PACKET_INDEX;	//! 包长度起始地址。
extern const unsigned int PROTO_PACKET_SIZE_BYTE;	//! 包长度储存字节数 
extern const unsigned int PROTO_HEAD_VERIFY_INDEX;	//! 数据验证起始地址。
extern const unsigned int PROTO_VERIFY_SIZE_BYTE;	//! 数据验证储存字节数 

typedef short proto_size;
//! \brief	打包。
//! \param[in,out] unpacket 数据包，打包后直接返回。
//! \return true 成功， false失败。当返回false时，则未对数据包做任何操作。
bool proto_pack(Universal::BinaryMemory &unpacket);
//! \brief	解包
//! \note	解包时会对包做完整性校验。
//! \return true 成功， false失败。当返回false时，则未对数据包做任何操作。
bool proto_unpack(Universal::BinaryMemory &packet);
//! \brief	验证一个包的完整性。
//! \retval 0 完整包。1 包长度不正确，2包数据未通过校验。
int proto_verification(const Universal::BinaryMemory &binary);
//! \brief	从包头中获取包体大小。
proto_size proto_bypeToSize(Byte* pHead);
//! \brief	将包体大小的数据传入包头。
void proto_sizeToByte(proto_size size, Byte* pHead);

#endif 

