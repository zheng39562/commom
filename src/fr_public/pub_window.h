/**********************************************************
 *  \file pub_window.h
 *  \brief
 *  \note	收集只用于window平台使用的零散函数。后续根据需求分文件
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _pub_window_H
#define _pub_window_H

#include "pub_define.h"
#if defined(_WIN32) || defined(WIN32)

namespace fr_public{
	std::string getGUIDStr(const GUID &guid);
	GUID parseGUIDStr(const std::string &strGuid);
	//! \brief	
	std::string GBKToUTF8(const std::string& strGBK);
	//! \brief	
	std::string UTF8ToGBK(const std::string& strGBK);

	typedef long long           int_t;
	typedef unsigned long long  uint_t;

	//! \brief	获取当前进程的cpu使用率，返回-1失败  
	int get_cpu_usage();
	//! \brief	获取当前进程内存和虚拟内存使用量，返回-1失败，0成功  
	int get_memory_usage(uint_t* mem, uint_t* vmem);
	//! \brief	获取当前进程总共读和写的IO字节数，返回-1失败，0成功  
	int get_io_bytes(uint_t* read_bytes, uint_t* write_bytes);
}
#endif

#endif 

