/**********************************************************
 *  \!file pub_log.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "pub_log.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include "pub_define.h"
#include "pub_file.h"
#include "pub_tool.h"
#include "pub_timer.h"

using namespace std;
using namespace Universal;

namespace Universal{
	using namespace std;
	LogServer::LogServer()
		:m_Path(),
		 m_FileName(),
		 m_Cache(),
		 m_MCache(),
		 m_CurLine(0),
		 m_MaxLine(1000 * 1000)
	{ ; }

	LogServer::~LogServer(){
		;
	}

	void LogServer::initLog(const string &path, const string &fileName){
		m_FileName = fileName;
		m_Path = completePath(path);

		createDir(m_Path);
		start();
	}

	void LogServer::writeLog(const string &time, const eLogLevel &level, const std::string &fileName, const string &funcName, const long &line, const string &msg){
		m_MCache.lock();
		m_Cache << "[" << time << "]"
			<< "[" << getThreadID() << "]"
			<< "[" << getLevelString(level) << "]"
			<< "[" << fileName << "]"
			<< "[" << funcName << ":"<< line << "]"
			<< " : " << msg << "\n";
		++m_CurLine;
		m_MCache.unlock();
	}

	string LogServer::getLevelString(const eLogLevel &level){
		switch(level){
			case eLogLevel_Debug:
				return "调试";
			case eLogLevel_Info:
				return "消息";
			case eLogLevel_Warning:
				return "警告";
			case eLogLevel_Error:
				return "错误";
			case eLogLevel_Crash:
				return "异常";
		}
		return "未知";
	}

	void LogServer::execute(){
		int fileIndex(0);

		string curDate;
		ofstream outfile;
		while(1){
			frSleep(10); // 10ms

			if(!m_FileName.empty() && !m_Path.empty()){
				if(curDate.empty() || curDate != getLocalTime("%Y%m%d") || m_CurLine >= m_MaxLine){
					m_CurLine = 0;
					curDate = getLocalTime("%Y%m%d");
					outfile.close();
					outfile.open(string(m_Path + m_FileName + "_" + curDate + "_" + intToStr(++fileIndex) + ".log").c_str(), ios::app);
				}

				m_MCache.lock();
				if(outfile){
					outfile << m_Cache.str();
					outfile.flush();
				}
				m_Cache.str(""); // clear cache
				m_MCache.unlock();
			}
		}
		outfile.close();
	}
}

static string g_TimeFormat = "%Y%m%d %H:%M:%S";
void Log_D(const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU(g_TimeFormat), eLogLevel_Debug, fileName, funcName, line, msg);
}

void Log_I(const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU(g_TimeFormat), eLogLevel_Info, fileName, funcName, line, msg);
}

void Log_W(const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU(g_TimeFormat), eLogLevel_Warning, fileName, funcName, line, msg);
}

void Log_E(const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU(g_TimeFormat), eLogLevel_Error, fileName, funcName, line, msg);
}

void Log_C(const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU(g_TimeFormat), eLogLevel_Crash, fileName, funcName, line, msg);
}

