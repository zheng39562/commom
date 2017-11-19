/**********************************************************
 *  \file pub_window.cpp
 *  \brief
 *  \note	注意事项： 
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#include "pub_window.h"

#if defined(_WIN32) || defined(WIN32)

#ifdef _MFC
#include <afxwin.h>
#else
#include <windows.h>
#endif

#include <psapi.h>  
#include <assert.h>  

using namespace std;

namespace fr_public{
	std::string getGUIDStr(const GUID &guid){
		char buf[70] = {0};
#ifdef __GNUC__
		snprintf(
#else // MSVC
		_snprintf_s(
#endif
		buf,
		sizeof(buf),
		"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
		return std::string(buf);
	}

	GUID parseGUIDStr(const std::string &strGuid){
		GUID guid;
		memset(&guid, 0, sizeof(guid));

		//{00000000-0000-0000-0000-000000000000}
		//01234567890123456789012345678901234568
		//0         1         2         3
		if(strGuid.size() == 38){
			auto funcConvertHexStrToInt = [](char c)->char{
				if('0' <= c && c <= '9'){
					return (int)(c - '0');
				}
				else if('A' <= c && c <= 'F'){
					return (int)(c - 'A' + 10);
				}
				else{
#ifdef _DEBUG
					assert(false);
#endif
				}
				return 0;
			};
			for(int index = 1; index <= 8; ++index){
				guid.Data1 <<= 4;
				guid.Data1 += funcConvertHexStrToInt(strGuid[index]);
			}
			for(int index = 10; index <= 13; ++index){
				guid.Data2 <<= 4;
				guid.Data2 += funcConvertHexStrToInt(strGuid[index]);
			}
			for(int index = 15; index <= 18; ++index){
				guid.Data3 <<= 4;
				guid.Data3 += funcConvertHexStrToInt(strGuid[index]);
			}
			guid.Data4[0] = (funcConvertHexStrToInt(strGuid[20]) << 4) + funcConvertHexStrToInt(strGuid[21]);
			guid.Data4[1] = (funcConvertHexStrToInt(strGuid[22]) << 4) + funcConvertHexStrToInt(strGuid[23]);
			for(int index = 2; index <= 7; ++index){
				guid.Data4[index] = (funcConvertHexStrToInt(strGuid[(index - 2) * 2 + 25]) << 4) + funcConvertHexStrToInt(strGuid[(index - 2) * 2 + 26]);
			}
		}
		return guid;
	}

	string GBKToUTF8(const string& strGBK){
		string strOutUTF8 = ""; 
		WCHAR * str1;
		int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0); 
		str1 = new WCHAR[n]; 
		MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n); 
		n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
		char * str2 = new char[n]; 
		WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL); 
		strOutUTF8 = str2; 
		delete[]str1; 
		str1 = NULL; 
		delete[]str2;
		str2 = NULL; 
		return strOutUTF8;
	}

	string UTF8ToGBK(const string& strUTF8){
		int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
		unsigned short * wszGBK = new unsigned short[len + 1]; 
		memset(wszGBK, 0, len * 2 + 2); 
		MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

		len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
		char *szGBK = new char[len + 1];
		memset(szGBK, 0, len + 1); 
		WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL); //strUTF8 = szGBK; 
		std::string strTemp(szGBK); 
		delete[]szGBK; 
		delete[]wszGBK; 
		return strTemp; 
	}


	/// 时间转换  
	static uint_t file_time_2_utc(const FILETIME* ftime)
	{
		LARGE_INTEGER li;
#ifdef _DEBUG
		assert(ftime);
#endif
		li.LowPart = ftime->dwLowDateTime;
		li.HighPart = ftime->dwHighDateTime;
		return li.QuadPart;
	}

	/// 获得CPU的核数  
	static int get_processor_number()
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return (int)info.dwNumberOfProcessors;
	}

	int get_cpu_usage()
	{
		//cpu数量  
		static int processor_count_ = -1;
		//上一次的时间  
		static int_t last_time_ = 0;
		static int_t last_system_time_ = 0;

		FILETIME now;
		FILETIME creation_time;
		FILETIME exit_time;
		FILETIME kernel_time;
		FILETIME user_time;
		int_t system_time;
		int_t time;
		int_t system_time_delta;
		int_t time_delta;

		int cpu = -1;

		if(processor_count_ == -1)
		{
			processor_count_ = get_processor_number();
		}

		GetSystemTimeAsFileTime(&now);

		if(!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time,
			&kernel_time, &user_time))
		{
			// We don't assert here because in some cases (such as in the Task Manager)
			// we may call this function on a process that has just exited but we have
			// not yet received the notification.  
			return -1;
		}
		system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / processor_count_;
		time = file_time_2_utc(&now);

		if((last_system_time_ == 0) || (last_time_ == 0))
		{
			// First call, just set the last values.  
			last_system_time_ = system_time;
			last_time_ = time;
			return -1;
		}

		system_time_delta = system_time - last_system_time_;
		time_delta = time - last_time_;

		if(time_delta == 0)
			return -1;

		// We add time_delta / 2 so the result is rounded.  
		cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
		last_system_time_ = system_time;
		last_time_ = time;
		return cpu;
	}

	int get_memory_usage(uint_t* mem, uint_t* vmem)
	{
		PROCESS_MEMORY_COUNTERS pmc;
		if(GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
		{
			if(mem) *mem = pmc.WorkingSetSize;
			if(vmem) *vmem = pmc.PagefileUsage;
			return 0;
		}
		return -1;
	}

	int get_io_bytes(uint_t* read_bytes, uint_t* write_bytes)
	{
		IO_COUNTERS io_counter;
		if(GetProcessIoCounters(GetCurrentProcess(), &io_counter))
		{
			if(read_bytes) *read_bytes = io_counter.ReadTransferCount;
			if(write_bytes) *write_bytes = io_counter.WriteTransferCount;
			return 0;
		}
		return -1;
	}

}

#endif

