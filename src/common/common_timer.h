/**********************************************************
 * \file logic/tool/Timer.h
 * \brief	时间函数。
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _Timer_H
#define _Timer_H

#include "common/common_tool.h"

//
// 时间通用函数。
//
namespace Universal{
	//! \brief	解析常见的时间，并得到对应的格式。
	//! \note	暂时仅识别年月日(需要完整的年份)。
	//! \note	格式以DateToTime/formatDateTime.函数的格式为标准。
	//! \note	此函数主要用于支持上述两个函数。
	//! \note	支持分割符：非数字的单字节任意分隔符(除字母以外) 或 无分隔符.
	//! \todo	1	两位数年份支持。
	//! \todo	2	时分秒的支持。
	string parseDateFormat(const string &date);
	//! \brief	strptime() 进行转换。
	//! \note	常用指令： %D 月/天/年 %F 年-月-日 %R ：hh:mm %T ：hh:mm:ss
	//!			%Y [年] %m [月] %d [日] %H [时] %M [分] %S [秒]   
	//!			%I 12小时制的小时 %a/A 星期几的简写/全称 %b/B 月分的简写/全称 %j 十进制表示的每年的第几天 %z，%Z 时区名称，如果不能得到时区名称则返回空字符。
	time_t dateToTime(const string date, string dateFormat = "%Y/%m/%d");
	//! \brief	strftime() 使用进行格式转换。
	//! \note	常用指令： %D 月/天/年 %F 年-月-日 %R ：hh:mm %T ：hh:mm:ss
	//!			%Y [年] %m [月] %d [日] %H [时] %M [分] %S [秒]   
	//!			%I 12小时制的小时 %a/A 星期几的简写/全称 %b/B 月分的简写/全称 %j 十进制表示的每年的第几天 %z，%Z 时区名称，如果不能得到时区名称则返回空字符。
	string formatDateTime(const time_t &second, string dateFormat = "%Y/%m/%d");
	string getDateStr(const time_t &second, const string &dateFormat);
	//! \brief	返回从1970.1.1:00:00到当前的秒数。
	time_t getLocalTime();
	time_t getLocalTimeU();
    //! \param[in] dateFormat 要处理的格式.
	string getLocalTime(const string &dateFormat);
	//! \brief	在格式后增加纳秒数，使用[]包含，暂不支持更多格式。
	//! \note	建议格式中包含到秒。
	string getLocalTimeU(const string &dateFormat);
	//! \brief	补全剩余的时间。默认使用 0 补充。可以选择最高数补全
	//! \note	仅支持  y-m-d h:m:s  格式。
	//! \param[in]	strtime	表示时间的字符串。可以省略部分尾部时间。
	//! \param[in]	useMaxNum	尾部时间的补全方式：true表示使用最大值补全。false使用最小值补全。
	time_t supplementTime(const string &strtime, bool useMaxNum = false);
}  // namespace : Universal


namespace Universal{
	//! \brief	计时函数：可以粗略的计算除函数的执行时间。
	//! \note	不建议在毫秒级以下进行计时，因无法保证准确性。
	class TimeCounter{
		public:
			TimeCounter();
			TimeCounter(const string &_logFolder);
			~TimeCounter();
		private:
			timeval			m_TvStart;
			timeval			m_TvStop;
			string			m_LogPath;
		public:
			//! \brief
			void start();
			//! \brief
			void stop();
			//! \brief
			long getSecTime();
			double getSecTimeD();
			long getMsTime();
			double getMsTimeD();
			long getUsTime();
			double getUsTimeD();
			//! \brief
			void setLogPath();
			//! \brief	
			void writeToLog(string preStr = "", string addStr = "");
	};
}  // namespace : Universal



#endif


