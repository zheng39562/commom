/**********************************************************
 * \file FileTransmission.h
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _FileTransmission_H
#define _FileTransmission_H

#include "common_define.h"

namespace Universal{
	class RemoteAddrInfo {
		public:
			RemoteAddrInfo( const std::string &_ip, const std::string &_port, const std::string &_user, const std::string &_pwd );
			~RemoteAddrInfo();
		private:
			std::string ip;
			std::string port;
			std::string user;
			std::string pwd;
		public:
			std::string getIp()const{ return ip; }
			std::string getPort()const{ return port; }
			std::string getUser()const{ return user; }
			std::string getPwd()const{ return pwd; }
	};
	//! \brief	文件传输者（基类）
	//! \note	Responsibility:
	//! \note		记录和保存：文件完整路径
	//!				上传和下载文件: 文件名参数支持正则表达式 。
	//!				提供完成的本地路径（上传和下载），以供调用。
	//! \todo	需要构建一个基本的可用子类。目标为：实现一个C++的基本下载/上传功能。
	class FileTransmission {
		public:
			FileTransmission( const std::string &_localPath, const std::string &_remotePath, const RemoteAddrInfo &_remoteAddrInfo );
			FileTransmission( const std::string &_downloadPath, const std::string &_uploadPath, const std::string &_remoteDown, const std::string &_remoteUp, 
					const RemoteAddrInfo &_remoteAddrInfo );
			virtual ~FileTransmission();
		protected:
			std::string										m_DownloadPath;
			std::string										m_RemoteDownloadPath;
			std::string										m_UploadPath;
			std::string										m_RemoteUploadPath;
			RemoteAddrInfo							m_RemoteAddrInfo;
		public:
			//! \brief	下载文件。
			//! \param[in] pattern 除文件名外支持正则表达式：（注意上传也支持相同的方式）.但不支持子目录的获取。
			//!				* 所有文件。
			//!				*.sh 所有.sh结尾的文件。
			virtual bool download( const std::string &pattern )=0;
			//! \brief	上传文件。
			virtual bool upload( const std::string &pattern )=0;

			//! \brief  获取本地的下载路径。
			std::string getDownloadPath();
			//! \brief  获取本地的上传路径。
			std::string getUploadPath();
	};
	//! \brief	socket版本的文件传输类。
	class SocketFileTransfer : public FileTransmission {
		public:
			SocketFileTransfer( const std::string &_localPath, const std::string &_remotePath, const RemoteAddrInfo &_remoteAddrInfo );
			SocketFileTransfer( const std::string &_downloadPath, const std::string &_uploadPath, const std::string &_remoteDown, const std::string &_remoteUp,
					const RemoteAddrInfo &_remoteAddrInfo );
			virtual ~SocketFileTransfer();
		public:
			//! \brief	下载文件。
			virtual bool download( const std::string &pattern );
			//! \brief	上传文件。
			virtual bool upload( const std::string &pattern );
	};
} // namespace Universal{
#endif 

