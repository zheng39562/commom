/**********************************************************
 *  \!file common_memory.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _common_memory_H
#define _common_memory_H

#include "common_define.h"

namespace Universal{
	//! \brief	用于储存二进制流数据。并支持类似string的运算符操作。
	//! \note
	class BinaryMemory{
		public:
			BinaryMemory();
			BinaryMemory(const void *_buffer, size_t _size);
			~BinaryMemory();
			BinaryMemory(const BinaryMemory &ref);
			BinaryMemory& operator=(const BinaryMemory &ref);
		public:
			BinaryMemory& operator+(const BinaryMemory &ref);
		public:
			void addBuffer(const void* buffer, size_t size);
			void setBuffer(const void* buffer, size_t size);
			//! \brief	删除部分内容
			//! \param[in] start 起始位置，如果大于当前最大尺寸则不删除任何内容。
			//! \param[in] length 删除的字节数。如果超过当前保存内容，则只删除到末尾。
			void delBuffer(size_t start, size_t length);
			//! \brief	清空缓存，不释放内存。
			void clearBuffer();
			void reserve(size_t size);

			inline bool empty()const{ return m_CurBufferSize == 0; }
			inline const void* getBuffer()const{ return m_Buffer; }
			inline size_t getBufferSize()const{ return m_CurBufferSize; }
		private:
			void* m_Buffer;
			size_t m_CurBufferSize;
			size_t m_MaxBufferSize;
	};
}
#endif 

