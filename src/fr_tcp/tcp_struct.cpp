/**********************************************************
 *  \!file tcp_struct.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "tcp_struct.h"

using namespace Universal;

const Socket UNKNOW_SOCKET = -1;

FrTcpCache::FrTcpCache(Socket _socket, uint32 _maxBufferSize)
	:socket(_socket),
	 connect(true),
	 bufferWrite(),
	 bufferRead(),
	 bufferTmp(),
	 mutexRead(),
	 mutexWrite()
{ 
	bufferTmp.reserve(_maxBufferSize);
	bufferTmp.setMaxLimit(_maxBufferSize);
	bufferWrite.setMaxLimit(_maxBufferSize);
	bufferRead.setMaxLimit(_maxBufferSize);
}

// ***************************************************
PushMsg::PushMsg(Socket _socket, eSocketEventType _eventType)
	:socket(_socket),
	 pBinary(),
	 eventType(_eventType)
{
	;
}
PushMsg::PushMsg(Socket _socket, eSocketEventType _eventType, Universal::BinaryMemoryPtr _pBinary)
	:socket(_socket),
	 pBinary(_pBinary),
	 eventType(_eventType)
{
	;
}
PushMsg::PushMsg(const PushMsg &ref){
	socket = ref.socket;
	pBinary = ref.pBinary;
	eventType = ref.eventType;
}
PushMsg& PushMsg::operator=(const PushMsg &ref){
	socket = ref.socket;
	pBinary = ref.pBinary;
	eventType = ref.eventType;

	return *this;
}

