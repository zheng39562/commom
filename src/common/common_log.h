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

#define DEBUG_D(msg) 							{std::ostringstream osTmp; osTmp << msg; Log_D(osTmp.str(), __FILE__, __FUNCTION__, __LINE__);}
#define DEBUG_I(msg) 							{std::ostringstream osTmp; osTmp << msg; Log_D(osTmp.str(), __FILE__, __FUNCTION__, __LINE__);}
#define DEBUG_W(msg) 							{std::ostringstream osTmp; osTmp << msg; Log_D(osTmp.str(), __FILE__, __FUNCTION__, __LINE__);}
#define DEBUG_E(msg) 							{std::ostringstream osTmp; osTmp << msg; Log_D(osTmp.str(), __FILE__, __FUNCTION__, __LINE__);}
#define DEBUG_C(msg) 							{std::ostringstream osTmp; osTmp << msg; Log_D(osTmp.str(), __FILE__, __FUNCTION__, __LINE__);}

namespace Universal{
	enum eLogLevel{
		eLogLevel_Debug,
		eLogLevel_Info,
		eLogLevel_Warning,
		eLogLevel_Error,
		eLogLevel_Crash
	};
	//! \brief	日志记录类。
	//! \note	简略的提供分文件功能：100W条数据作为一个文件。
	class LogServer : public PThread{
		public:
			LogServer();
			~LogServer();
		public:
			void InitLog(const std::string &path, const std::string &fileName);
			void writeLog(const std::string &time, const eLogLevel &level, const std::string &fileName, const std::string &funcName, const long &line, const std::string &msg);
		protected:
			virtual void execute();
			string getLevelString(const eLogLevel &level);
		private:
			std::string m_Path;
			std::string m_FileName;
			std::ostringstream m_Cache;
			PMutex m_MCache;
			size_t m_MaxLine; 
			size_t m_CurLine; 
	};
	typedef DesignMode::SingleMode<LogServer> SingleLogServer;
}

void Log_D(const std::string &msg, const std::string &fileName, const std::string funcName, long line);
void Log_I(const std::string &msg, const std::string &fileName, const std::string funcName, long line);
void Log_W(const std::string &msg, const std::string &fileName, const std::string funcName, long line);
void Log_E(const std::string &msg, const std::string &fileName, const std::string funcName, long line);
void Log_C(const std::string &msg, const std::string &fileName, const std::string funcName, long line);

#endif 

