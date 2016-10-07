/**********************************************************
 *  \!file net_interface.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _net_interface_H
#define _net_interface_H

#include "network/net_packer.h"

//! \brief	
//! \note	使用：客户端使用connect链接。成功后，可以直接通过net_send_C/net_recv_C来收发包即可。
//! \todo	不支持多ip/port链接：如果需要多端口，可以使用net_client来完成。具体见文件。
//! \attetion	需要先connect后再进行发送和接受操作。否则行为未定义。
bool net_connect(const std::string &ip, const long &port);
//! \note	需要注意未packer的指针类。不支持直接的packer类。
void net_send_C(const Network::PackerPtr &pPacker);
bool net_recv_C(Network::PackerPtr &pPacker);

//! \brief	
//! \note	使用：服务端使用listen链接。成功后，可以直接通过net_send_S/net_recv_S来收发包即可。
//! \todo	不支持多ip/port监听：如果需要多端口，可以使用net_server来完成。具体见文件。
//! \attetion	需要先listen后再进行发送和接受操作。否则行为未定义。
bool net_listen(const std::string &ip, const long &port);
//! \note	需要注意未packer的指针类。不支持直接的packer类。
void net_send_S(const Network::PackerPtr &pPacker);
bool net_recv_S(Network::PackerPtr &pPacker);

#endif 

