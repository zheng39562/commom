/**********************************************************
 * \file sql_manager.cpp
 * \brief 
 * \note	ע�����
 *			1,���еĳ�Ա�����е�ͬ�������ĺ�����ȫ��ͬ������ע������һ���������������������ظ�ע�͡������Ĳ������岻ͬʱ�������ע�⡣
 *			2,��1���Ĺ���ͬ�������ڷ���ֵ�ĺ��塣
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "sql_operator_pool.h"

#include "frpublic/pub_timer.h"

using namespace std;
using namespace frsql;
using namespace frpublic;

SqlOprtPool::SqlOprtPool()
	:sql_operator_pool_(),
	 read_timeout_(1),
	 write_timeout_(1),
	 mutex_sql_operator_()
{
}

SqlOprtPool::~SqlOprtPool(){
	;
}

void SqlOprtPool::Initialize(const string &host, const int &port, const string &user, const string &pwd, const string &db_name, 
	const int read_timeout, const int write_timeout, int pool_size){
	 read_timeout_ = read_timeout;
	 write_timeout_ = write_timeout;
	for(int index = 0; index < pool_size; ++index){
		sql_operator_pool_.push_back(SqlOperatorPtr(new SqlOperator(host, port, user, pwd, db_name, read_timeout, write_timeout)));
	}
}

bool SqlOprtPool::ExecQuery(const string &sql_cmd, map<string, string> &datas){
	SqlOperatorPtr pSqlOperator = popSqlOprtPtr(read_timeout_);
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->ExecQuery(sql_cmd, datas);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::ExecQuery(const string &sql_cmd, vector<map<string, string> > &datas){
	SqlOperatorPtr pSqlOperator = popSqlOprtPtr(read_timeout_);
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->ExecQuery(sql_cmd, datas);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::ExecUpdate(const string &sql_cmd, int &updateCount){
	SqlOperatorPtr pSqlOperator = popSqlOprtPtr(write_timeout_);
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->ExecUpdate(sql_cmd, updateCount);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::IsExist(const string& table_name, const map<string, string>& condition){
	SqlOperatorPtr pSqlOperator = popSqlOprtPtr(write_timeout_);
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->IsExist(table_name, condition);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::QueryItem(const string& table_name, const map<string, string>& condition, map<string, string>& fields){
	SqlOperatorPtr pSqlOperator = popSqlOprtPtr(write_timeout_);
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->QueryItem(table_name, condition, fields);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::QueryItems(const string& table_name, const map<string, string>& condition, vector<map<string, string> >& fields){
	SqlOperatorPtr pSqlOperator = popSqlOprtPtr(write_timeout_);
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->QueryItems(table_name, condition, fields);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::CreateItem(const string& table_name, const map<string, string>& fields){
	SqlOperatorPtr pSqlOperator = popSqlOprtPtr(write_timeout_);
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->CreateItem(table_name, fields);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::UpdateItem(const string& table_name, const map<string, string>& condition, const map<string, string>& fields){
	SqlOperatorPtr pSqlOperator = popSqlOprtPtr(write_timeout_);
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->UpdateItem(table_name, condition, fields);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

bool SqlOprtPool::DeleteItem(const string& table_name, const map<string, string>& condition){
	SqlOperatorPtr pSqlOperator = popSqlOprtPtr(write_timeout_);
	bool bRet(false);
	if(pSqlOperator != NULL){
		bRet = pSqlOperator->DeleteItem(table_name, condition);
		pushSqlOprtPtr(pSqlOperator);
	}
	return bRet;
}

SqlOperatorPtr SqlOprtPool::popSqlOprtPtr(int timeout){
	while(--timeout >= 0){
		{
			lock_guard<mutex> lock(mutex_sql_operator_);
			if(!sql_operator_pool_.empty()){
				SqlOperatorPtr pSqlOperator = *sql_operator_pool_.begin();
				sql_operator_pool_.erase(sql_operator_pool_.begin());
				return pSqlOperator;
			}
		}
		FrSleep(1);
	}
	return SqlOperatorPtr();
}

void SqlOprtPool::pushSqlOprtPtr(SqlOperatorPtr pSqlOperator){
	lock_guard<mutex> lock(mutex_sql_operator_);
	sql_operator_pool_.push_back(pSqlOperator);
}

void ConnectSql(const string &host, const int &port, const string &user, const string &pwd, const string &db_name, const int read_timeout, const int write_timeout, int pool_size){
	SingleSqlOprtPool::GetInstance()->Initialize(host, port, user, pwd, db_name, read_timeout, write_timeout, pool_size);
}

shared_ptr<SqlOprtPool> GetSqlOprtPool(){
	return SingleSqlOprtPool::GetInstance();
}

