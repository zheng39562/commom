/**********************************************************
 *  \!file pub_log.h
 *  \!brief
 *  \!note	×¢ÒâÊÂÏî£º 
 * 			1,ÀàÖÐµÄ³ÉÔ±º¯ÊýÖÐµÄÍ¬Ãû²ÎÊýµÄº¬ÒåÍêÈ«ÏàÍ¬¡£½ö»á×¢ÊÍÆäÖÐÒ»¸öº¯Êý£¬ÆäËûº¯ÊýÔò²»ÔÙÖØ¸´×¢ÊÍ¡£ÖØÃûµÄ²ÎÊýÒâÒå²»Í¬Ê±£¬»á¶ÀÁ¢×¢½â¡£ 
 * 			2,µÚ1ÌõµÄ¹æÔòÍ¬ÑùÊÊÓÃÓÚ·µ»ØÖµµÄº¬Òå¡£ 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _pub_log_H
#define _pub_log_H

#include "pub_define.h"
#include "fr_template/single_mode.hpp"
#include "pub_thread.h"

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
	//! \brief	ÈÕÖ¾¼ÇÂ¼Àà¡£
	//! \note	¼òÂÔµÄÌá¹©·ÖÎÄ¼þ¹¦ÄÜ£º100WÌõÊý¾Ý×÷ÎªÒ»¸öÎÄ¼þ¡£
	class LogServer : public FrThread{
		public:
			LogServer();
			~LogServer();
		public:
			void initLog(const std::string &path, const std::string &fileName);
			void writeLog(const std::string &time, const eLogLevel &level, const std::string &fileName, const std::string &funcName, const long &line, const std::string &msg);
		protected:
			virtual void execute();
			std::string getLevelString(const eLogLevel &level);
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

