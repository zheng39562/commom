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
#include "CSDefine.h"

#include <iostream>
#include "CSTemplateFunc.hpp"

using namespace std;
using namespace Universal;

int main( int agrc, char **argv ){

	map< string, string > mapVar;
	mapVar.insert( pair<string, string>("1", "2") );
	string value;
	if( mapFind( mapVar, string("1"), value ) ){
		cout << value << endl;
	}
	else{
		cout << " not found " << endl;
	}


	vector<long> vecVar;
	vecVar.push_back(1);
	vecVar.push_back(2);
	vecVar.push_back(3);
	cout << vectorAt( vecVar, 0 ) << endl;
	cout << vectorAt( vecVar, 10 ) << endl;
	



	return 0;
}

