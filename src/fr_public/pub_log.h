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
#include "pub_define.h"
#include "pub_file.h"
#include "fr_template/single_mode.hpp"

#ifdef _MFC
#include "other/mfc_interface.h"
#define LOG_PRINT(key, msg, level) \
{\
	std::ostringstream osTmp; osTmp << msg; \
	MFC_addLog(key, osTmp.str()); \
	universal::SingleLogServer::GetInstance()->WriteLog(key, level, __FILE__, __FUNCTION__, __LINE__, osTmp.str());\
}
#else
#define LOG_PRINT(key, msg, level) \
{\
	std::ostringstream osTmp; osTmp << msg; \
	std::cout << std::dec << "key[" << key << "] " << msg << std::endl;\
	universal::SingleLogServer::GetInstance()->WriteLog(key, level, __FILE__, __FUNCTION__, __LINE__, osTmp.str());\
}
#endif

#define K_DEBUG_P(key, msg) if(universal::SingleLogServer::GetInstance()->log_level(key) <= universal::eLogLevel_Program){ LOG_PRINT(key, msg, universal::eLogLevel_Program); }
#define K_DEBUG_D(key, msg) if(universal::SingleLogServer::GetInstance()->log_level(key) <= universal::eLogLevel_Debug){ LOG_PRINT(key, msg, universal::eLogLevel_Debug); }
#define K_DEBUG_I(key, msg) if(universal::SingleLogServer::GetInstance()->log_level(key) <= universal::eLogLevel_Info){ LOG_PRINT(key, msg, universal::eLogLevel_Info); }
#define K_DEBUG_W(key, msg) if(universal::SingleLogServer::GetInstance()->log_level(key) <= universal::eLogLevel_Warning){ LOG_PRINT(key, msg, universal::eLogLevel_Warning); }
#define K_DEBUG_E(key, msg) if(universal::SingleLogServer::GetInstance()->log_level(key) <= universal::eLogLevel_Error){ LOG_PRINT(key, msg, universal::eLogLevel_Error); }
#define K_DEBUG_C(key, msg) if(universal::SingleLogServer::GetInstance()->log_level(key) <= universal::eLogLevel_Crash){ LOG_PRINT(key, msg, universal::eLogLevel_Crash); }

#define DEBUG_KEY_DEFAULT "debug_log"
#define DEBUG_P(msg) K_DEBUG_P(DEBUG_KEY_DEFAULT, msg)
#define DEBUG_D(msg) K_DEBUG_D(DEBUG_KEY_DEFAULT, msg)
#define DEBUG_I(msg) K_DEBUG_I(DEBUG_KEY_DEFAULT, msg)
#define DEBUG_W(msg) K_DEBUG_W(DEBUG_KEY_DEFAULT, msg)
#define DEBUG_E(msg) K_DEBUG_E(DEBUG_KEY_DEFAULT, msg)
#define DEBUG_C(msg) K_DEBUG_C(DEBUG_KEY_DEFAULT, msg)

namespace universal{
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
			static void set_static_path(const Path &path);
			inline static void set_static_max_size(size_t maxSize){ static_max_size_ = maxSize; }

			inline void set_log_level(eLogLevel level){ log_level_ = level; }
			inline eLogLevel log_level()const{ return log_level_; }

			//! \breif	缓存的锁
			//! \note	时间问题，暂时使用外界加锁的形式解决多线程问题。
			inline void lock(){ mutex_.lock(); }
			inline void unlock(){ mutex_.unlock(); }

			//! \brief	单文件已满(即超过设置的最大储存上线)
			bool IsFileFull();
			//! \brief	是不是新的一天
			bool IsNewDate();
			//! \brief	重打开一个文件。
			//! \note	如果是当天，则标志+1.否则对应增加新日期的文件。
			void Reopen();
			//! \brief	将日志从缓存写入文件。
			void Write(const std::string &time, const eLogLevel &level, const std::string &fileName, const std::string &funcName, const long &line, const std::string &msg);
		private:
			std::string getLevelString(const eLogLevel &level);
		private:
			static Path static_path_;
			static size_t static_max_size_;

			std::mutex mutex_;
			eLogLevel log_level_;
			size_t cur_size_;
			std::ofstream out_file_;
			int cur_index_;
			std::string file_date_;
			FileName file_name_;
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
			void InitLog(const std::string &path, size_t _maxSize);
			void set_log_level(const LogKey &key, eLogLevel level);
			eLogLevel log_level(const LogKey &key)const;
			void WriteLog(const LogKey &key, const eLogLevel &level, const std::string &fileName, const std::string &funcName, const long &line, const std::string &msg);
		private:
			std::map<LogKey, LogCache*> caches_;
			std::thread loop_thread_;
			bool running_;
	};
	typedef universal::SingleMode<LogServer> SingleLogServer;
}

universal::eLogLevel PARSE_LOG_STRING(const std::string &log_level);

#endif 

