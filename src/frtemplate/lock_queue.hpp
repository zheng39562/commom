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
#include <mutex>
#include <thread>

namespace frtemplate{
	//! \brief	自实现的队列操作：队列使用先进先出模式（堆）。
	//! \note	拥有自己的互斥锁。互斥锁仅对单对象做出限制：多个不同的对象没有影响。
	//! \note	存在瑕疵：empty之后再进行pop 再多线程并发时有极小的一段时间处于解锁状态。
	template < typename T >
	class LockQueue {
		public:
			LockQueue();
			~LockQueue();
		private:
			std::mutex mutex_;
			std::queue<T> queue_;
		public:
			void clear();
			//! \brief	推送数据进栈
			bool push( const T &data );
			//! \brief	从栈顶获取对象并从栈中移除
			T pop();
			//
			bool pop(T &item);
			//! \param[in] TQueue 从队列中取出N个变量
			//! \prarm[in] size 取出的量，大于队列中消息量时,取出所有 
			bool pop(std::queue<T> &TQueue, size_t size);
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
		:mutex_(),
		 queue_()
	{ ; }
	template < typename T > LockQueue<T>::~LockQueue(){ ; }

	template < typename T > void LockQueue<T>::clear(){
		std::lock_guard<std::mutex> localLock(mutex_);
		while(!queue_.empty()){
			queue_.pop();
		}
	}

	template < typename T > bool LockQueue<T>::push( const T &data ){
		std::lock_guard<std::mutex> localLock(mutex_);
		queue_.push( data );
		return true;
	}

	template < typename T > T LockQueue<T>::pop(){
		std::lock_guard<std::mutex> localLock(mutex_);
		T dataTmp;
		if(!queue_.empty()){
			dataTmp = queue_.front();
			queue_.pop();
		}

		return dataTmp;
	}

	template < typename T > bool LockQueue<T>::pop(T& item){
		std::lock_guard<std::mutex> localLock(mutex_);
		if(!queue_.empty()){
			item = queue_.front();
			queue_.pop();
			return true;
		}

		return false;
	}

	template < typename T > bool LockQueue<T>::pop(std::queue<T> &TQueue, size_t size){
		std::lock_guard<std::mutex> localLock(mutex_);
		while(TQueue.size() < size && !queue_.empty()){
			TQueue.push(queue_.front());
			queue_.pop();
		}
		return true;
	}

	template < typename T > T LockQueue<T>::top(){
		std::lock_guard<std::mutex> localLock(mutex_);
		T dataTmp = queue_.front();

		return dataTmp;
	}

	template < typename T > bool LockQueue<T>::empty(){
		std::lock_guard<std::mutex> localLock(mutex_);
		bool empty = queue_.empty();

		return empty;
	}

	template < typename T > long LockQueue<T>::size(){
		std::lock_guard<std::mutex> localLock(mutex_);
		long size = queue_.size();

		return size;
	}

	template < typename T > bool LockQueue<T>::swap(std::queue<T> &TQueue){
		std::lock_guard<std::mutex> localLock(mutex_);
		queue_.swap(TQueue);
		return true;
	}

}

#endif 

