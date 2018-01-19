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

#include <memory>
#include <thread>
#include <mutex>

#include "mysql_driver.h"
#include "mysql_connection.h"

namespace frsql{
	//! \brief	sql条件转换函数。
	std::string convertCond(const std::map<std::string, std::string> &sqlWhere);
	std::string BuildQueryCmd(const std::string& table_name, const std::map<std::string, std::string>& condition);
	std::string BuildCreateCmd(const std::string& table_name, const std::map<std::string, std::string>& fields);
	std::string BuildUpdateCmd(const std::string& table_name, const std::map<std::string, std::string>& condition, const std::map<std::string, std::string>& fields);
	std::string BuildDeleteCmd(const std::string& table_name, const std::map<std::string, std::string>& condition);

	//! \brief	数据库操作类，并且为非线程安全类。
	//! \note	提供两种设置连接的方式：构造函数 和 设定函数。
	//!			注意：未成功连接数据库时，任何sql操作都会返回异常。
	//! \note	设计理念不支持跨库查询。但保留部分跨库接口: execSQL
	//! \todo	陆续支持所有mysql相关操作集。
	//! \todo	优先mysql。后续会增加其他数据库标准操作。
	class SqlOperator{
		public:
			//! \param[in] _readTimeout  unit is ms
			//! \param[in] _writeTimeout  unit is ms
			SqlOperator(const std::string &_host, const int &_port, const std::string &_user, const std::string &_pwd, const std::string &_dbName, 
					const int _readTimeout, const int _writeTimeout);
			~SqlOperator();
		public:
			//! \param[in] datas 仅对拥有返回值的select语句有效。其他复合语句返回空。
			bool ExecQuery(const std::string &sqlCmd, std::map<std::string, std::string> &datas);
			//! \param[in] datas 仅对拥有返回值的select语句有效。其他复合语句返回空。
			bool ExecQuery(const std::string &sqlCmd, std::vector<std::map<std::string, std::string> > &datas);
			//! \brief	返回影响条数
			bool ExecUpdate(const std::string &sqlCmd, int &updateCount);

			//! \brief	
			bool IsExist(const std::string& table_name, const std::map<std::string, std::string>& condition);
			//! \brief	
			//! \retval true 表示查询到数据。 false 可能是查询失败，也可能是查询不到数据.
			bool QueryItem(const std::string& table_name, const std::map<std::string, std::string>& condition, std::map<std::string, std::string>& fields);
			bool QueryItems(const std::string& table_name, const std::map<std::string, std::string>& condition, std::vector<std::map<std::string, std::string> >& fields);
			//! \brief	
			//! \retval true create success。 false create falied.
			bool CreateItem(const std::string& table_name, const std::map<std::string, std::string>& fields);
			//! \brief	
			bool UpdateItem(const std::string& table_name, const std::map<std::string, std::string>& condition, const std::map<std::string, std::string>& fields);
			//! \brief	
			bool DeleteItem(const std::string& table_name, const std::map<std::string, std::string>& condition);
		private:
			//! \brief	设置数据库链接。
			void SetConnection(const std::string &host, const int &port, const std::string &user, const std::string &pwd, const std::string &dbName, 
				const int &_readTimeout, const int &_writeTimeout);
			//! \brief	检查连接状态。当连接断开时，尝试自动重连
			bool CheckConnectAndReconnect();
		private:
			boost::shared_ptr<sql::Connection>			connection_;		//! 
			boost::shared_ptr<sql::Statement>			statement_;			//! 
			sql::ConnectOptionsMap						sql_options_;
			std::mutex									mutex_connect_;
	};
} //  namespace universal{


#endif 

