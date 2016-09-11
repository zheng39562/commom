/**********************************************************
 * \file logic/RptDefine.h
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _Define_H
#define _Define_H

#include <string>
#include <map>
#include <vector>
#include <list>
#include <time.h>

using namespace std;

/*
 * Debug information and operator.
 */
#ifndef __FUNC__
	#define __FUNC__ __FUNCTION__
	#define _RUN_FUNC_ __FUNC__
#else
	#define _RUN_FUNC_
#endif

#define DEBUG_D(msg, ...) 							cout << msg << endl;
#define DEBUG_I(msg, ...) 							cout << msg << endl;
#define DEBUG_W(msg, ...) 							cout << msg << endl;
#define DEBUG_E(msg, ...) 							cout << msg << endl;
#define DEBUG_C(msg, ...) 							cout << msg << endl;
#define	LOG_LVL_DBG									0
#define LOG_LVL_INFO								1
#define LOG_LVL_WARN								2
#define LOG_LVL_ERROR								3
#define LOG_LVL_CRITICAL							4

#ifdef IMPORTFILE_TEST
														//StartCount();
	#define	_START_FUNC(func)							LOG_D_CF();  \
														DEBUG_D( "%s: Start...", func );
	#define	_STOP_FUNC(func)							DEBUG_D( "%s: End...",  func );
														//stopCount(func);
#else
	#define	_START_FUNC(func)							LOG_D_CF();
	#define	_STOP_FUNC(func)
#endif

/// *******************************************************************************************************

/*
 * default and temporary value
 */
// temporary 
#define _TMP_INT									-100
#define _TMP_DOUBLE								-100.0
// type default error.
#define _STRINGFALSE								"string_false"
#define _INTFALSE									-1
#define _BOOLFALSE								false
#define _DOUBLEFALSE								-1.0
/// *******************************************************************************************************

/*
 * About time : format, default value.
 */
// datetime formate
#define _TIMEFORMAT_DEFAULT						string("Y-m-d H:i:s")
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


/*
 * sql : database name, table name, table item(code)
 */

// sql command
#define _SQLFIELD_COMMON_COUNTNAME				"countnumberA"
#define _SQLCMD_SELECT(field, table)				string("select ") + field + " from " + table + " where 1=1 "
#define _SQLCMD_SELECT_COUNT(table)				string("select count(1) as ") + _SQLFIELD_COMMON_COUNTNAME + " from " + table + " where 1=1 "
#define _SQLCMD_INSERT(table, fields, values )	string("insert into ") + table + " (" + fields + ") value (" + values + ")"
#define _SQLCMD_INSERT_DELAY(table, fields, values )	string("insert delayed into ") + table + " (" + fields + ") value (" + values + ")"
#define _SQLCMD_UPDATE(table, setValue )			string("update ") + table + " set " + setValue + " where 1=1 "
#define _SQLCMD_DELETE(table)						string("delete from ") + table + " where 1=1 "
// sql where 
#define _SERVICEDB_SQLWHERE_LIMIT					"limit"
#define _SERVICEDB_SQLWHERE_UNION					"union"

// split key
#define _SPLITKEY_COMMA							","
#define _SPLITKEY_POINT							"."

#define _COMMON_FUNC_(msg)						string(string("[") + msg + "::" + _RUN_FUNC_ + "]" ).c_str()

/// *******************************************************************************************************


//
// marco function
//
#define _LOOP_MAP_DEBUG(preStr, mapName )			for(map<string,string>::const_iterator citer = mapName.begin(); citer != mapName.end(); ++citer ) \
														DEBUG_D("%s : [%s] => [%s]", string(preStr).c_str(), citer->first.c_str(), citer->second.c_str() );
/// *******************************************************************************************************

#endif


