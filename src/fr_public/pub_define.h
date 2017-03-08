/**********************************************************
 *  \!file pub_define.h
 *  \!brief

 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _pub_define_H
#define _pub_define_H

#include <string>
#include <queue>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <iostream>

typedef unsigned char Byte;
typedef std::string Name;
typedef std::string FileName;
typedef std::string Path;
typedef std::string FilePath;

typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;
typedef short sint16;
typedef int sint32;
typedef long long sint64;
typedef sint16 int16;
typedef sint32 int32;
typedef sint64 int64;

#include "fr_public/pub_log.h"

#define LOG_KEY_PUBLIC "log_public"
#define PUB_DEBUG_P(msg) K_DEBUG_P(LOG_KEY_PUBLIC, msg)
#define PUB_DEBUG_D(msg) K_DEBUG_D(LOG_KEY_PUBLIC, msg)
#define PUB_DEBUG_I(msg) K_DEBUG_I(LOG_KEY_PUBLIC, msg)
#define PUB_DEBUG_W(msg) K_DEBUG_W(LOG_KEY_PUBLIC, msg)
#define PUB_DEBUG_E(msg) K_DEBUG_E(LOG_KEY_PUBLIC, msg)
#define PUB_DEBUG_C(msg) K_DEBUG_C(LOG_KEY_PUBLIC, msg) 

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
#define _TIMECONVERSION_MSTOUS						1000
#define _TIMECONVERSION_SECTOMS						1000
#define _TIMECONVERSION_SECTOUS						1000000
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

#endif 

