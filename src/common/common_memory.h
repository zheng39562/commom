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

namespace Universal{
	//! \brief	用于储存二进制流数据。并支持类似string的运算符操作。
	//! \note
	class BinaryMemory{
		public:
			BinaryMemory();
			~BinaryMemory();
			BinaryMemory(BinaryMemory &ref);
			BinaryMemory& operator=(BinaryMemory &ref);
		public:
			BinaryMemory& operator+(BinaryMemory &ref);
		public:
			void setBuffer(const void *buffer, size_t size);
			void addBuffer(const void *buffer, size_t size);

			inline void* getBuffer(){ return m_Buffer; }
			inline size_t getBufferSize()const{ return m_BufferSize; }
		private:
			void* m_Buffer;
			size_t m_CurBufferSize;
			size_t m_MaxBufferSize;
	};
}
#endif 

