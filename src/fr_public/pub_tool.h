/**********************************************************
 * \file logic/tool/CommonTool.h
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _CommonTool_H
#define _CommonTool_H

#include <stdio.h>
#include <algorithm>

#include "fr_public/pub_define.h"
#include "fr_public/pub_file.h"
#include "fr_public/pub_string.h"
#include "fr_public/pub_timer.h"

/*
 * other function
 */
namespace Universal{
#define _PTHONETYPE_MOBILE					"YD"
#define _PTHONETYPE_UNICOM					"LT"
#define _PTHONETYPE_CDMA					"DX"
#define _PTHONETYPE_QT						"QT"
#ifdef LINUX
	//! \brief	执行shell指令/脚本
	bool execShellCmd( const std::string &cmd );
#endif
	//! \brief	获取手机的运营商。
	std::string getMobileType( const std::string &mobile );

	//! \brief	从map中查询key对应的数据。如果查询不到则返回 宏[_STRINGFALSE] 定义的字符串。
	std::string findDataFromMap( const std::map<std::string, std::string> &mapData, const std::string &key );

	//! \brief	生成MD5
	//! \note	默认为32位的.如果需要使用16位。可以调用md5_16
	std::string md5(const std::string str);
	//! \brief	生成MD5(16位)
	std::string md5_16(const std::string str);

	bool rc4_encrypt(char *content, int32 contentLength, const char* sKey, int32 ketLength);
	bool rc4_encrypt(std::string &content, const std::string &sKey);
	bool rc4_encrypt(Byte* content, uint32 size, const std::string &sKey);

	//! \brief	字符串版本的rc4(加密)
	//! \note	先用rc4转换二进制，然后将二进制转为十六进制的字符串
	bool myStrEncrypt(std::string &content, const std::string &key);
	//! \brief	字符串版本的rc4(加密)
	//! \note	先用rc4转换二进制，然后将二进制转为十六进制的字符串
	bool myStrDecrypt(std::string &content, const std::string &key);
	bool convertBinaryToHexString(std::string &content);
	bool convertHexStringToBinary(std::string &content);
}  // namespace : Universal


//
// 字符验证。。。
//
namespace Universal{
	//! \brief	验证字符串是否是一个数字。
	//!	\note	可验证：正负，整数，小数。
	bool checkNumber(const std::string &number);
	//! \brief	验证是否是一个正确的格式。
	//! \note	使用正则表达式进行解析，暂时不支持平年和闰年的差别。
	//! \todo	平年和闰年的识别：正则修正，或添加逻辑代码。
	bool checkDate(const std::string &date);
	//! \brief	验证手机格式。
	bool checkMobile(const std::string &mobile);

}  // namespace : Universal

namespace Universal{
	//! \brief	线程休眠，单位ms
	void frSleep(unsigned long time);
#ifdef LINUX
	//! \brief	线程休眠，单位us
	//! \note	仅在linux下使用。
	void frUSleep(unsigned long time);
#endif
}

namespace Universal{
	//! \brief	获取本机IP： 支持IPV4 支持window and linux 
	//! \note	window版本需要定义宏 WIN32
	void getIp(std::string &ip);
}

#endif


