/**********************************************************
 * \file main.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "common_define.h"

#include <iostream>
#include "template_function.hpp"
#include "sql_operator.h"
#include "boost/scoped_ptr.hpp"

using namespace std;
using namespace Universal;

int main( int agrc, char **argv ){
	/*
	boost::scoped_ptr<SqlOperator> pSqlOperator( new SqlOperator( "192.168.1.235", 3306, "igr_all", "igrow123", "svr_log", 50 ) );

	TableDatas datas;

	if( pSqlOperator->execQuery( "select servname as servname, substring( time, 1, 10 ) as time, sum(usetime) as usetimecount, count(1) as usetimes, sum(usetime) / count(1) as averagetime from log_analyzeinfo where 1=1 and unix_timestamp(time) >= 1455638400 and unix_timestamp(time) <= 1455811199 group by substring( time, 1, 10 ), servname limit 0,20", datas ) ){
		cout << "success" << endl;
	}
	else{
		cout << "failure" << endl;
	}
	*/

	boost::shared_ptr<int> ptr( new int(4) );
	cout << ptr << endl;
	cout << *ptr << endl;

	return 0;
}

