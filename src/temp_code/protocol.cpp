/**********************************************************
 *  \!file protocol.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "protocol.h"

using namespace Universal;

const unsigned int PROTO_HEAD_PACKET_INDEX = 0;
const unsigned int PROTO_PACKET_SIZE_BYTE = 2;
const unsigned int PROTO_HEAD_VERIFY_INDEX = PROTO_HEAD_PACKET_INDEX + PROTO_PACKET_SIZE_BYTE;
const unsigned int PROTO_VERIFY_SIZE_BYTE = 1;
const unsigned int PROTO_HEAD_SIZE = PROTO_PACKET_SIZE_BYTE + PROTO_VERIFY_SIZE_BYTE;

bool proto_pack(BinaryMemory &unpacket){
	Byte* pHead = new Byte[PROTO_HEAD_SIZE];
	proto_sizeToByte(unpacket.getBufferSize(), pHead);
	// verification
	pHead[PROTO_HEAD_VERIFY_INDEX] = ((unsigned char*)unpacket.getBuffer())[unpacket.getBufferSize() - 1];

	BinaryMemory packetTmp((const void*)pHead, PROTO_HEAD_SIZE);
	unpacket = packetTmp + unpacket;

	if(pHead != NULL){
		delete pHead; pHead = NULL;
	}

	return true;
}

bool proto_unpack(BinaryMemory &packet){
	if(proto_verification(packet) != 0){
		DEBUG_E("验证包失败。");
		return false;
	}

	packet.delBuffer(0, PROTO_HEAD_SIZE);
	return true;
}

int proto_verification(const Universal::BinaryMemory &binary){
	short* pSize = (short*)(binary.getBuffer());
	short packSize = *pSize + PROTO_HEAD_SIZE;
	if(binary.getBufferSize() != packSize){
		DEBUG_E("包头内的包长度[" << binary.getBufferSize() << "]和实际包长度[" << packSize << "]不一致。");
		return 1;
	}

	Byte dataVerify = ((Byte*)binary.getBuffer())[PROTO_HEAD_VERIFY_INDEX];
	Byte lastByte = ((Byte*)binary.getBuffer())[binary.getBufferSize() - 1];
	if(dataVerify != lastByte){
		DEBUG_E("字节验证失败。验证字节[" << dataVerify << "] 待验证字节[" << lastByte << "]");
		return 2;
	}

	return 0;
}

proto_size proto_bypeToSize(Byte* pHead){
	if(pHead == NULL){
		DEBUG_E("包头未分配内存。");
		return 0;
	}
	proto_size* pSize = (proto_size*)pHead;
	return *pSize;
}

void proto_sizeToByte(proto_size size, Byte* pHead){
	if(pHead == NULL){
		DEBUG_E("包头未分配内存。");
		return ;
	}
	proto_size* pSize = (proto_size*)pHead;

	*pSize = size;
}



