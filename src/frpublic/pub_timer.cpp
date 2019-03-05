/**********************************************************
 * \file logic/tool/Timer.cpp
 * \brief 
 * \note	对时间进行
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "pub_timer.h"

#include <time.h>
#include <sstream>

#include "pub_define.h"
#include "pub_string.h"

using namespace std;

namespace frpublic{
	time_t DateToTime(const string &date, string date_format){
		struct tm tm_tmp;
#if defined(_WIN32) || defined(WIN32)
		// window not support strptime function.
		std::istringstream ss_date(date)
		//ss_date.imbue(std::locale("de_DE.utf-8"));
		ss_date >> std::get_time(&tm_tmp, date_format.c_str());
		if(ss_date.fail()) { return -1; }
#else
		// gcc 5.3.1还不支持put_time 暂时无法跨平台使用put_time
		strptime(date.c_str(), date_format.c_str(), &tm_tmp);
#endif
		return mktime(&tm_tmp);
	}

	string FormatDateTime(const time_t &second, string date_format){
		char date_tmp[35] = {0};
		strftime(date_tmp, sizeof(date_tmp), date_format.c_str(), localtime(&second)); 
		return string(date_tmp);
	}

	time_t GetLocalTime(){
		return chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	}
	string GetLocalDate(const string &date_format){
		return FormatDateTime(GetLocalTime(), date_format);
	}
	
	time_t GetLocalTimeMs(){
		return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	}

	string GetLocalDateMs(const string &date_format){
		time_t now_ms = GetLocalTimeMs();
		return FormatDateTime(now_ms / 1000, date_format) + "." + to_string(now_ms % 1000);
	}

	time_t GetLocalTimeUs(time_t& second_part, time_t& us_part){
		return chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch()).count();
	}
}  // namepsace : frpublic


/*
 * TimeCounter
 */
namespace frpublic{
	TimeCounter::TimeCounter()
		:start_(0),
		 stop_(0)
	{ ; }
	TimeCounter::~TimeCounter(){
	}

	void TimeCounter::start(){ 
		start_ = std::clock();
	}

	void TimeCounter::stop(){ 
		start_ = std::clock();
	}

	time_t TimeCounter::us_time(){ 
		stop_ = std::clock();
		return (stop_ - start_) * (1000000 / CLOCKS_PER_SEC);
	}
}  // namespace : frpublic

