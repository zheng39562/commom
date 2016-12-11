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
	//! \note	注意点和缺陷：
	//				* 没有提供内存修改的函数体系。如修改数据需要直接调用指针。然而直接调用指针无法对增删代码进行响应。
	//				* 最开始size 和 length 使用int64(即无符号整形)，但最后在使用中发现，使用int64时，碰见int的负数，会导致一些非期望行为.所以将所有int64修改为int
	//					* 例如：length = -1的del操作，会删除所有的内存。而length=-1可能在调用本意应该是不删除任何数据(无论是输错还是如何，-1等不应该delete any data。)
	//					* 这里注意uint 和 int之间的转换在g++某些版本的默认配置下，是没有任何提示(包括警告也是没有的)的
	//! \todo	考虑重载 <<。接受各类基础类型转换为字节流的方式。
	//! \todo	获取外界new指针，避免重复的内存new/malloc
	class BinaryMemory{
		public:
			BinaryMemory();
			BinaryMemory(const void *_buffer, int64 _size);
			BinaryMemory(const Byte *_buffer, int64 _size);
			BinaryMemory(const char *_buffer, int64 _size);
			BinaryMemory(const uint16 *_buffer, int64 _size);
			BinaryMemory(const uint64 *_buffer, int64 _size);
			BinaryMemory(const BinaryMemory &ref);
			~BinaryMemory();
		public:
			BinaryMemory& operator=(const BinaryMemory &ref);
			BinaryMemory& operator+(const BinaryMemory &ref);
		public:
			void addBuffer(const BinaryMemory &ref);
			inline void add(const BinaryMemory &ref){ addBuffer(ref); }
			void addBuffer(const void* buffer, int64 size);
			inline void add(const void* buffer, int64 size){ addBuffer(buffer, size); }
			void setBuffer(const void* buffer, int64 size);
			inline void set(const void* buffer, int64 size){ setBuffer(buffer, size); }
			//! \brief	删除buffer内容:不更改内存大小。
			void delBuffer(int64 start, int64 length);
			inline void del(int64 start, int64 length){ delBuffer(start, length); }
			void clearBuffer();
			inline void clear(){ clearBuffer(); }
			//! \brief	扩展内存大小。
			//! \note	如果已满足扩展大小则不做操作。
			//! \note	扩展不会影响已有的数据。
			void reserve(int64 size);
			void print(std::string expand = "")const;
			void setMaxLimit(int64 _maxLimit){ m_MaxLimit = _maxLimit; }

			inline bool empty()const{ return m_CurBufferSize == 0; }
			//! \note	直接操作内存.仅用于内容修改。：如有需要可以使用，但请慎重使用。
			//! \note	对内存的增删没有意义:因size不会改变。如做内存增删,请使用delBuffer函数。
			inline void* getBuffer(){ return m_Buffer; }
			inline void* buffer(){ return m_Buffer; }
			inline const void* getBuffer()const{ return m_Buffer; }
			inline int64 getBufferSize()const{ return m_CurBufferSize; }
			inline int64 size()const{ return m_CurBufferSize; }
			inline int64 maxSize()const{ return m_MaxBufferSize; }
			inline int64 maxLimit()const{ return m_MaxLimit; }
		private:
			void* m_Buffer;
			int64 m_CurBufferSize;
			int64 m_MaxBufferSize;
			int64 m_MaxLimit;
	};
	typedef boost::shared_ptr<BinaryMemory> BinaryMemoryPtr;
}
#endif 


