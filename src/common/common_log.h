/**********************************************************
 *  \!file common_log.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _common_log_H
#define _common_log_H

#include "common_define.h"
#include "template/single_mode.hpp"
#include "common/common_mutex.h"
#include "common/common_thread.h"

extern std::ostringstream os31415926_tmp;
namespace Universal{
	enum eLogLevel{
		eLogLevel_Debug,
		eLogLevel_Info,
		eLogLevel_Warning,
		eLogLevel_Error,
		eLogLevel_Crash
	};
	class LogServer : public PThread{
		public:
			LogServer();
			~LogServer();
		public:
			void InitLog(const std::string &path, const std::string &fileName);
			void writeLog(const std::string &time, const eLogLevel &level, const std::string &funcName, const long &line, const std::string &msg);
		protected:
			virtual void execute();
			string getLevelString(const eLogLevel &level);
		private:
			std::string m_Path;
			std::string m_FileName;
			std::ostringstream m_Cache;
			PMutex m_MCache;
	};
	typedef DesignMode::SingleMode<LogServer> SingleLogServer;
}

void Log_D(const std::string &msg, std::string funcName = __FUNCTION__, long line = __LINE__);
void Log_I(const std::string &msg, std::string funcName = __FUNCTION__, long line = __LINE__);
void Log_W(const std::string &msg, std::string funcName = __FUNCTION__, long line = __LINE__);
void Log_E(const std::string &msg, std::string funcName = __FUNCTION__, long line = __LINE__);
void Log_C(const std::string &msg, std::string funcName = __FUNCTION__, long line = __LINE__);

#endif 

