/**********************************************************
 *  \!file pub_define.h
 *  \!brief
 *  \!note	×¢ÒâÊÂÏî£º 
 * 			1,ÀàÖÐµÄ³ÉÔ±º¯ÊýÖÐµÄÍ¬Ãû²ÎÊýµÄº¬ÒåÍêÈ«ÏàÍ¬¡£½ö»á×¢ÊÍÆäÖÐÒ»¸öº¯Êý£¬ÆäËûº¯ÊýÔò²»ÔÙÖØ¸´×¢ÊÍ¡£ÖØÃûµÄ²ÎÊýÒâÒå²»Í¬Ê±£¬»á¶ÀÁ¢×¢½â¡£ 
 * 			2,µÚ1ÌõµÄ¹æÔòÍ¬ÑùÊÊÓÃÓÚ·µ»ØÖµµÄº¬Òå¡£ 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _pub_define_H
#define _pub_define_H

//! \note	string list map vector ³£ÓÃµÄstlÖ±½ÓÍ¨¹ýdefine°üº¬¡£
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
//#include "fr_public/pub_log.h"

using namespace std;

typedef unsigned char Byte;
typedef string Name;
typedef string Path;


/*
#define DEBUG_D(msg)  cout << msg;
#define DEBUG_I(msg)  cout << msg;
#define DEBUG_W(msg)  cout << msg;
#define DEBUG_C(msg)  cout << msg;
#define DEBUG_E(msg)  cout << msg;
*/
#define DEBUG_D(msg)  cout << msg << endl;
#define DEBUG_I(msg)  cout << msg << endl;
#define DEBUG_W(msg)  cout << msg << endl;
#define DEBUG_C(msg)  cout << msg << endl;
#define DEBUG_E(msg)  cout << msg << endl;

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

