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
#include "fr_public/pub_thread.h"

#include "boost/bind.hpp"
#include <boost/thread/detail/thread.hpp>
#include <boost/thread/thread_time.hpp>

using namespace boost;

namespace Universal{
	FrThread::FrThread()
		:m_pThread(NULL),
		 m_ThreadStatus(eThreadStatus_New),
		 m_Cond(),
		 m_Mutex(),
		 m_Running(false)
	{ 
		;
	}

	FrThread::~FrThread() { 
		stop();
		if(m_pThread != NULL){
			delete m_pThread;
		}
	}

	void FrThread::threadProxy(){
		this->execute();
	}

	void FrThread::start(){
		m_Running = true;
		function0<void> func = bind(&FrThread::threadProxy, this);
		m_pThread = new thread(func);
		m_ThreadStatus = eThreadStatus_Run;
	}

	void FrThread::stop(){
		m_Running = false;
		m_ThreadStatus = eThreadStatus_Exit;
		join();
	}

	void FrThread::resume(){
		mutex::scoped_lock localLock(m_Mutex);
		m_Cond.notify_all();
	}

	void FrThread::join(){
		if (m_pThread != 0){  
			m_pThread->join();
		}  
	}

	void FrThread::pause(){
		m_ThreadStatus = eThreadStatus_Pause;
		mutex::scoped_lock localLock(m_Mutex);
		m_Cond.wait(localLock);
	}

	bool FrThread::isRunningThread()const{ 
		return m_Running; 
	}

}
