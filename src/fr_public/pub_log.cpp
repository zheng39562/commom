/**********************************************************
 *  \!file pub_log.cpp
 *  \!brief
 *  \!note	×¢ÒâÊÂÏî£º 
 * 			1,ÀàÖÐµÄ³ÉÔ±º¯ÊýÖÐµÄÍ¬Ãû²ÎÊýµÄº¬ÒåÍêÈ«ÏàÍ¬¡£½ö»á×¢ÊÍÆäÖÐÒ»¸öº¯Êý£¬ÆäËûº¯ÊýÔò²»ÔÙÖØ¸´×¢ÊÍ¡£ÖØÃûµÄ²ÎÊýÒâÒå²»Í¬Ê±£¬»á¶ÀÁ¢×¢½â¡£ 
 * 			2,µÚ1ÌõµÄ¹æÔòÍ¬ÑùÊÊÓÃÓÚ·µ»ØÖµµÄº¬Òå¡£ 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "pub_log.h"

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "pub_define.h"
#include "pub_file.h"
#include "pub_tool.h"

using namespace std;
using namespace Universal;
namespace Universal{

	LogServer::LogServer()
		:m_Path(),
		 m_FileName(),
		 m_Cache(),
		 m_MCache(),
		 m_CurLine(0),
		 m_MaxLine(1000 * 1000)
	{
	}

	LogServer::~LogServer(){
		;
	}

	void LogServer::initLog(const string &path, const string &fileName){
		m_FileName = fileName;
		m_Path = completePath(path);

		if(createFolder(m_Path)){
			start();
		}
		else{
			throw string("create folder failed.path is [") + path + "]";
		}
	}

	void LogServer::writeLog(const string &time, const eLogLevel &level, const std::string &fileName, const string &funcName, const long &line, const string &msg){
		m_MCache.lock();
		m_Cache << "[" << time << "]"
			<< "[" << pthread_self() << "]"
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
				return "µ÷ÊÔ";
			case eLogLevel_Info:
				return "ÏûÏ¢";
			case eLogLevel_Warning:
				return "¾¯¸æ";
			case eLogLevel_Error:
				return "´íÎó";
			case eLogLevel_Crash:
				return "Òì³£";
		}
		return "Î´Öª";
	}

	void LogServer::execute(){
		int fileIndex(0);

		string curDate;
		ofstream outfile;
		while(1){
			usleep(10 * 1000); // 10ms

			if(!m_FileName.empty() && !m_Path.empty()){
				if(curDate.empty() || curDate != getLocalTime("%Y%m%d") || m_CurLine >= m_MaxLine){
					m_CurLine = 0;
					curDate = getLocalTime("%Y%m%d");
					outfile.close();
					outfile.open(string(m_Path + m_FileName + "_" + curDate + "_" + intToStr(++fileIndex) + ".log").c_str(), ios::app);
					//outfile.open(string(m_Path + m_FileName + "_" + curDate  + ".log").c_str(), ios::app);
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

void Log_D(const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU("%Y/%m/%d %T"), eLogLevel_Debug, fileName, funcName, line, msg);
}

void Log_I(const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU("%Y/%m/%d %T"), eLogLevel_Info, fileName, funcName, line, msg);
}

void Log_W(const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU("%Y/%m/%d %T"), eLogLevel_Warning, fileName, funcName, line, msg);
}

void Log_E(const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU("%Y/%m/%d %T"), eLogLevel_Error, fileName, funcName, line, msg);
}

void Log_C(const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU("%Y/%m/%d %T"), eLogLevel_Crash, fileName, funcName, line, msg);
}

