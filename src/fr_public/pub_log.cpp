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


#ifdef WIN32
#ifdef _MFC
#include "afxwin.h"
#else
#include <windows.h>
#endif
#endif
#ifdef Linux
#include <pthread.h>
#endif

#include <sstream>
#include "pub_define.h"
#include "pub_file.h"
#include "pub_tool.h"
#include "pub_timer.h"

using namespace std;
using namespace Universal;

namespace Universal{
	Path LogCache::m_s_Path = "";
	size_t LogCache::m_s_MaxSize = LOG_FILE_MAX_SIZE;

	LogCache::LogCache(const FileName &_fileName, eLogLevel _ignore)
		:m_IgnoreLevel(_ignore),
		 m_CurSize(0),
		 m_Outfile(),
		 m_CurIndex(0),
		 m_FileDate(getLocalTime("%Y%m%d")),
		 m_FileName(_fileName)
	{ 
		vector<string> fileNames;
		findFileFromDir(m_s_Path, fileNames, ".*" + m_FileName + ".*\.log");

		for(auto iterFileName = fileNames.begin(); iterFileName != fileNames.end(); ++iterFileName){
			int index = strToInt(iterFileName->substr(iterFileName->find_last_of("_") + 1, iterFileName->find_last_of(".") - iterFileName->find_last_of("_")));
			m_CurIndex = m_CurIndex > index ? m_CurIndex : index;
		}
		
		m_CurIndex -= 1;
		reopen(); 
	}

	LogCache::~LogCache(){
		m_Outfile.close();
	}

	void LogCache::setPath(const Path &path){
		m_s_Path = completePath(path);
		createDir(m_s_Path);
	}

	bool LogCache::fileFull(){
		if(m_s_MaxSize == 0){
			assert(false);
			return false;
		}
		return m_CurSize >= m_s_MaxSize;
	}

	bool LogCache::isNewDate(){ 
		return Universal::getLocalTime("%Y%m%d") != m_FileDate; 
	}

	void LogCache::reopen(){
		if(!m_s_Path.empty() && !m_FileName.empty()){
			string curDate = getLocalTime("%Y%m%d");
			if(curDate != m_FileDate){
				m_CurIndex = 0;
			}
			m_FileDate = curDate;
			m_Outfile.close();
			m_Outfile.open(string(m_s_Path + m_FileName + "_" + m_FileDate + "_" + intToStr(++m_CurIndex) + ".log").c_str(), ios::app);
			m_CurSize = 0;
		}
		else{
			assert(false);
		}
	}

	void LogCache::write(const std::string &time, const eLogLevel &level, const std::string &fileName, const std::string &funcName, const long &line, const std::string &msg){
		lock();
		if(m_IgnoreLevel < level){
			std::ostringstream cache;
			cache << "[" << time << "]"
#ifdef WIN32
				<< "[" << GetCurrentThreadId() << "]"
#endif
#ifdef Linux
				<< "[" << pthread_self() << "]"
#endif
				<< "[" << getLevelString(level) << "]"
				<< "[" << fileName << "]"
				<< "[" << funcName << ":"<< line << "]"
				<< " : " << msg << "\n";

			if(m_Outfile){
				m_CurSize += cache.str().size();
				m_Outfile << cache.str();
				m_Outfile.flush();
				if(fileFull()){
					reopen();
				}
			}
			else{
				assert(false);
			}
		}
		unlock();
	}

	string LogCache::getLevelString(const eLogLevel &level){
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
}

namespace Universal{
	LogServer::LogServer()
		:m_Caches()
	{ ; } 
	LogServer::~LogServer(){ 
		for(auto iterCache = m_Caches.begin(); iterCache != m_Caches.end(); ++iterCache){
			if(iterCache->second != NULL){
				delete iterCache->second; iterCache->second = NULL;
			}
		}
	}

	void LogServer::initLog(const string &path, size_t _maxSize){
		LogCache::setPath(path);
		LogCache::setMaxSize(_maxSize);
		start();
	}

	void LogServer::setLogIgnore(const LogKey &key, eLogLevel ignore){
		if(m_Caches.find(key) == m_Caches.end()){
			m_Caches.insert(make_pair(key, new LogCache(key, ignore)));
		}
		else{
			m_Caches.find(key)->second->setIgnore(ignore);
		}
	}

	void LogServer::writeLog(const LogKey &key, const string &time, const eLogLevel &level, const std::string &fileName, const string &funcName, const long &line, const string &msg){
		if(m_Caches.find(key) == m_Caches.end()){
			m_Caches.insert(make_pair(key, new LogCache(key, eLogLevel_IgnoreNothing)));
		}
		m_Caches.find(key)->second->write(time, level, fileName, funcName, line, msg);
	}

	void LogServer::execute(){
		while(m_Running){
			for(auto iterCache = m_Caches.begin(); iterCache != m_Caches.end(); ++iterCache){
				if(!iterCache->first.empty()){
					LogCache* pCache = iterCache->second;
					pCache->lock();
					if(pCache->isNewDate()){
						pCache->reopen();
					}
					pCache->unlock();
				}
			}

			int curHour(strToInt(getLocalTime("%H")));
			int curMin(strToInt(getLocalTime("%M")));
			if(curHour < 23){
				frSleep(3600 * _TIMECONVERSION_SECTOMS);
			}
			else if(curMin < 59){
				frSleep(60 * _TIMECONVERSION_SECTOMS);
			}
			else{
				frSleep(1 * _TIMECONVERSION_SECTOMS);
			}
		}
	}
}

static std::string g_LogFormat = "%m/%d-%H:%M:%S";  //%Y/%m/%d  
void Log_D(const LogKey &key, const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(key, getLocalTimeU(g_LogFormat), eLogLevel_Debug, fileName, funcName, line, msg);
}

void Log_I(const LogKey &key, const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(key, getLocalTimeU(g_LogFormat), eLogLevel_Info, fileName, funcName, line, msg);
}

void Log_W(const LogKey &key, const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(key, getLocalTimeU(g_LogFormat), eLogLevel_Warning, fileName, funcName, line, msg);
}

void Log_E(const LogKey &key, const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(key, getLocalTimeU(g_LogFormat), eLogLevel_Error, fileName, funcName, line, msg);
}

void Log_C(const LogKey &key, const std::string &msg, const std::string &fileName, const std::string funcName, long line){
	SingleLogServer::getInstance()->writeLog(key, getLocalTimeU(g_LogFormat), eLogLevel_Crash, fileName, funcName, line, msg);
}

