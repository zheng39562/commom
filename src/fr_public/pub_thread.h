/**********************************************************
 *  \!file pub_thread.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _pub_thread_H
#define _pub_thread_H

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
	class FCThread{
		public:
			typedef pthread_t THREAD;
		public:
			FCThread();
			virtual ~FCThread();
		public:
			static void* threadProxy(void* args);

			//! \brief	线程启动
			bool start();
			//! \brief	线程暂停
			void pause();
			//! \brief	线程恢复（针对暂停）
			void resume();
			//! \brief	线程关闭。
			void stop();
			//! \brief	等待线程退出。
			void join();

			inline const THREAD& getThreadID(){ return m_Thread; }
			inline eThreadStatus getStatus(){ return m_ThreadStatus; }
		protected:
			//! \brief	
			//! \note	
			virtual void execute()=0;
		private:
			THREAD m_Thread; //! 线程ID
			eThreadStatus m_ThreadStatus; //! 线程是否在运行
	};
}

namespace Universal{
	//! \brief
	//! \todo	需要按照pthread结构扩充类属性，按照pthead的属性先建立基本结构。
	class FCMutex{
		public: 
			FCMutex(){ pthread_mutex_init( &mutex, NULL ); }	
			~FCMutex()=default;
		private:
			mutable pthread_mutex_t mutex; 
		public:
			//! \brief 返回值表示错误。
			//! \return 0 is success.Other number is failed.
			inline int lock(){ return pthread_mutex_lock( &mutex); }
			inline int tryLock(){ return pthread_mutex_trylock( &mutex); }
			inline int unlock(){ return pthread_mutex_unlock( &mutex); }
	};
	//! \brief	读写锁未完成
	typedef FCMutex RWMutex;
}
  
#endif  

