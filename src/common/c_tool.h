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
#include <sys/time.h>
#include <algorithm>

#include "c_define.h"
#include "c_file.h"
#include "c_string.h"
#include "c_timer.h"

/*
 * other function
 */
namespace Universal{
#define _PTHONETYPE_MOBILE					"YD"
#define _PTHONETYPE_UNICOM					"LT"
#define _PTHONETYPE_CDMA						"DX"
#define _PTHONETYPE_QT						"QT"
	//! \brief	执行shell指令/脚本
	bool execShellCmd( const string &cmd );
	//! \brief	获取手机的运营商。
	string getMobileType( const string &mobile );

	//! \brief	从map中查询key对应的数据。如果查询不到则返回 宏[_STRINGFALSE] 定义的字符串。
	string findDataFromMap( const map<string, string> &mapData, const string &key );
}  // namespace : Universal


//
// 字符验证。。。
//
namespace Universal{
	//! \brief	验证字符串是否是一个数字。
	//!	\note	可验证：正负，整数，小数。
	bool checkNumber( const string &number );
	//! \brief	验证是否是一个正确的格式。
	//! \note	使用正则表达式进行解析，暂时不支持平年和闰年的差别。
	//! \todo	平年和闰年的识别：正则修正，或添加逻辑代码。
	bool checkDate( const string &date );
	//! \brief	验证手机格式。
	bool checkMobile( const string &mobile );

}  // namespace : Universal




#endif


