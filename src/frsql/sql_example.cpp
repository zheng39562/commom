#include "sql_example.h"

#include <iostream>

#include "sql_operator_pool.h"

using namespace std;

int main(){
	string host("47.52.69.59");
	int port(3306);
	string user("zjm");
	string pwd("123456aaa");
	string db_name("test");
	int read_timeout(5000);
	int write_timeout(5000);
	int pool_size(10);
	ConnectSql(host, port, user, pwd, db_name, read_timeout, write_timeout, pool_size);

	std::shared_ptr<fr_sql::SqlOprtPool> oprt_pool = GetSqlOprtPool();
	
	map<string, string> condition;
	vector<map<string, string>> fields;
	if(oprt_pool->QueryItems("test_table", condition, fields)){
		for(auto& field : fields){
			for(auto& field_item : field){
				cout << "key : [" << field_item.first << "] vlaue : [" << field_item.second << "]" << endl;
			}
		}
	}
	else{
		cout << "query db failed." << endl;
	}

	return 0;
}


