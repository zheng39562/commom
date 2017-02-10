/**********************************************************
 * \file pub_system.h
 * \brief
 * \note	系统相关信息函数。当前只有window下的API统计
 * 
 *
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _pub_system_H
#define _pub_system_H

#ifdef __cplusplus  
extern "C" {
#endif  

	typedef long long           int64_t;
	typedef unsigned long long  uint64_t;

	//! \brief	获取当前进程的cpu使用率，返回-1失败  
	int get_cpu_usage();
	//! \brief	获取当前进程内存和虚拟内存使用量，返回-1失败，0成功  
	int get_memory_usage(uint64_t* mem, uint64_t* vmem);
	//! \brief	获取当前进程总共读和写的IO字节数，返回-1失败，0成功  
	int get_io_bytes(uint64_t* read_bytes, uint64_t* write_bytes);

#ifdef  __cplusplus  
}
#endif  

#endif 

