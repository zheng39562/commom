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
			delete (char*)m_Buffer;
			m_Buffer = NULL;
		}
	}

	BinaryMemory::BinaryMemory(const BinaryMemory &ref){
		if(m_Buffer != ref.getBuffer()){
			m_Buffer = NULL;
			m_CurBufferSize = 0;
			m_MaxBufferSize = 0;
		}
		setBuffer(ref.getBuffer(), ref.getBufferSize());
	}

	BinaryMemory& BinaryMemory::operator=(const BinaryMemory &ref){
		setBuffer(ref.getBuffer(), ref.getBufferSize());
		return *this;
	}

	BinaryMemory& BinaryMemory::operator+(const BinaryMemory &ref){
		addBuffer(ref.getBuffer(), ref.getBufferSize());
		return *this;
	}

	void BinaryMemory::setBuffer(const void *buffer, size_t size){
		if(buffer == NULL || size == 0 || m_Buffer == buffer){
			return;
		}
		m_CurBufferSize = size;
		if(m_Buffer){
			if(size > m_MaxBufferSize){
				m_MaxBufferSize = size;

				delete (char*)m_Buffer;
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
		if(buffer == NULL || size == 0 || m_Buffer == buffer){
			return;
		}

		if(m_Buffer){
			if(m_CurBufferSize + size > m_MaxBufferSize){
				void* pBufferTmp = (void*)new char[m_CurBufferSize];
				memcpy(pBufferTmp, m_Buffer, m_CurBufferSize);

				m_MaxBufferSize = (size + m_CurBufferSize) * 2;

				delete (char*)m_Buffer;
				m_Buffer = (void*)new char[m_MaxBufferSize];
				memset(m_Buffer, 0, m_MaxBufferSize);
				memcpy(m_Buffer, pBufferTmp, m_CurBufferSize);

				delete (char*)pBufferTmp;
			}
		}
		else{
			m_CurBufferSize = 0;
			m_MaxBufferSize = size;
			m_Buffer = (void*)new char[m_MaxBufferSize];
		}
		void* pAddPos = (char*)m_Buffer + m_CurBufferSize;
		memcpy(pAddPos, buffer, size);
		m_CurBufferSize += size;
	}

	void BinaryMemory::delBuffer(size_t start, size_t length){
		if(m_CurBufferSize > start){
			length = length < (m_CurBufferSize - start) ? m_CurBufferSize - start : length;

			char* pMoveBuffer(NULL);
			size_t movePos = start + length;
			size_t moveLength = m_CurBufferSize - movePos;

			if(moveLength != 0){
				pMoveBuffer = new char[moveLength];
				memcpy(pMoveBuffer, (char*)m_Buffer + movePos, moveLength);
			}

			memset((char*)m_Buffer + start, 0, m_CurBufferSize - start);

			if(pMoveBuffer != NULL){
				memcpy((char*)m_Buffer + start, pMoveBuffer, moveLength);
				delete pMoveBuffer; pMoveBuffer = NULL;
			}
			
			m_CurBufferSize -= length;
		}
	}

	void BinaryMemory::clearBuffer(){
		m_CurBufferSize = 0;
		memset(m_Buffer, 0, m_MaxBufferSize);
	}

	void BinaryMemory::reserve(size_t size){
		if(m_MaxBufferSize < size){
			char* pSaveBuffer(NULL);
			if(m_CurBufferSize != 0){
				pSaveBuffer= new char[m_CurBufferSize];
				memcpy(pSaveBuffer, m_Buffer, m_CurBufferSize);
			}
			if(m_Buffer != NULL){
				delete (char*)m_Buffer; m_Buffer = NULL;
			}

			m_Buffer = (void*)new char[size];
			m_MaxBufferSize = size;

			if(pSaveBuffer != NULL && m_Buffer != NULL){
				memcpy(m_Buffer, pSaveBuffer, m_CurBufferSize);
				delete pSaveBuffer; pSaveBuffer = NULL;
			}
		}
	}

}

