/**********************************************************
 * \file logic/tool/CSPMutex.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "CSPMutex.h"

namespace Universal{
	CSPMutex::CSPMutex(){ pthread_mutex_init( &mutex, NULL ); }
	CSPMutex::~CSPMutex(){  }
	int CSPMutex::lock(){ return pthread_mutex_lock( &mutex); }
	int CSPMutex::tryLock(){ return pthread_mutex_trylock( &mutex); }
	int CSPMutex::unlock(){ return pthread_mutex_unlock( &mutex); }
}  // namespace CSThread

