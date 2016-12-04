/**********************************************************
 *  \file pub_test.h
 *  \brief
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _pub_test_H
#define _pub_test_H

#include "pub_define.h"
#include "pub_cfg.h"
#include "pub_collection.h"
#include "pub_file.h"
#include "pub_ini.h"
#include "pub_json.h"
#include "pub_log.h"
#include "pub_md5.h"
#include "pub_memory.h"
#include "pub_rc4.h"
#include "pub_string.h"
#include "pub_thread.h"
#include "pub_timer.h"
#include "pub_tool.h"

//! \brief	公共测试函数。
void doPublicTest(std::string arg0 = "", std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "");

//! \brief	测试各模块功能。
//! \note	大部分测试都有对应错误信息。但有些测试需要人工对比，会有标注。
//! \note	按文件分测试函数。
namespace TestSpace{
	//! \brief	pub_timer。
	void doTimeTest(std::string arg0 = "", std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "");
	//! \brief	pub_thread
	void doThreadTest(std::string arg0 = "", std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "");
	//! \brief	pub_log
	//! \note	该功能在编译时不能包含红_FR_CONSOLE
	void doLogTest(std::string arg0 = "", std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "");
	//! \brief	pub_init
	void doIniTest(std::string arg0 = "", std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "");
}

#endif 

