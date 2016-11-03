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
#include "boost/timer.hpp"
#include "boost/date_time.hpp"
#include "boost/progress.hpp"

using namespace std;
using namespace boost;
using namespace boost::gregorian;
using namespace boost::posix_time;

namespace Universal{
	time_t dateToTime(const string &date, string dateFormat){
		ptime pTime;
		ptime pBaseTime = time_from_string("1970-01-01 00:00:00");
		time_input_facet facet(dateFormat, 1);

		stringstream dateStream(date);
		dateStream.imbue(std::locale(dateStream.getloc(), &facet));
		dateStream >> pTime;

		time_duration ft = pTime - pBaseTime;
		return ft.total_seconds();
	}

	string formatDateTime(const time_t &second, string dateFormat){
		ptime pTime(from_time_t(second));
		string sTime(to_iso_string(pTime));

		string sYear(sTime.substr(0,4));
		string sMonth(sTime.substr(4,2));
		string sDay(sTime.substr(6,2));
		string sHour(sTime.substr(9,2));
		string sMin(sTime.substr(11,2));
		string sSecond(sTime.substr(13,2));

		bool bYear(false), bMonth(false), bDay(false), bHour(false), bMin(false), bSecond(false);
		string dateSplit("");
		string timeSplit("");
		string midSplit("");
		
		//! year month day 
		bYear = dateFormat.find("%Y") != string::npos;
		bMonth = dateFormat.find("%m") != string::npos;
		bDay = dateFormat.find("%d") != string::npos;;
		if(bYear || bMonth){
			string::size_type pos = dateFormat.find(bYear ? "%Y" : "%m");
			if(pos != string::npos && dateFormat.size() > pos+2 && dateFormat[pos+2] != '%'){
				dateSplit = dateFormat[pos+2];
			}
		}

		// middle split char
		string::size_type midSplitPos(0);
		if(bDay){
			midSplitPos = dateFormat.find("%d") + 2;
		}
		else if(bMonth){
			midSplitPos = dateFormat.find("%m") + 2;
		}
		else if(bYear){
			midSplitPos = dateFormat.find("%Y") + 2;
		}
		if(midSplitPos != 0 && dateFormat.size() > midSplitPos && dateFormat[midSplitPos] != '%'){
			midSplit = dateFormat[midSplitPos];
		}

		// hour min second
		bHour = dateFormat.find("%H") != string::npos;;
		bMin = dateFormat.find("%M") != string::npos;;
		bSecond = dateFormat.find("%S") != string::npos;;
		if(bHour || bMin){
			string::size_type pos = dateFormat.find(bHour ? "%H" : "%M");
			if(pos != string::npos && dateFormat.size() > pos+2 && dateFormat[pos+2] != '%'){
				timeSplit = dateFormat[pos + 2];
			}
		}

		// merge
		sTime.clear();
		sTime += bYear ? sYear + dateSplit : "";
		sTime += bMonth ? sMonth + dateSplit : "";
		sTime += bDay ? sDay : "";
		sTime += !sTime.empty() && (bHour || bMin || bSecond) ? midSplit : "";
		sTime += bHour ? sHour + timeSplit : "";
		sTime += bMin ? sMin + timeSplit : "";
		sTime += bSecond ? sSecond : "";
		return sTime;
	}

	time_t getLocalTime(){
		ptime pTime(second_clock::universal_time());
		ptime pBaseTime = time_from_string("1970-01-01 00:00:00");
		time_duration ft = pTime - pBaseTime;
		return ft.total_seconds();
	}
	
	time_t getLocalTimeU(){
		ptime pTime(microsec_clock::universal_time());
		ptime pBaseTime = time_from_string("1970-01-01 00:00:00.000000");
		time_duration ft = pTime - pBaseTime;
		return ft.total_microseconds();
	}

	string getLocalTime(const string &dateFormat){
		ptime pTime(second_clock::local_time());
		ptime pBaseTime = time_from_string("1970-01-01 00:00:00");
		time_duration ft = pTime - pBaseTime;
		return formatDateTime(ft.total_seconds(), dateFormat);
	}

	string getLocalTimeU(const string &dateFormat){
		ptime pTime(microsec_clock::local_time());
		ptime pBaseTime = time_from_string("1970-01-01 00:00:00.000000");
		time_duration ft = pTime - pBaseTime;
		return formatDateTime(ft.total_seconds(), dateFormat) + "." + intToStr(ft.total_microseconds() % _TIMECONVERSION_SECTOUS);
	}

	time_t supplementTime(const string &strtime, bool useMaxNum){
		if(strtime.size() == 0){
			return -1;
		}
		vector<string> timeDay, timeHour, timeDayAndHour;
		string splitDay("-"), splitHour(":"), splitDayAndHour(" ");

		splitString(strtime, splitDayAndHour, timeDayAndHour);
		if(timeDayAndHour.size() == 2){
			splitString(timeDayAndHour[0], splitDay, timeDay);
			splitString(timeDayAndHour[1], splitHour, timeHour);
		}
		else if(timeDayAndHour.size() == 1)
			splitString(timeDayAndHour[0], splitDay, timeDay);
		else
			return -1;

		string supplementTime;
		long index = 0;
		vector<string>::const_iterator citer = timeDay.begin();
		while(citer != timeDay.end()){
			supplementTime += *citer;
			++index;
			if(++citer != timeDay.end()){
				supplementTime += splitDay;
			}
		}
		// 表示时间到天不需要补全。
		if(index != 3){
			if(index++ == 0)
				supplementTime += "1990";
			if(useMaxNum){
				while(++index <= 3){
					switch(index){
						case 2:
							supplementTime += "-12";
							break;
						case 3:
							supplementTime += "-31";
							break;
					}
				}
			}
			else{
				while(++index <= 3){
					supplementTime += "-00";
				}
			}
		}
		supplementTime += splitDayAndHour;

		index = 0;
		for(vector<string>::const_iterator citer = timeHour.begin(); citer != timeHour.end();){
			supplementTime += *citer;
			++index;
			if(++citer != timeDay.end()){
				supplementTime += splitHour;
			}
		}
		// 表示时间到天不需要补全。
		if(index != 3){
			if(useMaxNum){
				while(++index <= 3){
					switch(index){
						case 1:
							supplementTime += "23";
							break;
						case 2:
							supplementTime += ":59";
							break;
						case 3:
							supplementTime += ":59";
							break;
					}
				}
			}
			else{
				if(index++ == 0)
					supplementTime += "00";
				while(++index <= 3){
					supplementTime += ":00";
				}
			}
		}
		return dateToTime(supplementTime);
	}
}  // namepsace : Universal


/*
 * TimeCounter
 */
namespace Universal{
	TimeCounter::TimeCounter()
		:m_FileName(""),
		 m_Dir("")
	{ ; }
	TimeCounter::TimeCounter(const string &_logDir)
		:m_FileName("time"),
		 m_Dir("")
	{
		setDirectory(_logDir);
	}
	TimeCounter::~TimeCounter(){
	}

	void TimeCounter::start(){ 
		m_Start = std::clock();
	}

	void TimeCounter::stop(string file, long line){ 
		m_Stop = std::clock();
		time_t microSecond = (m_Stop - m_Start) * (_TIMECONVERSION_SECTOUS / CLOCKS_PER_SEC);
		time_t second = microSecond / _TIMECONVERSION_SECTOUS;
		microSecond %= _TIMECONVERSION_SECTOUS;

		if(m_FileName.empty()){
			DEBUG_D("[" << file << ":" << line << "] 运行时间 [" << second << "." << microSecond << "]");
		}
		else{
			std::ostringstream osTmp;
			osTmp << "[" << file << ":" << line << "] 运行时间 [" << second << "." << microSecond << "]";
			addContentToFile(m_Dir + "time" + getLocalTime("%Y%m%d") + ".log", osTmp.str());
		}
	}

	time_t TimeCounter::getUsTime(){ 
		m_Stop = std::clock();
		return (std::clock() - m_Start) * (_TIMECONVERSION_SECTOUS / CLOCKS_PER_SEC);
	}

	void TimeCounter::setDirectory(const string &dir){
		if(!dir.empty()){
			m_Dir = completePath(dir);
			createDir(m_Dir);
		}
		else{
			m_Dir = "./";
		}
	}

	void TimeCounter::setFileName(const string &fileName){
		m_FileName = fileName;
	}

}  // namespace : Universal

