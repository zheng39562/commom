#include "sql_example.h"

#include <iostream>

#include "sql_operator_pool.h"

using namespace std;
using namespace frsql;

int main(){
	SqlConnectOption sql_option;
	sql_option.host = "47.110.229.226";
	sql_option.port = 3306;
	sql_option.user = "zjm";
	sql_option.pwd = "123456aaa";
	sql_option.db_name = "user_manager";
	sql_option.read_timeout = 5000;
	sql_option.write_timeout = 5000;
	SqlOprtPool oprt_pool("sql_name1", sql_option, 10);
	
	map<string, string> condition;
	vector<map<string, string>> fields;
	if(oprt_pool.QueryItems("user_info", condition, fields)){
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


