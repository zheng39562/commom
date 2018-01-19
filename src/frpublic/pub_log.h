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
#include "frtemplate/single_mode.hpp"

#define LOG_PRINT(key, msg, level) \
{\
	std::ostringstream osTmp; osTmp << msg; \
	frpublic::SingleLogServer::GetInstance()->WriteLog(key, level, __FILE__, __FUNCTION__, __LINE__, osTmp.str());\
}
#define LOG_PRINT_DEFAULT(msg, level) \
{\
	std::ostringstream osTmp; osTmp << msg; \
	frpublic::SingleLogServer::GetInstance()->WriteLog(level, __FILE__, __FUNCTION__, __LINE__, osTmp.str());\
}

#define K_DEBUG_P(key, msg) if(frpublic::SingleLogServer::GetInstance()->log_level(key) <= frpublic::eLogLevel_Program){ LOG_PRINT(key, msg, frpublic::eLogLevel_Program); }
#define K_DEBUG_D(key, msg) if(frpublic::SingleLogServer::GetInstance()->log_level(key) <= frpublic::eLogLevel_Debug){ LOG_PRINT(key, msg, frpublic::eLogLevel_Debug); }
#define K_DEBUG_I(key, msg) if(frpublic::SingleLogServer::GetInstance()->log_level(key) <= frpublic::eLogLevel_Info){ LOG_PRINT(key, msg, frpublic::eLogLevel_Info); }
#define K_DEBUG_W(key, msg) if(frpublic::SingleLogServer::GetInstance()->log_level(key) <= frpublic::eLogLevel_Warning){ LOG_PRINT(key, msg, frpublic::eLogLevel_Warning); }
#define K_DEBUG_E(key, msg) if(frpublic::SingleLogServer::GetInstance()->log_level(key) <= frpublic::eLogLevel_Error){ LOG_PRINT(key, msg, frpublic::eLogLevel_Error); }
#define K_DEBUG_C(key, msg) if(frpublic::SingleLogServer::GetInstance()->log_level(key) <= frpublic::eLogLevel_Crash){ LOG_PRINT(key, msg, frpublic::eLogLevel_Crash); }

#define DEBUG_KEY_DEFAULT "debug_log"
#define DEBUG_P(msg) if(frpublic::SingleLogServer::GetInstance()->default_log_level() <= frpublic::eLogLevel_Program){ LOG_PRINT_DEFAULT(msg, frpublic::eLogLevel_Program); }
#define DEBUG_D(msg) if(frpublic::SingleLogServer::GetInstance()->default_log_level() <= frpublic::eLogLevel_Debug){ LOG_PRINT_DEFAULT(msg, frpublic::eLogLevel_Debug); }
#define DEBUG_I(msg) if(frpublic::SingleLogServer::GetInstance()->default_log_level() <= frpublic::eLogLevel_Info){ LOG_PRINT_DEFAULT(msg, frpublic::eLogLevel_Info); }
#define DEBUG_W(msg) if(frpublic::SingleLogServer::GetInstance()->default_log_level() <= frpublic::eLogLevel_Warning){ LOG_PRINT_DEFAULT(msg, frpublic::eLogLevel_Warning); }
#define DEBUG_E(msg) if(frpublic::SingleLogServer::GetInstance()->default_log_level() <= frpublic::eLogLevel_Error){ LOG_PRINT_DEFAULT(msg, frpublic::eLogLevel_Error); }
#define DEBUG_C(msg) if(frpublic::SingleLogServer::GetInstance()->default_log_level() <= frpublic::eLogLevel_Crash){ LOG_PRINT_DEFAULT(msg, frpublic::eLogLevel_Crash); }

namespace frpublic{
	typedef std::string LogKey;
	enum eLogLevel{
		eLogLevel_IgnoreNothing = 0,
		eLogLevel_Program,		//  开发时（编写代码阶段）使用，该功能开发完成后删除或转为其他等级
		eLogLevel_Debug,
		eLogLevel_Info,
		eLogLevel_Warning,
		eLogLevel_Error,
		eLogLevel_Crash,
		eLogLevel_Close
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
			void Write(const std::string &time, const eLogLevel &level, const std::string &file_name, const std::string &func_name, const long &line, const std::string &msg);
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
			void set_default_log_key(const LogKey& log_key){ default_log_key_ = log_key; };
			const LogKey& default_log_key()const{ return default_log_key_; }
			void set_log_level(const LogKey &key, eLogLevel level);
			eLogLevel log_level(const LogKey &key)const;
			eLogLevel default_log_level()const{ return log_level(default_log_key_); };
			void WriteLog(const eLogLevel &level, const std::string &file_name, const std::string &func_name, const long &line, const std::string &msg);
			void WriteLog(const LogKey &key, const eLogLevel &level, const std::string &file_name, const std::string &func_name, const long &line, const std::string &msg);
		private:
			std::map<LogKey, LogCache*> caches_;
			std::thread loop_thread_;
			bool running_;
			LogKey default_log_key_;
	};
	typedef frtemplate::SingleMode<LogServer> SingleLogServer;
}

frpublic::eLogLevel PARSE_LOG_STRING(const std::string &log_level);

#endif 

