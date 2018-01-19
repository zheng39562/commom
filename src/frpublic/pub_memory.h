/**********************************************************
 *  \!file pub_memory.h
 *  \!brief
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _pub_memory_H
#define _pub_memory_H

#include "pub_define.h"

namespace frpublic{
	//! \brief	
	//! \note	请勿在外界直接操作内存。这会导致行为未定义。
	//! \note	创建和使用：
	//				* 主要提供内存的封装概念。
	//				* 该类会独立的管理内存，从外界复制的数据为深复制。所以，频繁的构建该类会有大量内存的malloc和free。
	//! \note	注意点和缺陷：
	//				* 没有提供内存修改的函数体系。如修改数据需要直接调用指针。然而直接调用指针无法对增删代码进行响应。
	//				* 最开始size 和 length 使用int(即无符号整形)，但最后在使用中发现，使用int时，碰见int的负数，会导致一些非期望行为.所以将所有int修改为int
	//					* 例如：length = -1的del操作，会删除所有的内存。而length=-1可能在调用本意应该是不删除任何数据(无论是输错还是如何，-1等不应该delete any data。)
	//					* 这里注意uint 和 int之间的转换在g++某些版本的默认配置下，是没有任何提示(包括警告也是没有的)的
	//! \todo	考虑重载 <<。接受各类基础类型转换为字节流的方式。
	//! \todo	获取外界new指针，避免重复的内存new/malloc
	class BinaryMemory{
		public:
			BinaryMemory();
			BinaryMemory(const void *_buffer, size_t _size);
			BinaryMemory(const Byte *_buffer, size_t _size);
			BinaryMemory(const char *_buffer, size_t _size);
			BinaryMemory(const short *_buffer, size_t _size);
			BinaryMemory(const int *_buffer, size_t _size);
			BinaryMemory(const BinaryMemory &ref);
			~BinaryMemory();
		public:
			BinaryMemory& operator=(const BinaryMemory &ref);
			BinaryMemory& operator+(const BinaryMemory &ref);
		public:
			void add(const BinaryMemory &ref);
			void add(const void* buffer, size_t size);
			inline void add(const Byte* buffer, size_t size){ add((const void*)buffer, size); }
			inline void add(const char* buffer, size_t size){ add((const void*)buffer, size); }

			void set(const void* buffer, size_t size);
			void* CopyMemoryFromOut(size_t size);

			//! \brief	删除buffer内容:不更改内存大小。
			void del(int start, size_t length);
			inline void del(size_t length){ del(0, length); }

			void clear();
			//! \brief	扩展内存大小。
			//! \note	如果已满足扩展大小则不做操作。
			//! \note	扩展不会影响已有的数据。
			void reserve(size_t size);
			//! \brief	重定义内存大小。
			//! \note	resize 和 reserve的区别主要在于，resize会或者内存尺寸。
			void resize(size_t size);
			void set_max_limit(size_t max_limit){ max_limit_ = max_limit; }
			std::string print(std::string expand = "")const;

			inline bool empty()const{ return cur_buffer_size_ == 0; }
			//! \note	直接操作内存.仅用于内容修改。：如有需要可以使用，但请慎重使用。
			//! \note	对内存的增删没有意义:因size不会改变。如做内存增删,请使用del函数。
			inline void* buffer(){ return buffer_; }
			inline void* buffer(int32_t offset){ return (cur_buffer_size_ > offset ? (void*)((char*)buffer_ + offset) : NULL); }

			inline const void* buffer()const{ return (const void*)buffer_; }
			inline const void* buffer(int32_t offset)const{ return (cur_buffer_size_ > offset ? (const void*)((char*)buffer_ + offset) : NULL); }

			inline size_t size()const{ return cur_buffer_size_; }
			inline size_t max_buffer_size()const{ return max_buffer_size_; }
			inline size_t max_limit()const{ return max_limit_; }
			inline size_t usable_size()const{ return max_buffer_size() - size(); } 
		private:
			void FreeMemory(void** pPoint);
		private:
			void* buffer_;
			size_t cur_buffer_size_;
			size_t max_buffer_size_;
			size_t max_limit_;
	};
	typedef std::shared_ptr<BinaryMemory> BinaryMemoryPtr;

	// 
	template <typename Integer>
	void PushNumberToBinaryMemory(Integer integer, BinaryMemoryPtr& binary){
		PushNumberToBinaryMemory(integer, *binary);
	}

	template <typename Integer>
	void PushNumberToBinaryMemory(Integer integer, BinaryMemory& binary){
		binary.add((void*)&integer, sizeof(Integer));
	}
}

#endif 


