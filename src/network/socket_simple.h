/**********************************************************
 *  \!file socket_simple.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _socket_simple_H
#define _socket_simple_H

#include <string>

//! \brief	仅用于学习测试，以及长时间不接触的备忘。
//! \note	如需要使用对应功能，建议使用其他封装完整的类。
//! \note	注意不能直接C+Z的方式结束。该方式不能真正关闭套接字。可能会导致端口被占用而无法再次使用。
namespace MyTest{
	//! \note	包含：一次链接行为，每发送一个信息需要等待对应返回后，才可以发送第二次。
	//! \note	字符"end" 被当作结束标志。
	void socket_client( const std::string &ip, const int &port );
	//! \note	每接收一个消息，会将消息内容自动返回。
	//! \note	当链接的客户端被关闭时 服务会自动关闭。
	void socket_server( const std::string &ip, const int &port );
}

#endif 

