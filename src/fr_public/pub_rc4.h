/**********************************************************
 *  \file pub_rc4.h
 *  \brief	RC4加密算法
 *  \note	用法: 
 *				1.定义一个RC4_KEY结构
 *				2.用函数RC4Init初始化状态
 *				3.用RC4Works加密
 *	\note	使用说明：
 *				1.定义密钥变量RC4_KEY rc4key;
 *				2.初始化密钥  RC4Init(255字节以内的口令字符串,口令长度,&rc4key);
 *				3.加/解密     RC4Works(无符号字符型数据,数据长度,&rc4key);
 *	\note	网上复制的源码，原作者已不可考。测试可用。
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _pub_rc4_H
#define _pub_rc4_H

#include "pub_define.h"

#ifdef __cplusplus
extern "C" {
#endif

struct RC4_KEY{
	Byte bySTab[256];   //256字节的S表
	Byte byIt, byJt;     //t时刻的两个指针
};

bool RC4Init(const char*, uint32, RC4_KEY *);
bool RC4Works (Byte* content, uint32 size, RC4_KEY *);

#ifdef __cplusplus
}
#endif


#endif 

