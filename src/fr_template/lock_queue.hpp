/**********************************************************
 * \file LockQueue.hpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _LockQueue_H
#define _LockQueue_H

#include <queue>
#include "fr_public/pub_thread.h"

namespace Universal{
	//! \brief	自实现的队列操作：队列使用先进先出模式（堆）。
	//! \note	拥有自己的互斥锁。互斥锁仅对单对象做出限制：多个不同的对象没有影响。
	//! \note	存在瑕疵：empty之后再进行pop 再多线程并发时有极小的一段时间处于解锁状态。
	template < typename T >
	class LockQueue {
		public:
			LockQueue();
			~LockQueue();
		private:
			FrMutex m_Mutex;
			std::queue<T> m_Queue;
		public:
			void clear();
			//! \brief	推送数据进栈
			void push( const T &data );
			//! \brief	从栈顶获取对象并从栈中移除
			T pop();
			//! \brief	从栈顶获取对象
			T top();
			//! \brief	
			bool empty();
			//! \brief	
			long size();
			//! \brief	
			bool swap(std::queue<T> &TQueue);
	};
	template < typename T > LockQueue<T>::LockQueue()
		:m_Mutex(),
		 m_Queue()
	{ ; }
	template < typename T > LockQueue<T>::~LockQueue(){ ; }

	template < typename T > void LockQueue<T>::clear(){
		mutex::scoped_lock localLock(m_Mutex);
		while( !m_Queue.empty() ){
			m_Queue.pop();
		}
	}

	template < typename T > void LockQueue<T>::push( const T &data ){
		mutex::scoped_lock localLock(m_Mutex);
		m_Queue.push( data );
	}

	template < typename T > T LockQueue<T>::pop(){
		mutex::scoped_lock localLock(m_Mutex);
		T dataTmp;
		if(!m_Queue.empty()){
			dataTmp = m_Queue.front();
			m_Queue.pop();
		}

		return dataTmp;
	}

	template < typename T > T LockQueue<T>::top(){
		mutex::scoped_lock localLock(m_Mutex);
		T dataTmp = m_Queue.front();

		return dataTmp;
	}

	template < typename T > bool LockQueue<T>::empty(){
		mutex::scoped_lock localLock(m_Mutex);
		bool empty = m_Queue.empty();

		return empty;
	}

	template < typename T > long LockQueue<T>::size(){
		mutex::scoped_lock localLock(m_Mutex);
		long size = m_Queue.size();

		return size;
	}

	template < typename T > long LockQueue<T>::swap(std::queue<T> &TQueue){
		;
	}

}

#endif 

