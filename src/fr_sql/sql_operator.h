/**********************************************************
 * \file SqlOperator.h
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _SqlOperator_H
#define _SqlOperator_H

#include "mysql_driver.h"
#include "mysql_connection.h"
#include "boost/shared_ptr.hpp"

#include "fr_public/pub_define.h"

namespace SqlSpace{

	/*
	//! \brief	数据库的管理器：获取数据库操作指针。
	//! \note	采用线程池方案。
	class SqlOprtManager{
		public:
			SqlOprtManager()=default;
			~SqlOprtManager()=default;
	};
	*/

	typedef std::pair<std::string, std::string>	TableFieldData;
	typedef std::map<std::string, std::string> TableData;
	typedef std::vector<TableData> TableDatas;
	typedef std::string	SQLWhere;
	//! \brief	数据库操作类，并且为非线程安全类。
	//! \note	提供两种设置连接的方式：构造函数 和 设定函数。
	//!			注意：未成功连接数据库时，任何sql操作都会返回异常。
	//! \note	设计理念不支持跨库查询。但保留部分跨库接口: execSQL
	//! \todo	陆续支持所有mysql相关操作集。
	//! \todo	优先mysql。后续会增加其他数据库标准操作。
	class SqlOperator{
		public:
			SqlOperator( const std::string &_host, const int &_port, const std::string &_user, const std::string &_pwd, const std::string &_dbName );
			SqlOperator( const std::string &_host, const int &_port, const std::string &_user, const std::string &_pwd, const std::string &_dbName, const int &_timeout );
			SqlOperator( const std::string &_host, const int &_port, const std::string &_user, const std::string &_pwd, const std::string &_dbName, 
					const int &_readTimeout, const int &_writeTimeout );
			~SqlOperator();
		private:
			boost::shared_ptr<sql::Connection>			m_p_Connection;		//! 
			boost::shared_ptr<sql::Statement>			m_p_Statement;		//! 
		public:
			//! \brief	设置数据库链接。
			bool setConnection( const std::string &host, const int &port, const std::string &user, const std::string &pwd, const std::string &dbName, 
				const int &_readTimeout, const int &_writeTimeout  );

			//! \brief	sql条件转换函数。
			SQLWhere convertCond( const std::map<std::string, std::string> &sqlWhere );

			//! \brief	执行SQL语句：可跨库查询。
			//! \note	不限制跨库语句。但需要当前用户拥有对应权限。
			bool execSQL( const std::string &sqlCmd );
			//! \param[in] datas 仅对拥有返回值的select语句有效。其他复合语句返回空。
			bool execQuery( const std::string &sqlCmd, TableDatas &datas );
	};
} //  namespace Universal{


#endif 

