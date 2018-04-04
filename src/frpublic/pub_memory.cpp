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

namespace frpublic{
	BinaryMemory::BinaryMemory()
		:buffer_(NULL),
		 max_limit_(0x7FFFFFFF),
		 cur_buffer_size_(0),
		 max_buffer_size_(0)
	{ ; }
	BinaryMemory::BinaryMemory(const void *_buffer, size_t _size)
		:buffer_(NULL),
		 max_limit_(0x7FFFFFFF),
		 cur_buffer_size_(0),
		 max_buffer_size_(0)
	{ 
		set(_buffer, _size);
	}
	BinaryMemory::BinaryMemory(const Byte *_buffer, size_t _size)
		:buffer_(NULL),
		 max_limit_(0x7FFFFFFF),
		 cur_buffer_size_(0),
		 max_buffer_size_(0)
	{
		int i = sizeof(Byte);
		set((void*)_buffer, _size * sizeof(Byte));
	}
	BinaryMemory::BinaryMemory(const char *_buffer, size_t _size)
		:buffer_(NULL),
		 max_limit_(0x7FFFFFFF),
		 cur_buffer_size_(0),
		 max_buffer_size_(0)
	{
		set((void*)_buffer, _size * sizeof(char));
	}
	BinaryMemory::BinaryMemory(const short *_buffer, size_t _size)
		:buffer_(NULL),
		 max_limit_(0x7FFFFFFF),
		 cur_buffer_size_(0),
		 max_buffer_size_(0)
	{
		set((void*)_buffer, _size * sizeof(short));
	}
	BinaryMemory::BinaryMemory(const int *_buffer, size_t _size)
		:buffer_(NULL),
		 max_limit_(0x7FFFFFFF),
		 cur_buffer_size_(0),
		 max_buffer_size_(0)
	{
		set((void*)_buffer, _size * sizeof(int));
	}

	BinaryMemory::~BinaryMemory(){
		if(buffer_ != NULL){
			FreeMemory(&buffer_);
		}
	}

	BinaryMemory::BinaryMemory(const BinaryMemory &ref)
		:buffer_(NULL),
		 max_limit_(0x7FFFFFFF),
		 cur_buffer_size_(0),
		 max_buffer_size_(0)
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

	void BinaryMemory::set(const void *buffer, size_t size){
		if(buffer == NULL || size <= 0 || buffer_ == buffer || size > max_limit_){
			return;
		}
		cur_buffer_size_ = size;
		if(buffer_){
			if(size > max_buffer_size_){
				max_buffer_size_ = size;

				FreeMemory(&buffer_);
				buffer_ = malloc(max_buffer_size_);
			}
		}
		else{
			max_buffer_size_ = size;
			buffer_ = malloc(max_buffer_size_);
		}
		memset(buffer_, 0, max_buffer_size_);
		memcpy(buffer_, buffer, cur_buffer_size_);
	}

	void* BinaryMemory::CopyMemoryFromOut(size_t size){
		cur_buffer_size_ += size;
		if(cur_buffer_size_ > max_limit_){
			cur_buffer_size_ -= size;
			// size bigger than limit. does not allow use buffer. 
			return NULL;
		}

		reserve(cur_buffer_size_);
		return (void*)((Byte*)buffer_ + cur_buffer_size_ - size);
	}

	void BinaryMemory::add(const BinaryMemory &ref){
		add(ref.buffer(), ref.size());
	}
	void BinaryMemory::add(const void *buffer, size_t size){
		if(buffer == NULL || size <= 0 || buffer_ == buffer || size + cur_buffer_size_ > max_limit_){
			return;
		}

		if(buffer_){
			if(cur_buffer_size_ + size > max_buffer_size_){
				void* pBufferTmp = malloc(max_buffer_size_);
				memcpy(pBufferTmp, buffer_, cur_buffer_size_);

				max_buffer_size_ = (size + cur_buffer_size_) * 2;

				FreeMemory(&buffer_);
				buffer_ = malloc(max_buffer_size_);
				memset(buffer_, 0, max_buffer_size_);
				memcpy(buffer_, pBufferTmp, cur_buffer_size_);

				FreeMemory(&pBufferTmp);
			}
		}
		else{
			cur_buffer_size_ = 0;
			max_buffer_size_ = size;
			buffer_ = malloc(max_buffer_size_);
		}
		void* add_pos = (char*)buffer_ + cur_buffer_size_;
		memcpy(add_pos, buffer, size);
		cur_buffer_size_ += size;
	}

	void BinaryMemory::del(int start, size_t length){
		if(start >= 0 && cur_buffer_size_ > start){
			length = length < (cur_buffer_size_ - start) ? length : cur_buffer_size_ - start;

			void* pMoveBuffer(NULL);
			int movePos = start + length;
			int moveLength = cur_buffer_size_ - movePos;

			if(moveLength != 0){
				pMoveBuffer = malloc(moveLength);
				memcpy(pMoveBuffer, (char*)buffer_ + movePos, moveLength);
			}

			memset((char*)buffer_ + start, 0, cur_buffer_size_ - start);

			if(pMoveBuffer != NULL){
				memcpy((char*)buffer_ + start, pMoveBuffer, moveLength);
				FreeMemory(&pMoveBuffer); pMoveBuffer = NULL;
			}
			
			cur_buffer_size_ -= length;
		}
	}

	void BinaryMemory::clear(){
		memset(buffer_, 0, cur_buffer_size_);
		cur_buffer_size_ = 0;
	}

	void BinaryMemory::reserve(size_t size){
		if(max_buffer_size_ < size && size <= max_limit_){
			void* pSaveBuffer(NULL);
			if(cur_buffer_size_ != 0){
				pSaveBuffer= (void*)malloc(cur_buffer_size_);
				memcpy(pSaveBuffer, buffer_, cur_buffer_size_);
			}
			if(buffer_ != NULL){
				FreeMemory(&buffer_);
			}

			buffer_ = malloc(size);
			max_buffer_size_ = size;
			memset(buffer_, 0, max_buffer_size_);

			if(pSaveBuffer != NULL && buffer_ != NULL){
				memcpy(buffer_, pSaveBuffer, cur_buffer_size_);
				FreeMemory(&pSaveBuffer);
			}
		}
	}

	void BinaryMemory::resize(size_t size){
		if(buffer_ != NULL){
			FreeMemory(&buffer_);
		}

		buffer_ = malloc(size);
		cur_buffer_size_ = size;
		max_buffer_size_ = size;

		memset(buffer_, 0, max_buffer_size_);
	}

	string BinaryMemory::to_hex()const{
		if(buffer_ == NULL){
			return "buffer is null.";
		}

		Byte* msg = (Byte*)buffer_;
		int index(0);
		ostringstream ostr;
		while(index < cur_buffer_size_){
			ostr << (short)(unsigned char)msg[index] << "|";
			++index;
		}
		return ostr.str();
	}

	void BinaryMemory::FreeMemory(void** pPoint){
		free(*pPoint);
		*pPoint = NULL;
	}
}

