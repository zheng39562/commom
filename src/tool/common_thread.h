/**********************************************************
 *  \!file common_thread.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _common_thread_H
#define _common_thread_H

#include <pthread.h>

namespace Universal{
	enum eThreadStatus{
		eThreadStatus_New,
		eThreadStatus_Run,
		eThreadStatus_Exit
	};
	//! \brief	线程类。
	//! \note	使用方法：实现execute函数即可。
	//! \note	virtual void execute(){
	//!				...
	//!			}
	//! \todo	支持window
	class PThread{
		public:
			typedef pthread_t THREAD;
		public:
			PThread();
			virtual ~PThread();
		public:
			static void threadProxy(void* args);
			//! \brief	
			//! \note	
			virtual void execute()=0;

			//! \brief	线程启动
			void start();
			//! \brief	线程关闭。
			void stop();
			//! \brief	等待线程退出。
			void join();

			inline const THREAD& getThreadID(){ return m_Thread; }
			inline eThreadStatus getStatus(){ return m_ThreadStatus; }
		private:
			THREAD m_Thread; //! 线程ID
			eThreadStatus m_ThreadStatus; //! 线程是否在运行
	};
}
  
#endif  

