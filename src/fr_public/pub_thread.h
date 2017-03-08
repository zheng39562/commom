/**********************************************************
 *  \file pub_thread.h
 *  \brief
 *  \note	注意事项： 
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _pub_thread_H
#define _pub_thread_H

#include "boost/thread.hpp"
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition.hpp>

namespace Universal{
	enum eThreadStatus{
		eThreadStatus_New,
		eThreadStatus_Run,
		eThreadStatus_Pause,
		eThreadStatus_Exit
	};

	typedef boost::mutex FrMutex;
	typedef FrMutex RWMutex;

	//! \brief	线程类。
	//! \note	使用方法：实现execute函数即可。
	//! \note	virtual void execute(){
	//!				...
	//!			}
	//! \attetion	通常派生类实现的execute会使用派生的成员变量，所以建议在派生类析构函数中对线程进行停止（调用stop func）。否则可能对未知内存进行引用
	//				当前通过m_Running变量来通知派生类，线程是否即将被停止
	class FrThread{
		public:
			FrThread();
			virtual ~FrThread();
		public:
			void threadProxy();

			//! \brief	线程启动
			void start();
			//! \brief	线程关闭。
			void stop();
			//! \brief	恢复
			void resume();
			//! \brief	等待线程退出。
			void join();

			inline boost::thread::native_handle_type getThreadID(){ return m_pThread->native_handle(); }
			inline eThreadStatus getStatus(){ return m_ThreadStatus; }
		protected:
			//! \brief	
			//! \note	如果对execute实现中增加死循环，需要将isRunningThread()作为循环的终止条件之一。
			virtual void execute()=0;
			//! \brief	暂停线程
			//! \note	只支持线程自身暂停自身。
			void pause();
			bool isRunningThread()const;
		protected:
		private:
			bool m_Running;
			boost::thread* m_pThread;
			boost::condition_variable m_Cond;
			FrMutex m_Mutex;
			eThreadStatus m_ThreadStatus; //! 线程是否在运行
	};
}

  
#endif  

