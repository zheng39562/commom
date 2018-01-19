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

#include <sstream>
#include "pub_tool.h"
#include "pub_timer.h"
#include "pub_string.h"

using namespace std;
using namespace frpublic;

namespace frpublic{
	Path LogCache::static_path_ = "";
	size_t LogCache::static_max_size_ = 10 * 1024 * 1024;

	LogCache::LogCache(const FileName &_fileName, eLogLevel _level)
		:log_level_(_level),
		 cur_size_(0),
		 out_file_(),
		 cur_index_(0),
		 file_date_(GetLocalTime("%Y%m%d")),
		 file_name_(_fileName)
	{ 
		vector<string> fileNames;
		FindFileFromDir(static_path_, fileNames, ".*" + file_name_ + ".*.log");

		for(auto iterFileName = fileNames.begin(); iterFileName != fileNames.end(); ++iterFileName){
			int index = stoi(iterFileName->substr(iterFileName->find_last_of("_") + 1, iterFileName->find_last_of(".") - iterFileName->find_last_of("_")));
			cur_index_ = cur_index_ > index ? cur_index_ : index;
		}
		
		cur_index_ -= 1;
		Reopen(); 
	}

	LogCache::~LogCache(){
		out_file_.close();
	}

	void LogCache::set_static_path(const Path &path){ 
		static_path_ = frpublic::CompletePath(path); 
		frpublic::CreateDir(static_path_); 
	}

	bool LogCache::IsFileFull(){
		if(static_max_size_ == 0){
#ifdef _DEBUG
			assert(false);
#endif
			return false;
		}
		return cur_size_ >= static_max_size_;
	}

	bool LogCache::IsNewDate(){ 
		return frpublic::GetLocalTime("%Y%m%d") != file_date_; 
	}

	void LogCache::Reopen(){
		if(!static_path_.empty() && !file_name_.empty()){
			string curDate = GetLocalTime("%Y%m%d");
			if(curDate != file_date_){
				cur_index_ = 0;
			}
			file_date_ = curDate;
			out_file_.close();
			out_file_.open(string(static_path_ + file_name_ + "_" + file_date_ + "_" + to_string(++cur_index_) + ".log").c_str(), ios::app);
			cur_size_ = 0;
		}
		else{
#ifdef _DEBUG
			assert(false);
#endif
		}
	}

	void LogCache::Write(const std::string &time, const eLogLevel &level, const std::string &file_name, const std::string &func_name, const long &line, const std::string &msg){
		lock();
		if(log_level_ <= level){
			std::ostringstream cache;
			cache << "[" << time << "]"
				<< "[" << std::this_thread::get_id() << "]"
				<< "[" << getLevelString(level) << "]"
				<< "[" << file_name << "]"
				<< "[" << func_name << ":"<< line << "]"
				<< " : " << msg << "\n";

			if(out_file_){
				cur_size_ += cache.str().size();
				out_file_ << cache.str();
				out_file_.flush();
				if(IsFileFull()){
					Reopen();
				}
			}
			else{
#ifdef _DEBUG
				assert(false);
#endif
			}
		}
		unlock();
	}

	string LogCache::getLevelString(const eLogLevel &level){
		switch(level){
			case eLogLevel_Program:
				return "程序";
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

namespace frpublic{
	LogServer::LogServer()
		:caches_(),
		 loop_thread_(),
		 running_(false),
		 default_log_key_(DEBUG_KEY_DEFAULT)
	{ ; }

	LogServer::~LogServer(){ 
		if(loop_thread_.joinable()){
			running_ = false;
			loop_thread_.join();
		}
		for(auto iterCache = caches_.begin(); iterCache != caches_.end(); ++iterCache){
			if(iterCache->second != NULL){
				delete iterCache->second; iterCache->second = NULL;
			}
		}
	}

	void LogServer::InitLog(const string &path, size_t _maxSize){
		LogCache::set_static_path(path);
		LogCache::set_static_max_size(_maxSize);

		running_ = true;
		loop_thread_ = std::thread([&](){
			while(running_){
				for(auto iterCache = caches_.begin(); iterCache != caches_.end(); ++iterCache){
					if(!iterCache->first.empty()){
						LogCache* pCache = iterCache->second;
						pCache->lock();
						if(pCache->IsNewDate()){
							pCache->Reopen();
						}
						pCache->unlock();
					}
				}

				FrSleep(1000);
			}
		});
	}

	void LogServer::set_log_level(const LogKey &key, eLogLevel level){
		if(caches_.find(key) == caches_.end()){
			caches_.insert(make_pair(key, new LogCache(key, level)));
		}
		else{
			caches_.find(key)->second->set_log_level(level);
		}
	}

	eLogLevel LogServer::log_level(const LogKey &key)const{
		auto iterCache = caches_.find(key);
		if(iterCache != caches_.end()){
			return iterCache->second->log_level();
		}
		return eLogLevel_IgnoreNothing;
	}

	void LogServer::WriteLog(const eLogLevel &level, const std::string &file_name, const std::string &func_name, const long &line, const std::string &msg){
		WriteLog(default_log_key_, level, file_name, func_name, line, msg);
	}
	void LogServer::WriteLog(const LogKey &key, const eLogLevel &level, const std::string &file_name, const string &func_name, const long &line, const string &msg){
		if(caches_.find(key) == caches_.end()){
			caches_.insert(make_pair(key, new LogCache(key, eLogLevel_IgnoreNothing)));
		}
		caches_.find(key)->second->Write(GetLocalTimeU("%m/%d-%H:%M:%S"), level, file_name, func_name, line, msg);
	}
}

frpublic::eLogLevel PARSE_LOG_STRING(const std::string &log_level){
	if("program" == frpublic::StrToLower(log_level)){ return frpublic::eLogLevel_Program; }
	else if("debug" == frpublic::StrToLower(log_level)){ return frpublic::eLogLevel_Debug; }
	else if("info" == frpublic::StrToLower(log_level)){ return frpublic::eLogLevel_Info; }
	else if("warning" == frpublic::StrToLower(log_level)){ return frpublic::eLogLevel_Warning; }
	else if("error" == frpublic::StrToLower(log_level)){ return frpublic::eLogLevel_Error; }
	else if("crash" == frpublic::StrToLower(log_level)){ return frpublic::eLogLevel_Crash; }
	else { return frpublic::eLogLevel_IgnoreNothing; }
}

