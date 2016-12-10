/**********************************************************
 *  \file tcp_server_thread.h
 *  \brief	tcp服务主类。
 *  \note	结构介绍：服务类由三部分组成：触发器，线程处理和回调类，信息缓存和处理类。
 *				* 触发器：根据socket返回各类触发消息，并唤醒回调线程。
 *					* 触发器初始化线程池。
 *				* 线程处理和回调类：生成线程，消息处理成功后进行回调。
 *					* 事件的所有处理都在该线程中，但逻辑都在其他类中，只是使用这个线程运行。
 *					* 该线程处理消息后，还会调用回调函数。
 *					* 处理完回调后会自动休眠。
 *				* 处理类：包含 1.未处理的消息（缓存） 2.实际的处理逻辑。
 *	\note	缺陷：
 *				* 设计结构不算完美，但使用应该问题不大。
 *				* 性能未测试，并且部分结构为了加快速度性能无法保证最优。
 *	\note	使用注意点：
//				* 对已经断开链接的socket发送消息，有极小的概率导致程序崩溃。
 *	\note	修改源码注意点：
 *				* 该设计方式核心是处理线程可以进行socket级别的锁，这样可以保证不同的连接之间的效率不因为锁相互影响。所以不要在socket更高级别加锁。
 *				* 修订要注意 send 由外部其他线程调用，在修改内部代码时，要关注此点（外部对socket的调用是无法预知的，例如disconnect后，逻辑线程依然可能调用send进行发送。）
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _tcp_server_H
#define _tcp_server_H

#include <iostream>
#include "tcp_link.h"
#include "fr_public/pub_memory.h"
#include "fr_public/pub_thread.h"
#include "tcp_server_thread.h"
#include "fr_template/single_mode.hpp"

//! \brief	tcp	服务类
//! \note	使用
//				1 继承此类，重载相应的回调函数。
//					* 按需实现。
//					* 初始化：缓存大小。
//				2 listen.
//! \note	职责：
//				* 监听端口。
//				* 事件触发和线程唤醒。
//				* 接受链接和断开链接
//				* 支持继承形式的回调函数.(on开头的虚方法)
//! \note	细节：
//				* 多线程模式：主线程发出，子线程进行回调和发送。
//				* 通常应用层会增加包头的消息内容长度。请注意和缓存长度进行匹配。
//				* 对带外数据暂不涉及：代码编写中没有考虑带外数据。
//! \note	其他：
//				* 大文件的传输，建议直接使用sftp或其他已成熟的程序。本类不会考虑超过大数据块的情况（设计时以2^16为准）的情况。
//					* 如果64K不足，可以进行包头扩展。但需要注意，这样也会自动的扩展缓存。导致程序对内存的需求扩大。
//				* 暂时，不考虑动态链接库。后期有需要再实现。
//!				* 处理队列轮训查找空闲线程，此处的效率"可能"存在问题。
//! \note	缺陷 or BUG
//				* 当读写缓存溢出时，会直接丢弃后来的数据包。同时会打印日志。
//					* send暂时没有失败的方式。因为缓存大小判断是到执行线程判断的。
//! \note	使用epoll做触发回调，后期会考虑增加window的IOCP(可能性不大)
//! \todo	处理队列轮训查找空闲线程，此处的效率"可能"存在问题。
class FrTcpServer : public FrTcpLinker{
	public:
		FrTcpServer(uint32 threadNum = 100, uint32 _maxBufferSize = 0xFFFF);
		~FrTcpServer();
	public:
		//! \brief	监听端口
		//! \note	不能监听多端口。如果要重新监听，需要stop再listen，
		virtual bool run(const std::string &ip, unsigned int port);

		//! \brief	断开某个连接。
		//! \todo	简单版本。后续视情况增加。
		bool disconnect(Socket socket);
	protected:
		//! \brief	
		virtual void activeRecv(Socket socket);
	private:
		//! \biref	处理断开/错误请求。
		virtual void dealConnect(Socket socket);
	private:
		Socket m_ListenSocket;
};
typedef DesignMode::SingleMode<FrTcpServer> SingleFrTcpServer;

#endif


