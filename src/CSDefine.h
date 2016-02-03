/**********************************************************
 * \file logic/CSRptDefine.h
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _CSRptDefine_H
#define _CSRptDefine_H

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

// DEBUG
#ifndef __COMMON
	#include "CSDebugLog.h"
	#define DEBUG_D(msg, ...) 							CSLOG_D(msg, ##__VA_ARGS__)
	#define DEBUG_I(msg, ...) 							CSLOG_I(msg, ##__VA_ARGS__)
	#define DEBUG_W(msg, ...) 							CSLOG_W(msg, ##__VA_ARGS__)
	#define DEBUG_E(msg, ...) 							CSLOG_E(msg, ##__VA_ARGS__)
	#define DEBUG_C(msg, ...) 							CSLOG_C(msg, ##__VA_ARGS__)
#else
    // 日志部分。
	#define CS_DEBUG(msg, ...) 							

	#define DEBUG_D(msg, ...) 							cout << msg << endl;
	#define DEBUG_I(msg, ...) 							cout << msg << endl;
	#define DEBUG_W(msg, ...) 							cout << msg << endl;
	#define DEBUG_E(msg, ...) 							cout << msg << endl;
	#define DEBUG_C(msg, ...) 							cout << msg << endl;
	#define	CSLOG_LVL_DBG								0
	#define CSLOG_LVL_INFO								1
	#define CSLOG_LVL_WARN								2
	#define CSLOG_LVL_ERROR								3
	#define CSLOG_LVL_CRITICAL							4
#endif
#ifdef DEBUG
	#define PRIINFO(msg, ...)							CSLOG_D(msg, ##__VA_ARGS__)
#endif

#ifdef IMPORTFILE_TEST
														//StartCount();
	#define	_START_FUNC(func)							CSLOG_D_CF();  \
														DEBUG_D( "%s: Start...", func );
	#define	_STOP_FUNC(func)							DEBUG_D( "%s: End...",  func );
														//stopCount(func);
#else
	#define	_START_FUNC(func)							CSLOG_D_CF();
	#define	_STOP_FUNC(func)
#endif

/// *******************************************************************************************************

/*
 * default and temporary value
 */
// temporary 
#define CS_TMP_INT									-100
#define CS_TMP_DOUBLE								-100.0
// type default error.
#define CS_STRINGFALSE								"string_false"
#define CS_INTFALSE									-1
#define CS_BOOLFALSE								false
#define CS_DOUBLEFALSE								-1.0
/// *******************************************************************************************************

/*
 * About time : format, default value.
 */
// datetime formate
#define CS_TIMEFORMAT_DEFAULT						string("Y-m-d H:i:s")
// sec to ms
#define CS_TIMEUNIT_BASENUM							1000
#define CS_TIMEUNIT_SECTOMS							CS_TIMEUNIT_BASENUM
#define CS_TIMEUNIT_SECTOUS							CS_TIMEUNIT_BASENUM*CS_TIMEUNIT_BASENUM
#define CS_TIMEUNIT_USTOSEC							(1/CS_TIMEUNIT_SECTOUS)
#define CS_TIMEUNIT_MSTOSEC							1/RPT_TIMEUNIT_BASENUM
// time conversion
#define CS_TIMECONVERSION_MSTOUS					1000
#define CS_TIMECONVERSION_SECTOMS					1000
#define CS_TIMECONVERSION_MINTOSEC					60
#define CS_TIMECONVERSION_HOURTOMIN					60
#define CS_TIMECONVERSION_HOURTOSEC					3600
#define CS_TIMECONVERSION_MONTHTODAY				28
#define CS_TIMECONVERSION_DATTOHOUR					24
#define CS_TIMECONVERSION_YEARTOMONTH				12	
#define CS_TIMECONVERSION_YEARTODAT					365
#define CS_TIMECONVERSION_DAYTOSEC					CS_TIMECONVERSION_DATTOHOUR * CS_TIMECONVERSION_HOURTOSEC
#define CS_TIMECONVERSION_MONTHTOSEC				CS_TIMECONVERSION_MONTHTODAY * CS_TIMECONVERSION_DAYTOSEC
/// *******************************************************************************************************


/*
 * sql : database name, table name, table item(code)
 */

// sql command
#define CS_SQLFIELD_COMMON_COUNTNAME				"countnumberA"
#define CS_SQLCMD_SELECT(field, table)				string("select ") + field + " from " + table + " where 1=1 "
#define CS_SQLCMD_SELECT_COUNT(table)				string("select count(1) as ") + CS_SQLFIELD_COMMON_COUNTNAME + " from " + table + " where 1=1 "
#define CS_SQLCMD_INSERT(table, fields, values )	string("insert into ") + table + " (" + fields + ") value (" + values + ")"
#define CS_SQLCMD_INSERT_DELAY(table, fields, values )	string("insert delayed into ") + table + " (" + fields + ") value (" + values + ")"
#define CS_SQLCMD_UPDATE(table, setValue )			string("update ") + table + " set " + setValue + " where 1=1 "
#define CS_SQLCMD_DELETE(table)						string("delete from ") + table + " where 1=1 "
// sql where 
#define CS_SERVICEDB_SQLWHERE_LIMIT					"limit"
#define CS_SERVICEDB_SQLWHERE_UNION					"union"

// split key
#define CS_SPLITKEY_COMMA							","
#define CS_SPLITKEY_POINT							"."

#define CS_COMMON_FUNC_(msg)						string(string("[") + msg + "::" + _RUN_FUNC_ + "]" ).c_str()

/// *******************************************************************************************************


//
// marco function
//
#define CS_LOOP_MAP_DEBUG(preStr, mapName )			for(map<string,string>::const_iterator citer = mapName.begin(); citer != mapName.end(); ++citer ) \
														DEBUG_D("%s : [%s] => [%s]", string(preStr).c_str(), citer->first.c_str(), citer->second.c_str() );
/// *******************************************************************************************************

#endif


