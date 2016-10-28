/**********************************************************
 *  \!file pub_memory.h
 *  \!brief
 *  \!note	×¢ÒâÊÂÏî£º 
 * 			1,ÀàÖÐµÄ³ÉÔ±º¯ÊýÖÐµÄÍ¬Ãû²ÎÊýµÄº¬ÒåÍêÈ«ÏàÍ¬¡£½ö»á×¢ÊÍÆäÖÐÒ»¸öº¯Êý£¬ÆäËûº¯ÊýÔò²»ÔÙÖØ¸´×¢ÊÍ¡£ÖØÃûµÄ²ÎÊýÒâÒå²»Í¬Ê±£¬»á¶ÀÁ¢×¢½â¡£ 
 * 			2,µÚ1ÌõµÄ¹æÔòÍ¬ÑùÊÊÓÃÓÚ·µ»ØÖµµÄº¬Òå¡£ 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _pub_memory_H
#define _pub_memory_H

#include "pub_define.h"

namespace Universal{
	//! \brief	ÓÃÓÚ´¢´æ¶þ½øÖÆÁ÷Êý¾Ý¡£²¢Ö§³ÖÀàËÆstringµÄÔËËã·û²Ù×÷¡£
	//! \note
	class BinaryMemory{
		public:
			BinaryMemory();
			BinaryMemory(const void *_buffer, size_t _size);
			BinaryMemory(const BinaryMemory &ref);
			~BinaryMemory();
		public:
			BinaryMemory& operator=(const BinaryMemory &ref);
			BinaryMemory& operator+(const BinaryMemory &ref);
		public:
			void addBuffer(const void* buffer, size_t size);
			void setBuffer(const void* buffer, size_t size);
			//! \brief	É¾³ý²¿·ÖÄÚÈÝ
			//! \param[in] start ÆðÊ¼Î»ÖÃ£¬Èç¹û´óÓÚµ±Ç°×î´ó³ß´çÔò²»É¾³ýÈÎºÎÄÚÈÝ¡£
			//! \param[in] length É¾³ýµÄ×Ö½ÚÊý¡£Èç¹û³¬¹ýµ±Ç°±£´æÄÚÈÝ£¬ÔòÖ»É¾³ýµ½Ä©Î²¡£
			void delBuffer(size_t start, size_t length);
			//! \brief	Çå¿Õ»º´æ£¬²»ÊÍ·ÅÄÚ´æ¡£
			void clearBuffer();
			//! \brief	ÉèÖÃÔ¤ÁôÄÚ´æ´óÐ¡¡£
			//! \note	Èç¹û´óÐ¡³¬¹ý²»×öÈÎºÎ²Ù×÷£¬À©ÈÝÊ±»á±£´æÔ­ÓÐÊý¾Ý
			void reserve(size_t size);
			//! \brief	ÖØÐÂÉèÖÃÄÚ´æ³ß´ç£¨¿ÉÄÜËõ¼õ£¬Ò²¿ÉÄÜÔö¼Ó£©
			//! \note	Óëreserve²»Í¬µÄ£¬º¯Êý²»»á±£ÁôÊý¾Ý¡£
			//! \note	Ã»ÓÐ¶Ôsize´óÐ¡×÷ÈÎºÎÏÞÖÆ£¬ËùÒÔÐèÒª×¢ÒâÈç¹ûÉêÇë³¬¹ýÔ¤ÁÏµÄ´óÐ¡£¨ÀýÈç100G£©£¬¿ÉÄÜ»áµ¼ÖÂ³ÌÐòÒì³£¡£
			void setBufferSize(size_t size);
			//! \brief	°´×Ö½Ú´òÓ¡Êý¾Ý¡£
			//! \note	Ê¹ÓÃÑ­»·´òÓ¡£¬Ð§ÂÊ²»¸ß¡£ÈÕÖ¾¼¶±ðÎªD¡£
			void print()const;

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


