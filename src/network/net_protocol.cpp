/**********************************************************
 *  \!file net_protocol.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "net_protocol.h"

#include "network/net_connection.h"

using namespace Universal;

namespace Network{

	void convertMsgToPacker(const ConnectKey &key, BinaryMemory &cache, MPackerPtrQueue &packerPtrQueue){
		const char* pMsg = (char*)cache.getBuffer();
		while(pMsg != NULL){
			size_t size;
			for(int i=PROTOCOL_INDEX_DATA_SIZE; i<PROTOCOL_MSG_SIZE_BYTE; ++i){
				size = size << 8;
				size += (size_t)pMsg[i];
			}

			if(size > cache.getBufferSize()){
				// 当前数据包未完全被获取。
				break;
			}

			char flags = pMsg[PROTOCOL_INDEX_EXPAND] & PROTOCOL_EXPAND_FILTER_OTHER;
			eProtocolDataFormat dataFormat = (eProtocolDataFormat)(pMsg[PROTOCOL_INDEX_EXPAND] & PROTOCOL_EXPAND_FILTER_FORMAT);

			PackerPtr pPacker(new Packer(key, dataFormat));
			pPacker->setBuffer(pMsg, size - PROTOCOL_HEAD_SIZE);
			packerPtrQueue.push(pPacker);

			cache.delBuffer(0, size);
		}

	}

	void convertPackerToMsg(const ConstPackerPtr &pPacker, BinaryMemory &buffer){
		char* pHead = new char[PROTOCOL_HEAD_SIZE];

		size_t size = pPacker->getBufferSize() + PROTOCOL_HEAD_SIZE;
		for(int i=PROTOCOL_MSG_SIZE_BYTE-1; i>=PROTOCOL_INDEX_DATA_SIZE; --i){
			pHead[i] = size & 0xFF;
			size = size >> 8;
		}
		//! flags还没有真正使用。暂时不需要添加。
		pHead[PROTOCOL_INDEX_EXPAND] = pPacker->getDataFormat() & PROTOCOL_EXPAND_FILTER_FORMAT; 

		buffer.addBuffer(pHead, PROTOCOL_HEAD_SIZE);
		buffer.addBuffer(pPacker->getBuffer(), pPacker->getBufferSize());

		delete pHead; pHead = NULL;
	}
}
