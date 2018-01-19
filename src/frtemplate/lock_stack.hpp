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

namespace frtemplate{
	//! \brief	自实现的栈操作。
	//! \note	拥有自己的互斥锁。互斥锁仅对单对象做出限制：多个不同的对象没有影响。
	//! \note	存在瑕疵：empty之后再进行pop 再多线程并发时有极小的一段时间处于解锁状态。
	template<typename T>
	class LockStack {
		public:
			LockStack();
			~LockStack();
		private:
			std::mutex					mutex_;
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
		:mutex_(),
		 m_Stack()
	{ ; }
	template<typename T> LockStack<T>::~LockStack(){ ; }

	template<typename T> void LockStack<T>::push( const T &data ){
		std::lock_guard<std::mutex> localLock(mutex_);
		m_Stack.push( data );
	}

	template<typename T> T LockStack<T>::pop(){
		std::lock_guard<std::mutex> localLock(mutex_);
		T dataTmp;
		if(!m_Stack.empty()){
			dateTmp = m_Stack.top();
			m_Stack.pop();
		}

		return dateTmp;
	}

	template<typename T> T LockStack<T>::top(){
		std::lock_guard<std::mutex> localLock(mutex_);
		T dataTmp = m_Stack.top();

		return dateTmp;
	}

	template<typename T> bool LockStack<T>::empty(){
		std::lock_guard<std::mutex> localLock(mutex_);
		bool empty = m_Stack.empty();

		return empty;
	}

	template<typename T> long LockStack<T>::size(){
		std::lock_guard<std::mutex> localLock(mutex_);
		long size = m_Stack.size();

		return size;
	}

}


#endif 

