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
	//				* 需要注意，execute如果是死循环，则必须提供出口。
	//! \note	virtual void execute(){
	//!				...
	//!			}
	//! \todo	支持window
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

			inline boost::thread::native_handle_type getThreadID(){ return m_pThread->native_handle(); }
			inline eThreadStatus getStatus(){ return m_ThreadStatus; }
		protected:
			//! \brief	执行函数。
			//! \note	此函数需要自行设置退出时间，否则会一直执行。
			//! \note	对于exe中的循环线程关闭暂时没有更好的办法。增加m_Running变量，可以辅助关闭线程。但需要在作为循环条件。
			//! \todo	更好的线程关闭方式。
			virtual void execute()=0;
			//! \brief	暂停线程
			//! \note	只支持线程自身暂停自身。
			void pause();
		protected:
			bool m_Running;		//! 启动时设true，析构时false。
		private:
			boost::thread* m_pThread;
			boost::condition_variable m_Cond;
			FrMutex m_Mutex;
			eThreadStatus m_ThreadStatus; //! 线程是否在运行
	};
}

  
#endif  

