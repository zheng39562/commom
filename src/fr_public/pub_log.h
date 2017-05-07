/**********************************************************
 *  \!file pub_log.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _pub_log_H
#define _pub_log_H

#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include "fr_public/pub_define.h"
#include "fr_template/single_mode.hpp"

#ifdef _MFC
#include "other/mfc_interface.h"
#define LOG_PRINT(key, msg, level) \
{\
	std::ostringstream osTmp; osTmp << msg; \
	MFC_addLog(key, osTmp.str()); \
	Universal::SingleLogServer::getInstance()->writeLog(key, level, __FILE__, __FUNCTION__, __LINE__, osTmp.str());\
}
#else
#define LOG_PRINT(key, msg, level) \
{\
	std::ostringstream osTmp; osTmp << msg; \
	std::cout << std::dec << "key[" << key << "] " << msg << std::endl;\
	Universal::SingleLogServer::getInstance()->writeLog(key, level, __FILE__, __FUNCTION__, __LINE__, osTmp.str());\
}
#endif

#define K_DEBUG_P(key, msg) if(Universal::SingleLogServer::getInstance()->getLogLevel(key) <= Universal::eLogLevel_Program){ LOG_PRINT(key, msg, Universal::eLogLevel_Program); }
#define K_DEBUG_D(key, msg) if(Universal::SingleLogServer::getInstance()->getLogLevel(key) <= Universal::eLogLevel_Debug){ LOG_PRINT(key, msg, Universal::eLogLevel_Debug); }
#define K_DEBUG_I(key, msg) if(Universal::SingleLogServer::getInstance()->getLogLevel(key) <= Universal::eLogLevel_Info){ LOG_PRINT(key, msg, Universal::eLogLevel_Info); }
#define K_DEBUG_W(key, msg) if(Universal::SingleLogServer::getInstance()->getLogLevel(key) <= Universal::eLogLevel_Warning){ LOG_PRINT(key, msg, Universal::eLogLevel_Warning); }
#define K_DEBUG_E(key, msg) if(Universal::SingleLogServer::getInstance()->getLogLevel(key) <= Universal::eLogLevel_Error){ LOG_PRINT(key, msg, Universal::eLogLevel_Error); }
#define K_DEBUG_C(key, msg) if(Universal::SingleLogServer::getInstance()->getLogLevel(key) <= Universal::eLogLevel_Crash){ LOG_PRINT(key, msg, Universal::eLogLevel_Crash); }

#define DEBUG_KEY_DEFAULT "debug_log"
#define DEBUG_P(msg) K_DEBUG_P(DEBUG_KEY_DEFAULT, msg)
#define DEBUG_D(msg) K_DEBUG_D(DEBUG_KEY_DEFAULT, msg)
#define DEBUG_I(msg) K_DEBUG_I(DEBUG_KEY_DEFAULT, msg)
#define DEBUG_W(msg) K_DEBUG_W(DEBUG_KEY_DEFAULT, msg)
#define DEBUG_E(msg) K_DEBUG_E(DEBUG_KEY_DEFAULT, msg)
#define DEBUG_C(msg) K_DEBUG_C(DEBUG_KEY_DEFAULT, msg)

namespace Universal{
	typedef std::string LogKey;
	enum eLogLevel{
		eLogLevel_IgnoreNothing = 0,
		eLogLevel_Program,		//  开发时（编写代码阶段）使用，该功能开发完成后删除或转为其他等级
		eLogLevel_Debug,
		eLogLevel_Info,
		eLogLevel_Warning,
		eLogLevel_Error,
		eLogLevel_Crash
	};
	class LogCache{
		public:
			LogCache(const FileName &_fileName, eLogLevel _level);
			LogCache(const LogCache &ref)=delete;
			LogCache& operator=(const LogCache &ref)=delete;
			~LogCache();
		public:
			inline static void setPath(const Path &path);
			inline static void setMaxSize(size_t maxSize){ m_s_MaxSize = maxSize; }

			inline void setLogLevel(eLogLevel level){ m_LogLevel = level; }
			inline eLogLevel getLogLevel()const{ return m_LogLevel; }

			//! \breif	缓存的锁
			//! \note	时间问题，暂时使用外界加锁的形式解决多线程问题。
			inline void lock(){ m_Mutex.lock(); }
			inline void unlock(){ m_Mutex.unlock(); }

			//! \brief	单文件已满(即超过设置的最大储存上线)
			bool fileFull();
			//! \brief	是不是新的一天
			bool isNewDate();
			//! \brief	重打开一个文件。
			//! \note	如果是当天，则标志+1.否则对应增加新日期的文件。
			void reopen();
			//! \brief	将日志从缓存写入文件。
			void write(const std::string &time, const eLogLevel &level, const std::string &fileName, const std::string &funcName, const long &line, const std::string &msg);
		private:
			std::string getLevelString(const eLogLevel &level);
		private:
			static Path m_s_Path;
			static size_t m_s_MaxSize;

			std::mutex m_Mutex;
			eLogLevel m_LogLevel;
			size_t m_CurSize;
			std::ofstream m_Outfile;
			int32 m_CurIndex;
			std::string m_FileDate;
			FileName m_FileName;
	};
	//! \brief	日志记录类。
	//! \note	增加按照索引分文件记录的功能。
	//				* key 即作为索引，也作为文件名。
	//! \note	日志索引和路径在运行过程中不能减少：即不会提供删除key的接口
	//				* 不能减少,主要防止多线程的删除导致未知的引用or指针
	class LogServer{
		public:
			LogServer();
			virtual ~LogServer();
		public:
			//! \brief	日志初始化。必须进行初始化。因为单例原因。在构造时不能进行初始化。
			void initLog(const std::string &path, size_t _maxSize);
			void setLogLevel(const LogKey &key, eLogLevel level);
			eLogLevel getLogLevel(const LogKey &key)const;
			void writeLog(const LogKey &key, const eLogLevel &level, const std::string &fileName, const std::string &funcName, const long &line, const std::string &msg);
		private:
			std::map<LogKey, LogCache*> m_Caches;
			std::thread m_LoopThread;
			bool m_Running;
	};
	typedef DesignMode::SingleMode<LogServer> SingleLogServer;
}

Universal::eLogLevel PARSE_LOG_STRING(const std::string &log_level);

#endif 

