/**********************************************************
 *  \!file pub_define.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _pub_define_H
#define _pub_define_H

//! \note	string list map vector 常用的stl直接通过define包含。
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include "fr_public/pub_log.h"

using namespace std;
typedef string Name;
typedef string Path;

/// *******************************************************************************************************

/*
 * default and temporary value
 */
// type default error.
#define _STRINGFALSE								"string_false"
#define _INTFALSE									-1
#define _BOOLFALSE									false
#define _DOUBLEFALSE								-1.0
/// *******************************************************************************************************

/*
 * About time : format, default value.
 */
// sec to ms
#define _TIMEUNIT_BASENUM							1000
#define _TIMEUNIT_SECTOMS							_TIMEUNIT_BASENUM
#define _TIMEUNIT_SECTOUS							_TIMEUNIT_BASENUM*_TIMEUNIT_BASENUM
#define _TIMEUNIT_USTOSEC							(1/_TIMEUNIT_SECTOUS)
#define _TIMEUNIT_MSTOSEC							1/RPT_TIMEUNIT_BASENUM
// time conversion
#define _TIMECONVERSION_MSTOUS					1000
#define _TIMECONVERSION_SECTOMS					1000
#define _TIMECONVERSION_MINTOSEC					60
#define _TIMECONVERSION_HOURTOMIN					60
#define _TIMECONVERSION_HOURTOSEC					3600
#define _TIMECONVERSION_MONTHTODAY				28
#define _TIMECONVERSION_DATTOHOUR					24
#define _TIMECONVERSION_YEARTOMONTH				12	
#define _TIMECONVERSION_YEARTODAT					365
#define _TIMECONVERSION_DAYTOSEC					_TIMECONVERSION_DATTOHOUR * _TIMECONVERSION_HOURTOSEC
#define _TIMECONVERSION_MONTHTOSEC				_TIMECONVERSION_MONTHTODAY * _TIMECONVERSION_DAYTOSEC
/// *******************************************************************************************************

//
// marco function
//
#define _LOOP_MAP_DEBUG(preStr, mapName )			for(map<string,string>::const_iterator citer = mapName.begin(); citer != mapName.end(); ++citer ) \
														DEBUG_D("%s : [%s] => [%s]", string(preStr).pub_str(), citer->first.pub_str(), citer->second.pub_str() );
/// *******************************************************************************************************

#endif 

