/**********************************************************
 * \file sql_operator_pool.h
 *  
 * 	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef sql_operator_pool_H
#define sql_operator_pool_H

#include <mutex>
#include "sql_operator.h"
#include "frtemplate/single_mode.hpp"

namespace frsql{
	// 数据库连接池
	// 用法详情：sql_example.cpp
	class SqlOprtPool{
		public:
			SqlOprtPool(const std::string& name, const SqlConnectOption& option, int pool_size);
			~SqlOprtPool();
		public:
			// 	查询语句使用
			// 	该函数在连接池为空时会锁住线程。通过条件变量进行唤醒。
			//			锁线程的最大时间通常为超时时间(readTimerout)，可能略超出。
			// \param[in] datas 仅对拥有返回值的select语句有效。其他复合语句返回空。
			bool ExecQuery(const std::string &sql_cmd, std::map<std::string, std::string> &datas);
			bool ExecQuery(const std::string &sql_cmd, std::vector<std::map<std::string, std::string> > &datas);
			// 	在update/insert/delete以及操作数据的语句中使用.
			// 	该函数在连接池为空时会锁住线程。通过条件变量进行唤醒。
			//			锁线程的最大时间通常为超时时间(writeTimerout)，可能略超出。
			// \retval	返回影响条数
			bool ExecUpdate(const std::string &sql_cmd, int &updateCount);

			bool IsExist(const std::string& table_name, const std::map<std::string, std::string>& condition);
			bool QueryItem(const std::string& table_name, const std::map<std::string, std::string>& condition, std::map<std::string, std::string>& fields);
			bool QueryItems(const std::string& table_name, const std::map<std::string, std::string>& condition, 
					std::vector<std::map<std::string, std::string> >& fields);
			bool CreateItem(const std::string& table_name, const std::map<std::string, std::string>& fields);
			bool UpdateItem(const std::string& table_name, const std::map<std::string, std::string>& condition, 
					const std::map<std::string, std::string>& fields);
			bool DeleteItem(const std::string& table_name, const std::map<std::string, std::string>& condition);

			inline const std::string& name()const{ return name_; }
		private:
			 SqlOperator* popSqlOprtPtr();
			 void pushSqlOprtPtr(SqlOperator* pSqlOperator);
		private:
			const std::string name_;
			std::list<SqlOperator*> sql_operator_pool_;
			std::mutex mutex_sql_operator_;
	};
}

#endif 

