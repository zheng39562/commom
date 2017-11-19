/**********************************************************
 *  \!file pub_memory.cpp
 *  \!brief
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "pub_memory.h"

#include <string.h>

using namespace std;

namespace fr_public{
	BinaryMemory::BinaryMemory()
		:m_Buffer(NULL),
		 m_MaxLimit(0x7FFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{ ; }
	BinaryMemory::BinaryMemory(const void *_buffer, int _size)
		:m_Buffer(NULL),
		 m_MaxLimit(0x7FFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{ 
		set(_buffer, _size);
	}
	BinaryMemory::BinaryMemory(const Byte *_buffer, int _size)
		:m_Buffer(NULL),
		 m_MaxLimit(0x7FFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{
		int i = sizeof(Byte);
		set((void*)_buffer, _size * sizeof(Byte));
	}
	BinaryMemory::BinaryMemory(const char *_buffer, int _size)
		:m_Buffer(NULL),
		 m_MaxLimit(0x7FFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{
		set((void*)_buffer, _size * sizeof(char));
	}
	BinaryMemory::BinaryMemory(const short *_buffer, int _size)
		:m_Buffer(NULL),
		 m_MaxLimit(0x7FFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{
		set((void*)_buffer, _size * sizeof(short));
	}
	BinaryMemory::BinaryMemory(const int *_buffer, int _size)
		:m_Buffer(NULL),
		 m_MaxLimit(0x7FFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{
		set((void*)_buffer, _size * sizeof(int));
	}

	BinaryMemory::~BinaryMemory(){
		if(m_Buffer != NULL){
			freeMemory(&m_Buffer);
		}
	}

	BinaryMemory::BinaryMemory(const BinaryMemory &ref)
		:m_Buffer(NULL),
		 m_MaxLimit(0x7FFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{
		set(ref.buffer(), ref.size());
	}

	BinaryMemory& BinaryMemory::operator=(const BinaryMemory &ref){
		set(ref.buffer(), ref.size());
		return *this;
	}

	BinaryMemory& BinaryMemory::operator+(const BinaryMemory &ref){
		add(ref.buffer(), ref.size());
		return *this;
	}

	void BinaryMemory::set(const void *buffer, int size){
		if(buffer == NULL || size <= 0 || m_Buffer == buffer || size > m_MaxLimit){
			return;
		}
		m_CurBufferSize = size;
		if(m_Buffer){
			if(size > m_MaxBufferSize){
				m_MaxBufferSize = size;

				freeMemory(&m_Buffer);
				m_Buffer = malloc(m_MaxBufferSize);
			}
		}
		else{
			m_MaxBufferSize = size;
			m_Buffer = malloc(m_MaxBufferSize);
		}
		memset(m_Buffer, 0, m_MaxBufferSize);
		memcpy(m_Buffer, buffer, m_CurBufferSize);
	}

	void BinaryMemory::add(const BinaryMemory &ref){
		add(ref.buffer(), ref.size());
	}
	void BinaryMemory::add(const void *buffer, int size){
		if(buffer == NULL || size <= 0 || m_Buffer == buffer || size + m_CurBufferSize > m_MaxLimit){
			return;
		}

		if(m_Buffer){
			if(m_CurBufferSize + size > m_MaxBufferSize){
				void* pBufferTmp = malloc(m_MaxBufferSize);
				memcpy(pBufferTmp, m_Buffer, m_CurBufferSize);

				m_MaxBufferSize = (size + m_CurBufferSize) * 2;

				freeMemory(&m_Buffer);
				m_Buffer = malloc(m_MaxBufferSize);
				memset(m_Buffer, 0, m_MaxBufferSize);
				memcpy(m_Buffer, pBufferTmp, m_CurBufferSize);

				freeMemory(&pBufferTmp);
			}
		}
		else{
			m_CurBufferSize = 0;
			m_MaxBufferSize = size;
			m_Buffer = malloc(m_MaxBufferSize);
		}
		void* pAddPos = (char*)m_Buffer + m_CurBufferSize;
		memcpy(pAddPos, buffer, size);
		m_CurBufferSize += size;
	}

	void BinaryMemory::del(int start, int length){
		if(m_CurBufferSize > start && length > 0){
			length = length < (m_CurBufferSize - start) ? length : m_CurBufferSize - start;

			void* pMoveBuffer(NULL);
			int movePos = start + length;
			int moveLength = m_CurBufferSize - movePos;

			if(moveLength != 0){
				pMoveBuffer = malloc(moveLength);
				memcpy(pMoveBuffer, (char*)m_Buffer + movePos, moveLength);
			}

			memset((char*)m_Buffer + start, 0, m_CurBufferSize - start);

			if(pMoveBuffer != NULL){
				memcpy((char*)m_Buffer + start, pMoveBuffer, moveLength);
				freeMemory(&pMoveBuffer); pMoveBuffer = NULL;
			}
			
			m_CurBufferSize -= length;
		}
	}

	void BinaryMemory::clear(){
		memset(m_Buffer, 0, m_CurBufferSize);
		m_CurBufferSize = 0;
	}

	void BinaryMemory::reserve(int size){
		if(m_MaxBufferSize < size &&  size <= m_MaxLimit){
			void* pSaveBuffer(NULL);
			if(m_CurBufferSize != 0){
				pSaveBuffer= (void*)malloc(m_CurBufferSize);
				memcpy(pSaveBuffer, m_Buffer, m_CurBufferSize);
			}
			if(m_Buffer != NULL){
				freeMemory(&m_Buffer);
			}

			m_Buffer = malloc(size);
			m_MaxBufferSize = size;
			memset(m_Buffer, 0, m_MaxBufferSize);

			if(pSaveBuffer != NULL && m_Buffer != NULL){
				memcpy(m_Buffer, pSaveBuffer, m_CurBufferSize);
				freeMemory(&pSaveBuffer);
			}
		}
	}

	void BinaryMemory::resize(int size){
		if(m_Buffer != NULL){
			freeMemory(&m_Buffer);
		}

		m_Buffer = malloc(size);
		m_CurBufferSize = size;
		m_MaxBufferSize = size;

		memset(m_Buffer, 0, m_MaxBufferSize);
	}

	string BinaryMemory::print(string expand)const{
		if(m_Buffer == NULL){
			return "buffer is null.";
		}

		Byte* msg = (Byte*)m_Buffer;
		int index(0);
		ostringstream ostr;
		ostr << expand << " buffer is [";
		while(index < m_CurBufferSize){
			ostr << (short)(unsigned char)msg[index] << "|";
			++index;
		}
		ostr << "]";
		return ostr.str();
	}

	void BinaryMemory::freeMemory(void** pPoint){
		free(*pPoint);
		*pPoint = NULL;
	}
}

