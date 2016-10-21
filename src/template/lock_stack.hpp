/**********************************************************
 * \file LockStack.hpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * \note	栈有封顶
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _LockStack_H
#define _LockStack_H

#include <stack>
#include "common/c_thread.h"

namespace Universal{
	//! \brief	自实现的栈操作。
	//! \note	拥有自己的互斥锁。互斥锁仅对单对象做出限制：多个不同的对象没有影响。
	template<typename T>
	class LockStack {
		public:
			LockStack();
			~LockStack();
		private:
			PMutex					m_Mutex;
			std::stack<T>				m_Stack;
		public:
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
	};
	template<typename T> LockStack<T>::LockStack()
		:m_Mutex(),
		 m_Stack()
	{ ; }
	template<typename T> LockStack<T>::~LockStack(){ ; }

	template<typename T> void LockStack<T>::push( const T &data ){
		m_Mutex.lock();
		m_Stack.push( data );
		m_Mutex.unlock();
	}


	template<typename T> T LockStack<T>::pop(){
		m_Mutex.lock();
		T dataTmp;
		if(!m_Stack.empty()){
			dateTmp = m_Stack.top();
			m_Stack.pop();
		}
		m_Mutex.unlock();

		return dateTmp;
	}


	template<typename T> T LockStack<T>::top(){
		m_Mutex.lock();
		T dataTmp = m_Stack.top();
		m_Mutex.unlock();

		return dateTmp;
	}


	template<typename T> bool LockStack<T>::empty(){
		m_Mutex.lock();
		bool empty = m_Stack.empty();
		m_Mutex.unlock();

		return empty;
	}


	template<typename T> long LockStack<T>::size(){
		m_Mutex.lock();
		long size = m_Stack.size();
		m_Mutex.unlock();

		return size;
	}


}


#endif 

