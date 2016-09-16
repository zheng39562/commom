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

#include <string>
#include <list>
#include <map>
#include "network/net_struct.h"
#include "tool/single_mode.hpp"

namespace Network{
	enum eProtocolDataFormat;

	typedef std::string PackerKey;
	//! \brief	暂时不需要更多扩展。仅单纯作为基类。
	class Packer{
		public:
			Packer()
				:m_ConnectKey(NET_CACHEKEY_DEFAULT)
			{;}
			virtual ~Packer(){;}
		public:
			virtual bool parseMsg(const std::string &msg)=0; 
			virtual string getPackerStr(const PackerKey &packerKey)=0;
			virtual string getString(const PackerKey &packerKey)=0;
			virtual string getInt(const PackerKey &packerKey)=0;
			virtual string getDouble(const PackerKey &packerKey)=0;
			virtual string getBoolean(const PackerKey &packerKey)=0;
			virtual string getChar(const PackerKey &packerKey)=0;

			virtual eProtocolDataFormat getDataFormat()=0;

			ConnectKey getConnectKey();
		private:
			ConnectKey m_ConnectKey;
	};
	typedef boost::shared_ptr<Packer> PackerPtr;
	typedef boost::shared_ptr<const Packer> ConstPackerPtr;

	typedef Universal::LockQueue<MsgPtr> MPackerPtrQueue;
	typedef MPackerPtrQueue PackerCache;
}
#endif 

