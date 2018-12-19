/**********************************************************
 * \file SqlOperator.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "sql_operator.h"

#include "sql_define.h"
#include "cppconn/connection.h"
#include "cppconn/statement.h"
#include "cppconn/resultset.h"
#include "cppconn/resultset_metadata.h"

using namespace std;
using namespace boost;
using namespace sql;
using namespace frpublic;
//
// SqlOperator
//
namespace frsql{

	string convertCond(const map<string, string> &sqlWhere){
		string sqlWhereTmp;
		for(auto& item : sqlWhere){
			sqlWhereTmp += " and " + item.first + " = " + QuotedStr(item.second);
		}
		return sqlWhereTmp;
	}
	string BuildQueryCmd(const std::string& table_name, const std::map<std::string, std::string>& condition){
		return "select * from " + table_name + " where 1 = 1" + convertCond(condition);
	}

	string BuildCreateCmd(const std::string& table_name, const std::map<std::string, std::string>& fields){
		auto GetFirst = [](const map<string, string>& map_info, string& tag)->string{
			string str;
			for(auto item : map_info){
				str += tag + item.first; tag = ",";
			}
			return str;
		};
		auto GetSecond = [](const map<string, string>& map_info, string& tag)->string{
			string str;
			for(auto item : map_info){
				str += tag + QuotedStr(item.second); tag = ",";
			}
			return str;
		};

		string tag1, tag2;
		return "insert into " + table_name + " (" + GetFirst(fields, tag1) + ") values (" + GetSecond(fields, tag2) + ")"; 
	}

	string BuildUpdateCmd(const std::string& table_name, const std::map<std::string, std::string>& condition, const std::map<std::string, std::string>& fields){
		string cmd_update = "update " + table_name + " set ";
		string tag("");
		for(auto& field_item : fields){
			cmd_update += tag + field_item.first + " = " + QuotedStr(field_item.second);
			tag = ", ";
		}
		cmd_update += " where 1=1 " + convertCond(condition);
		return cmd_update;
	}

	string BuildDeleteCmd(const std::string& table_name, const std::map<std::string, std::string>& condition){
		return "delete from " + table_name + " where 1=1" + convertCond(condition);
	}


	SqlOperator::SqlOperator(const string &_host, const int &_port, const string &_user, const string &_pwd, const string &_dbName, 
			const int _readTimeout, const int _writeTimeout)
		:connection_(NULL),
		 statement_(NULL)
	{
		SetConnection(_host, _port, _user, _pwd, _dbName, _readTimeout, _writeTimeout);
	}
	SqlOperator::~SqlOperator(){ ; }

	void SqlOperator::SetConnection(const string &host, const int &port, const string &user, const string &pwd, const string &dbName, 
			const int &_readTimeout, const int &_writeTimeout ){
		sql_options_.clear();
		sql_options_.insert(pair< sql::SQLString, ConnectPropertyVal >("hostName", host));
		sql_options_.insert(pair< sql::SQLString, ConnectPropertyVal >("password", pwd));
		sql_options_.insert(pair< sql::SQLString, ConnectPropertyVal >("userName", user));
		sql_options_.insert(pair< sql::SQLString, ConnectPropertyVal >("port", port));
		sql_options_.insert(pair< sql::SQLString, ConnectPropertyVal >("schema", dbName));
		sql_options_.insert(pair< sql::SQLString, ConnectPropertyVal >("OPT_READ_TIMEOUT", _readTimeout));
		sql_options_.insert(pair< sql::SQLString, ConnectPropertyVal >("OPT_WRITE_TIMEOUT", _writeTimeout));
	}

	bool SqlOperator::ExecQuery(const string &sqlCmd, std::map<std::string, std::string>& datas){
		try{
			if(!CheckConnectAndReconnect()){ return false; }

			scoped_ptr<sql::ResultSet> pResult(statement_->executeQuery(sqlCmd));

			sql::ResultSetMetaData* pResultMetaData = pResult->getMetaData();
			if(pResult != NULL && pResultMetaData != NULL && pResult->next()){
				for(unsigned int columnIndex = 1; columnIndex <= pResultMetaData->getColumnCount(); ++columnIndex){
					datas.insert(make_pair(pResultMetaData->getColumnName(columnIndex), pResult->getString(columnIndex)));
				}
			}
			return true;
		}
		catch(sql::SQLException &e){
			SQL_DEBUG_E("query operator error. state [%d] errno [%d] what [%s] ", e.getSQLState().c_str(), e.getErrorCode(), e.what());
			return false;
		}
	}

	bool SqlOperator::ExecQuery(const string &sqlCmd, std::vector<std::map<std::string, std::string> > &datas){
		try{
			if(!CheckConnectAndReconnect()){ return false; }

			datas.clear();
			scoped_ptr<sql::ResultSet> pResult(statement_->executeQuery(sqlCmd));

			sql::ResultSetMetaData* pResultMetaData = pResult->getMetaData();
			while(pResult != NULL && pResultMetaData != NULL && pResult->next()){
				map<string, string> sqlData;
				for(unsigned int columnIndex = 1; columnIndex <= pResultMetaData->getColumnCount(); ++columnIndex){
					sqlData.insert(make_pair(pResultMetaData->getColumnName(columnIndex), pResult->getString(columnIndex)));
				}
				datas.push_back(sqlData);
			}
			return true;
		}
		catch(sql::SQLException &e){
			SQL_DEBUG_E("query operator error. state [%d] errno [%d] what [%s] ", e.getSQLState().c_str(), e.getErrorCode(), e.what());
			return false;
		}
	}

	bool SqlOperator::ExecUpdate(const std::string &sqlCmd, int &updateCount){
		try{
			if(!CheckConnectAndReconnect()){ return false; }

			updateCount = statement_->executeUpdate(sqlCmd);
			return true;
		}
		catch(sql::SQLException &e){
			updateCount = 0;
			SQL_DEBUG_E("query operator error. state [%d] errno [%d] what [%s] ", e.getSQLState().c_str(), e.getErrorCode(), e.what());
			return false;
		}
	}

	bool SqlOperator::IsExist(const std::string& table_name, const std::map<std::string, std::string>& condition){
		map<string, string> fields;
		QueryItem(table_name, condition, fields);
		return !fields.empty();
	}

	bool SqlOperator::QueryItem(const std::string& table_name, const std::map<std::string, std::string>& condition, std::map<std::string, std::string>& fields){
		fields.clear();
		if(ExecQuery("select * from " + table_name + " where 1 = 1" + convertCond(condition), fields)){
			return !fields.empty();
		}
		return false;
	}

	bool SqlOperator::QueryItems(const std::string& table_name, const std::map<std::string, std::string>& condition, std::vector<std::map<std::string, std::string> >& fields){
		fields.clear();
		if(ExecQuery("select * from " + table_name + " where 1 = 1" + convertCond(condition), fields)){
			return !fields.empty();
		}
		return false;
	}

	bool SqlOperator::CreateItem(const std::string& table_name, const std::map<std::string, std::string>& fields){
		int update_count(0);
		if(!ExecUpdate(BuildCreateCmd(table_name, fields), update_count)){
			SQL_DEBUG_E("创建item 失败 table[%s]", table_name.c_str());
			return false;
		}
		return update_count >= 1;
	}

	bool SqlOperator::UpdateItem(const std::string& table_name, const std::map<std::string, std::string>& condition, const std::map<std::string, std::string>& fields){
		int update_count(0);
		if(!ExecUpdate(BuildUpdateCmd(table_name, condition, fields), update_count)){
			SQL_DEBUG_E("update item failed. table name [%s]", table_name.c_str());
			return false;
		}
		return update_count >= 1;
	}

	bool SqlOperator::DeleteItem(const std::string& table_name, const std::map<std::string, std::string>& condition){
		int update_count(0);
		if(!ExecUpdate(BuildDeleteCmd(table_name, condition), update_count)){
			SQL_DEBUG_E("update item failed. table name [%s]", table_name.c_str());
			return false;
		}
		return update_count >= 1;
	}

	bool SqlOperator::CheckConnectAndReconnect(){
		if(statement_ == NULL || connection_ == NULL){
			lock_guard<mutex> local_lock(mutex_connect_);
			if(connection_ == NULL){
				connection_ = boost::shared_ptr<sql::Connection>(sql::mysql::get_mysql_driver_instance()->connect(sql_options_));
				if(connection_ == NULL){
					SQL_DEBUG_E("create sql connect failed.");
					return false;
				}
			}

			if(statement_ == NULL){
				statement_ = boost::shared_ptr<sql::Statement>(connection_->createStatement());
				if(connection_ == NULL){
					SQL_DEBUG_E("create sql statement failed.");
					return false;
				}
			}
		}
		else if(!statement_->getConnection()->reconnect()){
			SQL_DEBUG_E("socket is disconnect and reconnect is falied.");
			return false;
		}
		return true;
	}
} 


