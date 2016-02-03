/**********************************************************
 * \file CSSingleMode.hpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _CSSingleMode_H
#define _CSSingleMode_H

#include "CSPMutex.h"
#include "boost/shared_ptr.hpp"

namespace CSDesignMode{
	//! \brief	单例模式的模板类。
	//! \note	使用前置：
	//!			1	被单例的对象，需要有公有的无参数的构造器。
	//!			2	使用该类时可以保证单例。但如果外界有其他创建则无法知晓和保证单例。
	//! \note	建议的通用命名：使用typedef来设置别名，别名在其他类的基础上增加S / Single。
	//! \note	支持有参数的构造器：使用getInstance传入指针。但被单例的类依然必须保证拥有一个无参数的构造器（哪怕此构造器完全无用）
	//! \todo	解决被单例的类必须拥有一个无参数构造器的问题。
	template < typename T >
	class CSSingleMode{
		public:
			static boost::shared_ptr<T> getInstance();
			//! \brief	允许外界传入一个指针初始化：为保证意外的delete，故使用智能指针。
			static boost::shared_ptr<T> getInstance( boost::shared_ptr<T> ptrT );
		private:
			CSSingleMode(){
				m_p_Object = boost::shared_ptr<T>(new T());
			}
			CSSingleMode( boost::shared_ptr<T> _ptrT )
				:m_p_Object( _ptrT )
			{ ; }
			virtual ~CSSingleMode(){ ; }
		private:
			class CSDeleter{
				public:
					~CSDeleter(){
						if( CSSingleMode::m_s_p_Instance != NULL )
							delete CSSingleMode::m_s_p_Instance;
					}
			};
			static CSDeleter						m_s_Deleter;
		private:
			static CSSingleMode<T>*					m_s_p_Instance;  // 
			static Universal::CSPMutex				m_s_Mutex;
	
			boost::shared_ptr<T>					m_p_Object;
	};

	template < typename T > Universal::CSPMutex		CSSingleMode<T>::m_s_Mutex = Universal::CSPMutex();
	template < typename T > CSSingleMode<T>*		CSSingleMode<T>::m_s_p_Instance = NULL;
	
	template < typename T >
	boost::shared_ptr<T> CSSingleMode<T>::getInstance(){
		if( m_s_p_Instance == NULL ){
			m_s_Mutex.lock();
			if( m_s_p_Instance == NULL ){
				m_s_p_Instance = new CSSingleMode();
				return m_s_p_Instance->m_p_Object;
			}
			m_s_Mutex.unlock();
		}
		return m_s_p_Instance->m_p_Object;
	}
	template < typename T >
	boost::shared_ptr<T> CSSingleMode<T>::getInstance( boost::shared_ptr<T> ptrT ){
		if( m_s_p_Instance == NULL ){
			m_s_Mutex.lock();
			if( m_s_p_Instance == NULL ){
				m_s_p_Instance = new CSSingleMode(ptrT);
				return m_s_p_Instance->m_p_Object;
			}
			m_s_Mutex.unlock();
		}
		return m_s_p_Instance->m_p_Object;
	}
} // namespace CSDesignMode

#endif 



