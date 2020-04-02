/**********************************************************
 * \file sql_manager.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "sql_operator_pool.h"

#include "frpublic/pub_timer.h"

using namespace std;
using namespace frsql;
using namespace frpublic;


bool SqlOprtPool::ExecQuery(const string &sql_cmd, map<string, string> &datas){
	SqlOperator* pSqlOperator = popSqlOprtPtr();
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->ExecQuery(sql_cmd, datas);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::ExecQuery(const string &sql_cmd, vector<map<string, string> > &datas){
	SqlOperator* pSqlOperator = popSqlOprtPtr();
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->ExecQuery(sql_cmd, datas);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::ExecUpdate(const string &sql_cmd, int &updateCount){
	SqlOperator* pSqlOperator = popSqlOprtPtr();
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->ExecUpdate(sql_cmd, updateCount);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::IsExist(const string& table_name, const map<string, string>& condition){
	SqlOperator* pSqlOperator = popSqlOprtPtr();
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->IsExist(table_name, condition);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::QueryItem(const string& table_name, const map<string, string>& condition, map<string, string>& fields){
	SqlOperator* pSqlOperator = popSqlOprtPtr();
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->QueryItem(table_name, condition, fields);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::QueryItems(const string& table_name, const map<string, string>& condition, vector<map<string, string> >& fields){
	SqlOperator* pSqlOperator = popSqlOprtPtr();
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->QueryItems(table_name, condition, fields);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::CreateItem(const string& table_name, const map<string, string>& fields){
	SqlOperator* pSqlOperator = popSqlOprtPtr();
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->CreateItem(table_name, fields);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::UpdateItem(const string& table_name, const map<string, string>& condition, const map<string, string>& fields){
	SqlOperator* pSqlOperator = popSqlOprtPtr();
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->UpdateItem(table_name, condition, fields);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::DeleteItem(const string& table_name, const map<string, string>& condition){
	SqlOperator* pSqlOperator = popSqlOprtPtr();
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->DeleteItem(table_name, condition);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

SqlOprtPool::SqlOprtPool(const std::string& name, const SqlConnectOption& option, int pool_size)
	:name_(name),
	 sql_operator_pool_(),
	 mutex_sql_operator_()
{
	for(int index = 0; index < pool_size; ++index){
		SqlOperator* sql_oprt = new SqlOperator(option);
		if(!sql_oprt->CheckConnectAndReconnect()){
			sql_operator_pool_.clear();
			return ;
		}
		sql_operator_pool_.push_back(sql_oprt);
	}
}

SqlOprtPool::~SqlOprtPool(){
	;
}

SqlOperator* SqlOprtPool::popSqlOprtPtr(){
	lock_guard<mutex> lock(mutex_sql_operator_);
	if(!sql_operator_pool_.empty()){
		SqlOperator* pSqlOperator = *sql_operator_pool_.begin();
		sql_operator_pool_.erase(sql_operator_pool_.begin());
		return pSqlOperator;
	}
	return nullptr;
}

void SqlOprtPool::pushSqlOprtPtr(SqlOperator* pSqlOperator){
	lock_guard<mutex> lock(mutex_sql_operator_);
	sql_operator_pool_.push_back(pSqlOperator);
}


