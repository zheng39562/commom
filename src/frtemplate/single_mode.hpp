/**********************************************************
 * \file SingleMode.hpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _SingleMode_H
#define _SingleMode_H

#include <mutex>
#include <thread>

namespace frtemplate{
	//! \brief	单例模式的模板类。
	//! \note	使用前置：
	//!			1	被单例的对象，需要有公有的无参数的构造器。
	//!			2	使用该类时可以保证单例。但如果外界有其他创建则无法知晓和保证单例。
	//! \note	建议的通用命名：使用typedef来设置别名，别名在其他类的基础上增加S / Single。
	//! \note	支持有参数的构造器：使用getInstance传入指针。但被单例的类依然必须保证拥有一个无参数的构造器（哪怕此构造器完全无用）
	//! \todo	解决被单例的类必须拥有一个无参数构造器的问题。
	template < typename T >
	class SingleMode{
		public:
			static std::shared_ptr<T> GetInstance();
			//! \brief	允许外界传入一个指针初始化：为保证意外的delete，故使用智能指针。
			static std::shared_ptr<T> GetInstance( std::shared_ptr<T> ptrT );
			//!	\brief	在希望删除单例时调用。但需要注意，如果删除后再调用getInstance依然会重新创建单例.
			static void DestoryInstance();
		private:
			SingleMode(){
				object_ = std::shared_ptr<T>(new T());
			}
			SingleMode( std::shared_ptr<T> _ptrT )
				:object_( _ptrT )
			{ ; }
			virtual ~SingleMode(){ ; }
		private:
			class Deleter{
				public:
					~Deleter(){
						if( SingleMode::static_instance_ != NULL )
							static_mutex_.release();
							delete SingleMode::static_instance_;
					}
			};
			static Deleter						static_delete_;
		private:
			static SingleMode<T>*				static_instance_;  // 
			static std::unique_ptr<std::mutex>	static_mutex_;
	
			std::shared_ptr<T>					object_;
	};

	template < typename T > SingleMode<T>*					SingleMode<T>::static_instance_ = NULL;
	template < typename T > std::unique_ptr<std::mutex>		SingleMode<T>::static_mutex_ = std::unique_ptr<std::mutex>(new std::mutex());
	
	template < typename T >
	std::shared_ptr<T> SingleMode<T>::GetInstance(){
		if(static_instance_ == NULL){
			std::lock_guard<std::mutex> local_lock(*static_mutex_);
			if(static_instance_ == NULL){
				static_instance_ = new SingleMode();
				return static_instance_->object_;
			}
		}
		return static_instance_->object_;
	}
	template < typename T >
	std::shared_ptr<T> SingleMode<T>::GetInstance( std::shared_ptr<T> ptrT ){
		if(static_instance_ == NULL){
			std::lock_guard<std::mutex> local_lock(*static_mutex_);
			if(static_instance_ == NULL){
				static_instance_ = new SingleMode(ptrT);
				return static_instance_->object_;
			}
		}
		return static_instance_->object_;
	}
	template < typename T >
	void SingleMode<T>::DestoryInstance(){
		if(SingleMode::static_instance_ != NULL){ 
			std::lock_guard<std::mutex> local_lock(*static_mutex_);
			if(SingleMode::static_instance_ != NULL){ 
				delete SingleMode::static_instance_; 
				SingleMode::static_instance_ = NULL;
			}
		} 
	}
} // namespace DesignMode

#endif 



