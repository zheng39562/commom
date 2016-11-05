/**********************************************************
 * \file sql_manager.h
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _sql_manager_H
#define _sql_manager_H

#include "boost/shared_ptr.hpp"

#include "sql_operator.h"

namespace SqlSpace{
	typedef std::vector< boost::shared_ptr<SqlOperator> > SqlOprtPool;
	//! \brief	数据库的管理器：获取数据库操作指针。
	//! \note	采用线程池方案。
	//! \note	 
	class SqlOprtManager{
		public:
			SqlOprtManager()=default;
			~SqlOprtManager()=default;
		private:

		public:
			boost::shared_ptr<SqlOperator> getSqlOprtPtr();
			boost::shared_ptr<SqlOperator> getSqlOprtPtr( const std::string &sqlHandle );
	};
} // namespace Universal{

#endif 

