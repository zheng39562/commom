/**********************************************************
 *  \file example.cpp
 *  \brief
 *  \note	注意事项： 
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#include "example.h"

#include "pub_log.h"
#include "pub_tool.h"

int main(){
	universal::SingleLogServer::GetInstance()->InitLog("./log", 10000);
	DEBUG_D("123");

	while(1){
		universal::FrSleep(3000);
	}
	return 0;
}

