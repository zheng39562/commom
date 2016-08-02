/**********************************************************
 * \file http_server.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "http_server.h"

#include "single_mode.hpp"

namespace HttpServerSpace{
	//!	\brief	http服务类。
	//! \note	可以创建该对象来启动一个http监听服务。	
	//! \note	未设置单例模式，但注意使用时不应该拥有多个该对象。 建议：和singleMode模板类一起使用。
	//! \todo	线程池类未完成，暂使用单线程方式。		
	class HttpServer{
		public:
			HttpServer();
			~HttpServer();
		private:
		public:
			bool start( const string &ip, const long &port );
	};
	typedef DesignMode::SingleMode< HttpServer > SingleHttpServer;
}
