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

namespace fr_public{
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
		char date_tmp[35] = {};
		strftime(date_tmp, sizeof(date_tmp), date_format.c_str(), localtime(&second)); 
		return string(date_tmp, 35);
	}

	time_t GetLocalTime(){
		return chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	}
	
	void GetLocalTimeUs(time_t& second_part, time_t& us_part){
		auto time_now = chrono::system_clock::now();
		auto duration_in_us = chrono::duration_cast<chrono::microseconds>(time_now.time_since_epoch());
		auto us_part_ref = duration_in_us - chrono::duration_cast<chrono::seconds>(duration_in_us);
		second_part = chrono::system_clock::to_time_t(time_now);
		us_part = us_part_ref.count(); 
	}

	string GetLocalTime(const string &date_format){
		return FormatDateTime(GetLocalTime(), date_format);
	}

	string GetLocalTimeU(const string &date_format){
		time_t second_part(0), us_part(0);
		GetLocalTimeUs(second_part, us_part);
		return FormatDateTime(second_part, date_format) + "." + to_string(us_part);
	}
}  // namepsace : fr_public


/*
 * TimeCounter
 */
namespace fr_public{
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
}  // namespace : fr_public

