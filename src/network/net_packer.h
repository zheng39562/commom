/**********************************************************
 *  \!file net_packer.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _net_packer_H
#define _net_packer_H

#include "network/net_struct.h"
#include "template/single_mode.hpp"
#include "template/lock_queue.hpp"

namespace Network{
	//! \brief	数据包。
	//! \note	结构类似linux文件。path表示路径。collection类似目录，item类似文件。目录没有内容。具体内容保存在item中（即文件）
	//! \todo	目标会接受xml 和 json。但目前看来json会优先（已有三方库可用）
	//! \todo	考虑使用三方库需要先解析，然后再进行深复制。考虑对此进行优化。
	//! \todo	暂时不对数组类型单独考虑（把数字0，1等当作一个key。）。在实际使用中，再后续考虑是否需要对数组单独做一些接口。
	class Packer{
		public:
			Packer(const ConnectKey &_key);
			Packer(const ConnectKey &_key, const eProtocolDataFormat &_dataFormat);
			Packer(const Packer &ref);
			Packer& operator=(const Packer &ref);
			virtual ~Packer();
		public:
			void setBuffer(const void *buffer, size_t size);

			inline void* getBuffer(){ return m_Buffer; }
			inline size_t getBufferSize()const{ return m_BufferSize; }

			inline ConnectKey getConnectKey()const{ return m_ConnectKey; }
			inline const eProtocolDataFormat& getDataFormat()const{ return m_DataFormat; }
		private:
			ConnectKey m_ConnectKey;
			eProtocolDataFormat m_DataFormat;
			void* m_Buffer;
			size_t m_BufferSize;
	};
	typedef boost::shared_ptr<Packer> PackerPtr;
	typedef boost::shared_ptr<const Packer> ConstPackerPtr;

	typedef Universal::LockQueue<PackerPtr> MPackerPtrQueue;
	typedef MPackerPtrQueue PackerCache;
}
#endif 

