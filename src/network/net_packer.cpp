/**********************************************************
 *  \!file net_packer.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "net_packer.h"

namespace Network{
	Packer::Packer(const ConnectKey &_key)
		:m_ConnectKey(_key),
		 m_DataFormat(eProtocolDataFormat_Memory),
		 m_Buffer()
	{ ; }
	Packer::Packer(const ConnectKey &_key, const eProtocolDataFormat &_dataFormat)
		:m_ConnectKey(_key),
		 m_DataFormat(_dataFormat),
		 m_Buffer()
	{ ; }
	Packer::Packer(const Packer &ref){
		m_ConnectKey = ref.getConnectKey();
		m_DataFormat = ref.getDataFormat();
		setBuffer(ref.getBuffer(), ref.getBufferSize());
	}
	Packer& Packer::operator=(const Packer &ref){
		m_ConnectKey = ref.getConnectKey();
		m_DataFormat = ref.getDataFormat();
		setBuffer(ref.getBuffer(), ref.getBufferSize());
		return *this;
	}
	Packer::~Packer(){ ; }
} // namespace Network{

