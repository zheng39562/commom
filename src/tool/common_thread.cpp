/**********************************************************
 *  \!file common_thread.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "common_thread.h"

namespace Universal{
	PThread::PThread()
	{ ; }

	virtual PThread::~PThread() { ; }

	void PThread::threadProxy(void* args){
		PThread *pThread = static_cast<PThread*>(args);
		pThread->execute();
	}

	virtual void PThread::execute()=0{
		;
	}

	bool PThread::start(){
		return pthread_create(&m_PThread, NULL, PThread::threadProxy, this) == 0;
	}

	void PThread::stop(){
		;
	}

}
