/**********************************************************
 *  \!file pub_thread.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "pub_thread.h"

namespace Universal{
	PThread::PThread()
		:m_Thread(-1),
		 m_ThreadStatus(eThreadStatus_New)
	{ ; }

	PThread::~PThread() { 
		if(m_Thread != -1){
			pthread_exit(NULL); 
		}
	}

	void* PThread::threadProxy(void* args){
		PThread *pThread = static_cast<PThread*>(args);
		pThread->execute();

		pthread_exit(NULL);

		return NULL;
	}

	bool PThread::start(){
		int iRet = pthread_create(&m_Thread, NULL, PThread::threadProxy, this);
		if(iRet == 0){
			m_Thread = pthread_self();
			m_ThreadStatus = eThreadStatus_Run;
		}
		else{
			//DEBUG_E("create thread failed. error no [" << iRet << "]");
		}
		return iRet == 0;
	}

	void PThread::stop(){
		m_ThreadStatus = eThreadStatus_Exit;
		m_Thread = -1;
		pthread_exit(NULL);
	}

	void PThread::join(){
		if (m_Thread > 0){  
			pthread_join(m_Thread, NULL);  
		}  
	}

}
  
  
