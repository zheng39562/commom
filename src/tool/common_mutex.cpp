/**********************************************************
 * \file logic/tool/PMutex.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "common_mutex.h"

namespace Universal{
	PMutex::PMutex(){ pthread_mutex_init( &mutex, NULL ); }
	PMutex::~PMutex(){  }
	int PMutex::lock(){ return pthread_mutex_lock( &mutex); }
	int PMutex::tryLock(){ return pthread_mutex_trylock( &mutex); }
	int PMutex::unlock(){ return pthread_mutex_unlock( &mutex); }
}  // namespace Thread

