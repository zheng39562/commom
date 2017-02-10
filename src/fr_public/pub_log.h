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
#include "pub_define.h"
#include "fr_template/single_mode.hpp"
#include "pub_thread.h"

#define LOG_FILE_MAX_SIZE 10000000 // 10MB
#define DEBUG_KEY_DEFAULT "debug_log"
//! \note	来cout 流信息

#ifdef _MFC
#include "mfc_interface.h"
#define K_DEBUG_D(Key, msg, show) {\
	if(show){std::ostringstream osTmp; osTmp << "key[" << Key << "] " << msg << std::endl; MFC_addLog(Key, osTmp.str());} \
	std::ostringstream osTmp; osTmp << msg; Log_D(Key, osTmp.str(), __FILE__, __FUNCTION__, __LINE__);\
}
#define K_DEBUG_I(Key, msg, show) {\
	if(show){std::ostringstream osTmp; osTmp << "key[" << Key << "] " << msg << std::endl; MFC_addLog(Key, osTmp.str());} \
	std::ostringstream osTmp; osTmp << msg; Log_I(Key, osTmp.str(), __FILE__, __FUNCTION__, __LINE__);\
}
#define K_DEBUG_W(Key, msg, show) {\
	if(show){std::ostringstream osTmp; osTmp << "key[" << Key << "] " << msg << std::endl; MFC_addLog(Key, osTmp.str());} \
	std::ostringstream osTmp; osTmp << msg; Log_W(Key, osTmp.str(), __FILE__, __FUNCTION__, __LINE__);\
}
#define K_DEBUG_E(Key, msg, show) {\
	if(show){std::ostringstream osTmp; osTmp << "key[" << Key << "] " << msg << std::endl; MFC_addLog(Key, osTmp.str());} \
	std::ostringstream osTmp; osTmp << msg; Log_E(Key, osTmp.str(), __FILE__, __FUNCTION__, __LINE__);\
}
#define K_DEBUG_C(Key, msg, show) {\
	if(show){std::ostringstream osTmp; osTmp << "key[" << Key << "] " << msg << std::endl; MFC_addLog(Key, osTmp.str());} \
	std::ostringstream osTmp; osTmp << msg; Log_C(Key, osTmp.str(), __FILE__, __FUNCTION__, __LINE__);\
}
#else
#define K_DEBUG_D(Key, msg, show) {if(show){std::cout << std::dec << "key[" << Key << "] " << msg << std::endl; } std::ostringstream osTmp; osTmp << msg; Log_D(Key, osTmp.str(), __FILE__, __FUNCTION__, __LINE__);}
#define K_DEBUG_I(Key, msg, show) {if(show){std::cout << std::dec << "key[" << Key << "] " << msg << std::endl; } std::ostringstream osTmp; osTmp << msg; Log_I(Key, osTmp.str(), __FILE__, __FUNCTION__, __LINE__);}
#define K_DEBUG_W(Key, msg, show) {if(show){std::cout << std::dec << "key[" << Key << "] " << msg << std::endl; } std::ostringstream osTmp; osTmp << msg; Log_W(Key, osTmp.str(), __FILE__, __FUNCTION__, __LINE__);}
#define K_DEBUG_E(Key, msg, show) {if(show){std::cout << std::dec << "key[" << Key << "] " << msg << std::endl; } std::ostringstream osTmp; osTmp << msg; Log_E(Key, osTmp.str(), __FILE__, __FUNCTION__, __LINE__);}
#define K_DEBUG_C(Key, msg, show) {if(show){std::cout << std::dec << "key[" << Key << "] " << msg << std::endl; } std::ostringstream osTmp; osTmp << msg; Log_C(Key, osTmp.str(), __FILE__, __FUNCTION__, __LINE__);}
#endif
/*
#define K_DEBUG_D(Key, msg, show) 
#define K_DEBUG_I(Key, msg, show) 
#define K_DEBUG_W(Key, msg, show) 
#define K_DEBUG_E(Key, msg, show) std::cout << std::dec << msg << std::endl;
#define K_DEBUG_C(Key, msg, show) 
*/
//#define DEBUG_D(msg) 	{std::cout << std::dec << msg << std::endl; std::ostringstream osTmp; osTmp << msg; Log_D(DEBUG_KEY_DEFAULT, osTmp.str(), __FILE__, __FUNCTION__, __LINE__);}
#define DEBUG_D(msg) K_DEBUG_D(DEBUG_KEY_DEFAULT, msg, false)
#define DEBUG_I(msg) K_DEBUG_I(DEBUG_KEY_DEFAULT, msg, true)
#define DEBUG_W(msg) K_DEBUG_W(DEBUG_KEY_DEFAULT, msg, true)
#define DEBUG_E(msg) K_DEBUG_E(DEBUG_KEY_DEFAULT, msg, true)
#define DEBUG_C(msg) K_DEBUG_C(DEBUG_KEY_DEFAULT, msg, true)

typedef std::string LogKey;
namespace Universal{
	enum eLogLevel{
		eLogLevel_IgnoreNothing = 0,
		eLogLevel_Debug,
		eLogLevel_Info,
		eLogLevel_Warning,
		eLogLevel_Error,
		eLogLevel_Crash
	};
	class LogCache{
		public:
			LogCache(const FileName &_fileName, eLogLevel _ignore);
			LogCache(const LogCache &ref)=delete;
			LogCache& operator=(const LogCache &ref)=delete;
			~LogCache();
		public:
			inline static void setPath(const Path &path);
			inline static void setMaxSize(size_t maxSize){ m_s_MaxSize = maxSize; }

			inline void setIgnore(eLogLevel ignore){ m_IgnoreLevel = ignore; }

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

			FrMutex m_Mutex;
			eLogLevel m_IgnoreLevel;
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
	class LogServer : public Universal::FrThread{
		public:
			LogServer();
			virtual ~LogServer();
		public:
			//! \brief	日志初始化。必须进行初始化。因为单例原因。在构造时不能进行初始化。
			void initLog(const std::string &path, size_t _maxSize);
			void setLogIgnore(const LogKey &key, eLogLevel ignore);
			void writeLog(const LogKey &key, const std::string &time, const eLogLevel &level, const std::string &fileName, const std::string &funcName, const long &line, const std::string &msg);
		protected:
			virtual void execute();
		private:
			std::map<LogKey, LogCache*> m_Caches;
	};
	typedef DesignMode::SingleMode<LogServer> SingleLogServer;
}

void Log_D(const LogKey &key, const std::string &msg, const std::string &fileName, const std::string funcName, long line);
void Log_I(const LogKey &key, const std::string &msg, const std::string &fileName, const std::string funcName, long line);
void Log_W(const LogKey &key, const std::string &msg, const std::string &fileName, const std::string funcName, long line);
void Log_E(const LogKey &key, const std::string &msg, const std::string &fileName, const std::string funcName, long line);
void Log_C(const LogKey &key, const std::string &msg, const std::string &fileName, const std::string funcName, long line);

#endif 

