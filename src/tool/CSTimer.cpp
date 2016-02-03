/**********************************************************
 * \file logic/tool/CSTimer.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "CSTimer.h"

#include "CSDefine.h"
#include "CSStrUtil.h"

namespace Universal{
	string parseDateFormat( const string &date ){
		// check
		if( date.size() < 8 )  return CS_STRINGFALSE;

		string keyOfSplit = date.substr(4, 1);
		if( checkNumber( keyOfSplit ) ){
			return "%Y%m%d";
		}
		if( ( keyOfSplit[0] > 'a' && keyOfSplit[0] < 'z' ) || ( keyOfSplit[0] > 'A' && keyOfSplit[0] < 'Z' ) ){
			return CS_STRINGFALSE;
		}
		return "%Y" + keyOfSplit + "%m" + keyOfSplit + "%d";
	}


	time_t dateToTime( const string date, string dateFormat ){
		struct tm local_tm;
		local_tm.tm_year = 0;
		local_tm.tm_mon = 0;
		local_tm.tm_mday = 0;
		local_tm.tm_min = 0;
		local_tm.tm_hour = 0;
		local_tm.tm_sec = 0;

		strptime( date.c_str(), dateFormat.c_str(), &local_tm ) ;
		time_t ft = mktime( &local_tm );
		return ft;
	}


	string formatdateTime( const time_t &second, string dateFormat ){
		struct tm local_tm;
		localtime_r( &second, &local_tm );
		char buf[128] = {0};
		strftime( buf, 64, dateFormat.c_str(), &local_tm );
		return string(buf);
	}


	string getDateStr( const time_t &second, const string &dateFormat ){
		return formatdateTime( second, dateFormat );
	}


	time_t getLocalTime(){
		struct timeval time;
		gettimeofday( &time, NULL);
		return time.tv_sec;
	}


	string getLocalTime( const string &dateFormat ){
		struct timeval time;
		gettimeofday( &time, NULL);
		return formatdateTime( time.tv_sec, dateFormat );
	}


	time_t supplementTime( const string &strtime, bool useMaxNum ){
		if( strtime.size() == 0 ){
			return -1;
		}
		vector<string> timeDay, timeHour, timeDayAndHour;
		string splitDay( "-"), splitHour(":"), splitDayAndHour(" ");

		splitString( strtime, splitDayAndHour, timeDayAndHour );
		if( timeDayAndHour.size() == 2 ){
			splitString( timeDayAndHour[0], splitDay, timeDay );
			splitString( timeDayAndHour[1], splitHour, timeHour );
		}
		else if( timeDayAndHour.size() == 1 )
			splitString( timeDayAndHour[0], splitDay, timeDay );
		else
			return -1;

		string supplementTime;
		long index = 0;
		vector<string>::const_iterator citer = timeDay.begin();
		while( citer != timeDay.end() ){
			supplementTime += *citer;
			++index;
			if( ++citer != timeDay.end() ){
				supplementTime += splitDay;
			}
		}
		// 表示时间到天不需要补全。
		if( index != 3 ){
			if( index++ == 0 )
				supplementTime += "1990";
			if( useMaxNum ){
				while( ++index <= 3){
					switch( index ){
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
				while( ++index <= 3){
					supplementTime += "-00";
				}
			}
		}
		supplementTime += splitDayAndHour;

		index = 0;
		for( vector<string>::const_iterator citer = timeHour.begin(); citer != timeHour.end(); ){
			supplementTime += *citer;
			++index;
			if( ++citer != timeDay.end() ){
				supplementTime += splitHour;
			}
		}
		// 表示时间到天不需要补全。
		if( index != 3 ){
			if( useMaxNum ){
				while( ++index <= 3){
					switch( index ){
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
				if( index++ == 0 )
					supplementTime += "00";
				while( ++index <= 3){
					supplementTime += ":00";
				}
			}
		}
		// return strTodateTime( supplementTime );
		return 0;
	}


}  // namepsace : Universal


/*
 * CSTimeCounter
 */
namespace Universal{
	CSTimeCounter::CSTimeCounter(){
		start();
		string directory = "./mylog";
		createFolder( directory );
		m_LogPath = directory + "time_date" + getLocalTime( "%Y%m%d") + ".log";
	}
	CSTimeCounter::CSTimeCounter( const string &_logFolder ){
		start();
		createFolder( _logFolder );
		m_LogPath = _logFolder + "time_date" + getLocalTime( "%Y%m%d") + ".log";
	}
	CSTimeCounter::~CSTimeCounter(){
	}

	void CSTimeCounter::start(){ gettimeofday( &m_TvStart, NULL ); }


	void CSTimeCounter::stop(){ gettimeofday( &m_TvStop, NULL ); }


	long CSTimeCounter::getSecTime(){ return ( long)getSecTimeD(); }


	double CSTimeCounter::getSecTimeD(){
		return (  m_TvStop.tv_sec - m_TvStart.tv_sec ) + (m_TvStop.tv_usec - m_TvStart.tv_usec ) / (CS_TIMECONVERSION_SECTOMS * CS_TIMECONVERSION_MSTOUS );
	}


	long CSTimeCounter::getMsTime(){ return ( long)getMsTimeD(); }


	double CSTimeCounter::getMsTimeD(){
		return (  m_TvStop.tv_sec - m_TvStart.tv_sec ) * CS_TIMECONVERSION_SECTOMS + (m_TvStop.tv_usec - m_TvStart.tv_usec ) / CS_TIMECONVERSION_MSTOUS;
	}


	long CSTimeCounter::getUsTime(){ return ( long)getUsTimeD(); }


	double CSTimeCounter::getUsTimeD(){
		return (  m_TvStop.tv_sec - m_TvStart.tv_sec ) * CS_TIMECONVERSION_SECTOMS * CS_TIMECONVERSION_MSTOUS + (m_TvStop.tv_usec - m_TvStart.tv_usec );
	}


	void CSTimeCounter::setLogPath(){ m_LogPath = ""; }


	void CSTimeCounter::writeToLog( string preStr, string addStr ){
//		CSAddContentToFile( m_LogPath, preStr + "[ " + intToStr(getMsTime() ) + " ] " + addStr + "  (ms)\n" );
	}
}  // namespace : Universal

