/**********************************************************
 *  \!file common_memory.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "common_memory.h"

#include <string.h>

namespace Universal{
	BinaryMemory::BinaryMemory()
		:m_Buffer(NULL),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{ ; }
	BinaryMemory::BinaryMemory(const void *_buffer, size_t _size)
		:m_Buffer(NULL),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(_size)
	{ 
		setBuffer(_buffer, _size);
	}

	BinaryMemory::~BinaryMemory(){
		if(m_Buffer){
			delete m_Buffer;
			m_Buffer = NULL;
		}
	}

	BinaryMemory::BinaryMemory(BinaryMemory &ref){
		setBuffer(ref.getBuffer(), ref.getBufferSize());
	}

	BinaryMemory& BinaryMemory::operator=(BinaryMemory &ref){
		setBuffer(ref.getBuffer(), ref.getBufferSize());
		return this;
	}

	BinaryMemory& BinaryMemory::operator+(BinaryMemory &ref){
		addBuffer(ref.getBuffer(), ref.getBufferSize());
		return this;
	}

	void BinaryMemory::setBuffer(const void *buffer, size_t size){
		m_CurBufferSize = size;
		if(m_Buffer){
			if(size > m_MaxBufferSize){
				m_MaxBufferSize = size;

				delete m_Buffer;
				m_Buffer = (void*)new char[m_MaxBufferSize];
			}
		}
		else{
			m_MaxBufferSize = size;
			m_Buffer = (void*)new char[m_MaxBufferSize];
		}
		memset(m_Buffer, 0, m_MaxBufferSize);
		memcpy(m_Buffer, buffer, m_CurBufferSize);
	}

	void BinaryMemory::addBuffer(const void *buffer, size_t size){
	}

}
