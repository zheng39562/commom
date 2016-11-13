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

//! \brief	tcp	服务类
//! \note	使用
//				1 初始化对象：建议使用SingleFrTcpServer
//				2 调用setCallBack设置回调函数。
//					* 如果不设置回调，直接listen虽然在运行，但实际上数据直接处理后就被丢弃了没有任何意义。
//				3 listen.
//! \note	职责：
//				* 监听端口。
//				* 保证数据包的完整性：实现包头。
//				* 提供根据socket的读/写缓存区，按照socket进行缓存。
//					* 缓存区大小可设置。当缓存满后，每次可读会产生回调。但不会再从内核中读取数据。
//					* 读缓存：实现peek和fetch两种方式。
//					* 写缓存：写入数据到缓存。实际发送存在延时（由底层触发决定）
//				* 提供回调函数的触发：.连接成功.连接断开.有可读数据.有可写数据.异常事件.
//! \note	细节：
//				* 多线程模式：主线程发出，子线程进行回调和发送。
//				* 缓存最大为：2^16Byte.
//				* 每个进程只支持一个服务类对象。
//! \note	其他：
//				* 通常应用层会增加包头的消息内容长度。请注意和缓存长度进行匹配。
//				* 大文件的传输，建议直接使用sftp或其他已成熟的程序。本类不会考虑超过大数据块的情况（设计时以2^16为准）的情况。
//				* 暂时，不考虑动态链接库。后期有需要再实现。
//				* 处理队列轮训查找空闲线程，此处的效率"可能"存在问题。
//				* 对带外数据暂不涉及：代码编写中没有考虑带外数据。
//! \note	使用epoll做触发回调，后期会考虑增加window的IOCP(可能性不大)
//! \note	如果需要window的tcp库可以参考我基于HPSocket的封装（更建议直接使用HPSocket）
class FrTcpServer : public Universal::FrThread{
	public:
		FrTcpServer(uint32 threadNum = 10);
		~FrTcpServer();
	public:
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
		
		//! \brief	 设置回调函数。
		void setCallBack(fp_connect_cb connect_cb, fp_disconnect_cb disconn_cb, fp_send_cb send_cb, fp_receive_cb receive_cb, void* etc);
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

#endif


