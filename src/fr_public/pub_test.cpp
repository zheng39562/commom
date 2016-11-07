/**********************************************************
 *  \file pub_test.cpp
 *  \brief
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#include "pub_test.h"

#include "pub_file.h"
#include "pub_ini.h"
#include "pub_log.h"
#include "pub_memory.h"
#include "pub_string.h"
#include "pub_thread.h"
#include "pub_timer.h"
#include "pub_tool.h"

using namespace std;
using namespace TestSpace;
using namespace Universal;

void doPublicTest(std::string arg0, std::string arg1, std::string arg2, std::string arg3, std::string arg4){
//	doTimeTest(arg0, arg1, arg2, arg3, arg4);
//	doThreadTest(arg0, arg1, arg2, arg3, arg4);
//	doLogTest(arg0, arg1, arg2, arg3, arg4);
//	doIniTest(arg0, arg1, arg2, arg3, arg4);
}

//! \brief	pub_timer
namespace TestSpace{
	//! \brief	时间函数测试。
	void doTimeTest(std::string arg0, std::string arg1, std::string arg2, std::string arg3, std::string arg4){
		// 字符串转时间
		map<string, time_t> dates;
		dates.insert(make_pair("1969-12", -2678400));
		dates.insert(make_pair("1969-12-01 00:00:01", -2678399));
		dates.insert(make_pair("2010-02-01 00:00:02", 1264982402));
		dates.insert(make_pair("2010-02-01 01:00:03", 1264986003));
		dates.insert(make_pair("2012-03-04 05:06:07", 1330837567));
		DEBUG_D("开始 测试函数dateToTime : 只打印错误数据");
		DEBUG_D(">>>");
		for (map<string, time_t>::const_iterator citer = dates.begin(); citer != dates.end(); ++citer){
			if (dateToTime(citer->first) != citer->second){
				DEBUG_E("***ERROR***" << citer->first << " : " << dateToTime(citer->first) << " | right time : " << citer->second);
			}
		}
		DEBUG_D("<<<"); DEBUG_D("");

		DEBUG_D("开始 测试函数dateToTime : 只打印错误数据"); DEBUG_D(">>>");
		map<string, string> formatMapDate;
		time_t tTest = 1330837567; // 2012-03-04 05:06:07
		formatMapDate.insert(make_pair("%m/%d %H:%S", "03/04 05:07"));
		formatMapDate.insert(make_pair("%Y%d %H%S", "201204 0507"));
		formatMapDate.insert(make_pair("%Y%m%d%H%M%S", "20120304050607"));
		formatMapDate.insert(make_pair("%Y%m%d %H%M%S", "20120304 050607"));
		formatMapDate.insert(make_pair("%Y/%m/%d %H:%M:%S", "2012/03/04 05:06:07"));
		formatMapDate.insert(make_pair("%Y/%m/%d", "2012/03/04"));
		for (map<string, string>::const_iterator citer = formatMapDate.begin(); citer != formatMapDate.end(); ++citer){
			if (formatDateTime(tTest, citer->first) != citer->second){
				DEBUG_E("***ERROR***" << citer->first << " : " << formatDateTime(tTest, citer->first) << " | right time : " << citer->second);
			}
		}
		DEBUG_D("<<<"); DEBUG_D("");

		// 时间戳测试:需要人工对比观察。
		DEBUG_D("[人工确认] 当前时间戳[s] : " << getLocalTime());
		DEBUG_D("[人工确认] 当前时间戳[ms]" << getLocalTimeU());
		DEBUG_D("[人工确认] 当前时间[s] : " << getLocalTime("%Y/%m/%d %H:%M:%S"));
		DEBUG_D("[人工确认] 当前时间[ms]" << getLocalTimeU("%Y/%m/%d %H:%M:%S"));

		// 计时对象。
		TimeCounter timeDebug;
		TimeCounter timeFile("./time_log");

		DEBUG_D("开始计时对象（日志版）测试。"); DEBUG_D(">>>");
		timeDebug.start();
		frSleep(123);
		timeDebug.stop();
		DEBUG_D("<<<"); DEBUG_D("");

		DEBUG_D("[人工确认] 开始计时对象（文件版）测试：检查文件生成 和 文件内容");
		timeFile.start();
		frSleep(321);
		timeFile.stop();
	}
}

//! \brief	pub_thread
namespace TestSpace{
	class TestThread : public Universal::FrThread{
		public:
			virtual void execute(){
				DEBUG_D("A : 线程开始休眠，等待唤醒.");
				pause();
				DEBUG_D("B : 线程已唤醒，该日志打印完后，线程结束.");
			}
	};
	void doThreadTest(std::string arg0, std::string arg1, std::string arg2, std::string arg3, std::string arg4){
		TestThread pThread;
		DEBUG_D("开始线程测试："); DEBUG_D(">>>");
		DEBUG_D("[人工确认] 共需要打印 A B 两部。"); 
		pThread.start();
		frSleep(1000);
		pThread.resume();
		pThread.stop();
		DEBUG_D("<<<"); DEBUG_D("");
	}
}

//! \brief	pub_log
namespace TestSpace{
	void doLogTest(std::string arg0, std::string arg1, std::string arg2, std::string arg3, std::string arg4){
		// 该测试需要对应查找文件，病校验文件中的功能。
#ifndef _FR_CONSOLE
		SingleLogServer::getInstance()->initLog("./", "logtest");
		DEBUG_D("调试");
		DEBUG_I("消息");
		DEBUG_W("警告");
		DEBUG_E("错误");
		DEBUG_C("崩溃");
#else
		DEBUG_D("日志测试 ： 该功能测试需要不声明宏 ： _FR_CONSOLE。测试情况下，都会声明该宏");
#endif
	}
}

//! \brief	pub_ini
namespace TestSpace{
	void doIniTest(std::string arg0, std::string arg1, std::string arg2, std::string arg3, std::string arg4){
		map<string, string> keys;
		keys.insert(make_pair("key1", "01"));
		keys.insert(make_pair("key2", "02"));
		map<string, map<string, string> > sec2Keys;
		sec2Keys.insert(make_pair("section1", keys));
		sec2Keys.insert(make_pair("section2", keys));
		ostringstream oStrIni;
		for(map<string, map<string, string> >::const_iterator citerSec2Keys = sec2Keys.begin(); citerSec2Keys != sec2Keys.end(); ++citerSec2Keys){
			oStrIni << "[" << citerSec2Keys->first << "]" << endl;
			for(map<string, string>::const_iterator citerKey = citerSec2Keys->second.begin(); citerKey != citerSec2Keys->second.end(); ++citerKey){
				oStrIni << " " << citerKey->first << " = " << citerKey->second << " " << endl;
			}
		}

		string filePath = "./test.ini";
		writeFile(filePath, oStrIni.str());

		DEBUG_D("开始测试ini："); DEBUG_D(">>>");
		SingleIniCfg::getInstance()->initFile(filePath);
		for(map<string, map<string, string> >::const_iterator citerSec2Keys = sec2Keys.begin(); citerSec2Keys != sec2Keys.end(); ++citerSec2Keys){
			oStrIni << "]" << citerSec2Keys->first << "]" << endl;
			for(map<string, string>::const_iterator citerKey = citerSec2Keys->second.begin(); citerKey != citerSec2Keys->second.end(); ++citerKey){
				if(citerKey->second != SingleIniCfg::getInstance()->getString(citerSec2Keys->first, citerKey->first)){
					DEBUG_D("***ERROR***没有找到对应的value. " 
							<< "section [" << citerSec2Keys->first << "] " 
							<< "key [" << citerKey->first << "] "
							<< "value[" << SingleIniCfg::getInstance()->getString(citerSec2Keys->first, citerKey->first) << "] "
							<< "right value [" << citerKey->second<< "]");
				}
			}
		}
		DEBUG_D("<<<"); DEBUG_D("");
	}
}

