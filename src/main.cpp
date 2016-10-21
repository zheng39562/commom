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
#include <iostream>

#include "common/c_string.h"
#include "common/c_json.h"

using namespace Universal;

int main( int agrc, char **argv ){
	/*
	for(int i =0; i<agrc; ++i){
		cout << argv[i] << endl;
	}
	if(agrc = 3){
		NetworkTest::doTest(argv[1], argv[2]);
	}
	if(agrc = 4){
		NetworkTest::doTest(argv[1], argv[2], argv[3]);
	}
	else{
		cout << "parameter error" << endl;
	}
	*/

	string s = "{\"key\":1}";

	Json::Value jsValue;
	if(JsonTool::parseJs(s, jsValue)){
		cout << jsValue.toStyledString() << endl;
	}

	return 0;
}

