/**********************************************************
 *  \!file pub_memory.h
 *  \!brief
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _pub_memory_H
#define _pub_memory_H

#include "boost/shared_ptr.hpp"
// 前端使用。
#ifndef _CLIENT
#include "pub_define.h"
#else
#include "common/Common.h"
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#endif

namespace Universal{
	//! \brief	
	//! \note	请勿在外界直接操作内存。这会导致行为未定义。
	//! \note	创建和使用：
	//				* 主要提供内存的封装概念。
	//				* 该类会独立的管理内存，从外界复制的数据为深复制。所以，频繁的构建该类会有大量内存的malloc和free。
	//! \todo	考虑重载 <<。接受各类基础类型转换为字节流的方式。
	//! \todo	获取外界new指针，避免重复的内存new/malloc
	class BinaryMemory{
		public:
			BinaryMemory();
			BinaryMemory(const void *_buffer, size_t _size);
			BinaryMemory(const Byte *_buffer, size_t _size);
			BinaryMemory(const char *_buffer, size_t _size);
			BinaryMemory(const uint16 *_buffer, size_t _size);
			BinaryMemory(const uint32 *_buffer, size_t _size);
			BinaryMemory(const BinaryMemory &ref);
			~BinaryMemory();
		public:
			BinaryMemory& operator=(const BinaryMemory &ref);
			BinaryMemory& operator+(const BinaryMemory &ref);
		public:
			void addBuffer(const void* buffer, size_t size);
			void setBuffer(const void* buffer, size_t size);
			//! \brief	删除buffer内容:不更改内存大小。
			void delBuffer(size_t start, size_t length);
			void clearBuffer();
			//! \brief	扩展内存大小。
			//! \note	如果已满足扩展大小则不做操作。
			//! \note	扩展不会影响已有的数据。
			void reserve(size_t size);
			void print(std::string expand = "")const;

			inline bool empty()const{ return m_CurBufferSize == 0; }
			//! \note	请勿在外界直接操作内存。这会导致行为未定义。
			inline void* getBuffer(){ return m_Buffer; }
			inline const void* getBuffer()const{ return m_Buffer; }
			inline size_t getBufferSize()const{ return m_CurBufferSize; }
		private:
			void* m_Buffer;
			size_t m_CurBufferSize;
			size_t m_MaxBufferSize;
	};
	typedef boost::shared_ptr<BinaryMemory> BinaryMemoryPtr;
}
#endif 


