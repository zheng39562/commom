/**********************************************************
 *  \!file net_test.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _net_test_H
#define _net_test_H

#include "common_define.h"

//! \note	用于network的测试。
//! \note	单元测试，回归测试都可用。
//! \note	该代码可读性不佳，仅用于测试，可作为了解参考。
//! \note	测试方法。将该函数在main函数中调用。
//!				服务端输入参数server，客户端输入参数client.
//!				测试该功能需要启动2个进程。

//! \note	该命名空间仅用于测试network。任何其他功能都不应该放在该空间内。
namespace NetworkTest{
	//! \brief	总测试接口：外部进行回归测试直接使用该接口即可。
	//! \note
	void doTest(const std::string &arg);

	//void libevent_test(const std::string &arg);

	//! \brief	连通性测试。
	void doLink(const std::string &arg);

	struct DataStruct{
		int i;
		double d;
		char c;
	};
	class DataClass{
		public:
			int i;
			double d;
			char c;
	};
	//! \brief	不包含字符串的结构体和类是否能够直接通过memcpy进行赋值的测试。
	void doMemCpy(const std::string &arg);

	//! \brief	数据包准确性测试，以及10W个40-50B的包的性能测试(会打印对应的时间)。
	//! \note	数据统计存储在客户端和服务端。
	//! \note	性能测试如果需要考虑tcp传输时间，则需要有对应的2台机器。
	//! \note	
	void doPerformance(const std::string &arg);
}

#endif 

