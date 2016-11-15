/**********************************************************
 *  !file tcp_server_thread.h
 *  !brief
 * 
 * !version 
 * * !author zheng39562@163.com
**********************************************************/
#ifndef _tcp_server_H
#define _tcp_server_H

#include <iostream>
#include "tcp_link.h"
#include "fr_public/pub_memory.h"
#include "fr_public/pub_thread.h"
#include "tcp_server_thread.h"
#include "fr_template/single_mode.hpp"

//! \brief	回调事件的返回值。
enum eEventResult{
	eEventResult_OK
};
//! \brief	tcp	服务类
//! \note	使用
//				1 继承此类，重载相应的回调函数。
//					* 按需实现。
//				2 listen.
//! \note	职责：
//				* 监听端口。
//				* 保证数据包的完整性：实现包头。
//				* 提供根据socket的读/写缓存区，按照socket进行缓存。
//					* 当缓存满后，每次可读会产生回调。但不会再从内核中读取数据。
//					* 发送数据实际上是写入数据到缓存。实际发送存在延时（由底层触发决定）
//				* 增加回调函数的继承形式： connect, disconnect, send, recv 
//! \note	细节：
//				* 多线程模式：主线程发出，子线程进行回调和发送。
//				* 单包最大为：2^16Byte.包最大值即缓存最大值。
//! \note	其他：
//				* 通常应用层会增加包头的消息内容长度。请注意和缓存长度进行匹配。
//				* 大文件的传输，建议直接使用sftp或其他已成熟的程序。本类不会考虑超过大数据块的情况（设计时以2^16为准）的情况。
//					* 如果64K不足，可以进行包头扩展。但需要注意，这样也会自动的扩展缓存。导致程序对内存的需求扩大。
//				* 暂时，不考虑动态链接库。后期有需要再实现。
//				* 处理队列轮训查找空闲线程，此处的效率"可能"存在问题。
//				* 对带外数据暂不涉及：代码编写中没有考虑带外数据。
//				* 考虑过外部设置回调，但比较过两种方式后续编写成本 选择由派生类实现的方式。
//					* 主要考虑快速实现。可能一定程度牺牲了灵活性（例如，必须派生该类）
//! \note	使用epoll做触发回调，后期会考虑增加window的IOCP(可能性不大)
//! \note	如果需要window的tcp库可以参考我基于HPSocket的封装（更建议直接使用HPSocket）
//! \todo	还可以增加的回调函数：发送成功，错误事件，服务关闭回调。
//! \todo	关于Socket的map树避免锁的解决方案：
//				* 考虑增加 断开的socket回收机制。 从而避免在socket的map上加锁。
//					* 但延时时间多少合适， 或计数器方式？
//				* 在不考虑外界send的前提下，可以使用计数器来解决recv和send在运行时的erase问题。
//					* 接收到disconnect的线程进行死循环，等待其他recv和send处理结束。如此，可能需要一定的冗余线程，保证大量断开消息时，能有足够的线程池来容纳这些等待断开的链接。
//						* 对disconnect在epoll判断时加上判断.当链接状态为断开时，所有消息都将被丢弃（增加日志），返回false。判断的锁在单socket级别的。
//						* 需要对struct结构进行大改。。坑爹啊！！！
class FrTcpServer : public Universal::FrThread{
	public:
		FrTcpServer(uint32 threadNum = 10);
		~FrTcpServer();
	public:
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		// 回调函数。由派生类自行实现。
		//
		//! \brief	发送成功回调
		virtual eEventResult onSend(Socket socket);
		//! \brief	接收回调
		//! \note	返回数据指针。避免数据的反复copy
		virtual eEventResult onReceive(Socket socket, Universal::BinaryMemoryPtr pBinary);
		//! \brief	链接成功回调(成功)
		virtual eEventResult onConnect(Socket socket);
		//! \brief	链接断开回调(成功)
		virtual eEventResult onDisconnect(Socket socket);
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		
		//! \brief	监听端口
		//! \note	不能监听多端口。如果要重新监听，需要stop再listen，
		bool listen(const std::string &ip, unsigned int port, size_t maxListenNum = 10000);
		//! \brief	关闭所有端口。
		//! \note	将会关闭所有已连接的端口。
		bool stop();

		//! \brief	断开某个连接。
		bool disconnect(Socket socket);

		//! \brief	发送数据。
		//! \note	组播和广播有大致实现相同，不服用send是为了减少锁开销。
		bool send(Socket socket, const Universal::BinaryMemory &pBuffer);
		//! \brief	根据socket群发。
		bool sendToGroup(const std::vector<Socket> &sockets, const Universal::BinaryMemory &binary);
		//! \brief	广播
		bool sendAll(const Universal::BinaryMemory &binary);
	private:
		//! \ntoe	 断开和错误暂时不细分。
		//! \attetion	程序关闭时，没有依次关闭所有连接。不确定底层是否会自动关闭。需要测试。
		//				但从实际使用上，这类问题应该不太大。后续如果有这类问题，再解决。
		virtual void execute();
	private:
		//! \brief	获取空闲的处理线程。
		FrTcpServerThread* getReadyThread();
		//! \brief	处理连接请求。
		void dealConnectReq();
		//! \biref	处理断开/错误请求。
		void dealDisconnectReq(Socket socket);
	private:
		Universal::FrMutex m_SocketMutex;
		TcpCache m_TcpCache;
		Socket m_EpollSocket;
		Socket m_ListenSocket;

		std::list<FrTcpServerThread*> m_ServerThreads;
};
typedef DesignMode::SingleMode<FrTcpServer> SingleFrTcpServer;


#endif


