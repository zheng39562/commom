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
#include "network/socket_simple.h"
#include "tool/string_util.h"

using namespace std;
using namespace Universal;
using namespace MyTest;

int main( int agrc, char **argv ){
	if( agrc >= 2 ){
		string option( argv[1] );
		string ip( "127.0.0.1" );
		string port( "6000" );

		if( option == "c" || option == "client" || option == "-c" ){
			cout << " run socket client. " << endl;
			socket_client( ip, Inside::strToInt(port) );
		}
		else{
			cout << " run socket server. " << endl;
			socket_server( ip, Inside::strToInt(port) );
		}
	}
	else{
		cout << " param size is wrong. size [" << agrc << "]" << endl;
	}
	return 0;
}

