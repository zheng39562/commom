/**********************************************************
 * \file logic/tool/PMutex.h
 * \brief 
 * \note 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _PMutex_H
#define _PMutex_H

#include <pthread.h>

namespace Universal{
	//! \brief
	//! \todo	需要按照pthread结构扩充类属性，按照pthead的属性先建立基本结构。
	class PMutex{
		public: 
			PMutex();	
			~PMutex();	
		private:
			mutable pthread_mutex_t mutex; 
		public:
			//! \brief 返回值表示错误。
			//! \return 0 is success.Other number is failed.
			int lock();
			int tryLock();
			int unlock();
	};
	class RWMutex;
}

#endif

