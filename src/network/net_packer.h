/**********************************************************
 *  \!file packer.h
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
#include "event2/bufferevent.h"

namespace Network{
	//! \brief	暂时不需要更多扩展。仅单纯作为基类。
	class Packer{
		public:
			Packer(){;}
			virtual ~Packer(){;}
	};

	class NetPacker{
		public:
			NetPacker();
			virtual ~NetPacker();
		public:
			bufferevent* m_BufferEvent;
			std::string m_Data;
	};
}
#endif 

