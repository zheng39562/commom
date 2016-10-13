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

#include "network/net_test.h"
#include "common/common_memory.h"

using namespace Universal;

int main( int agrc, char **argv ){
	for(int i =0; i<agrc; ++i){
		cout << argv[i] << endl;
	}
	if(agrc >= 2){
		NetworkTest::doTest(argv[1]);
	}
	else{
		cout << "parameter error" << endl;
	}
	/*
	string s1 = "1213141";
	BinaryMemory b1, b2;
	b1.setBuffer(s1.c_str(), s1.size());
	//b2.addBuffer(b1.getBuffer(), b1.getBufferSize());
	b2 = b2 + b1;

	cout << string((char*)b2.getBuffer(), b2.getBufferSize()) << endl;
	*/

	return 0;
}

