/**********************************************************
 * \file CSFileTransmission.h
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _CSFileTransmission_H
#define _CSFileTransmission_H

#include "CSDefine.h"

namespace Universal{
	class CSRemoteAddrInfo {
		public:
			CSRemoteAddrInfo( const string &_ip, const string &_port, const string &_user, const string &_pwd );
			~CSRemoteAddrInfo();
		private:
			string ip;
			string port;
			string user;
			string pwd;
		public:
			string getIp()const{ return ip; }
			string getPort()const{ return port; }
			string getUser()const{ return user; }
			string getPwd()const{ return pwd; }
	};
	//! \brief	文件传输者（基类）
	//! \note	Responsibility:
	//! \note		记录和保存：文件完整路径
	//!				上传和下载文件: 文件名参数支持正则表达式 。
	//!				提供完成的本地路径（上传和下载），以供调用。
	//! \todo	需要构建一个基本的可用子类。目标为：实现一个C++的基本下载/上传功能。
	class CSFileTransmission {
		public:
			CSFileTransmission( const string &_localPath, const string &_remotePath, const CSRemoteAddrInfo &_remoteAddrInfo );
			CSFileTransmission( const string &_downloadPath, const string &_uploadPath, const string &_remoteDown, const string &_remoteUp, 
					const CSRemoteAddrInfo &_remoteAddrInfo );
			virtual ~CSFileTransmission();
		protected:
			string										m_DownloadPath;
			string										m_RemoteDownloadPath;
			string										m_UploadPath;
			string										m_RemoteUploadPath;
			CSRemoteAddrInfo							m_RemoteAddrInfo;
		public:
			//! \brief	下载文件。
			//! \param[in] pattern 除文件名外支持正则表达式：（注意上传也支持相同的方式）.但不支持子目录的获取。
			//!				* 所有文件。
			//!				*.sh 所有.sh结尾的文件。
			virtual bool download( const string &pattern )=0;
			//! \brief	上传文件。
			virtual bool upload( const string &pattern )=0;

			//! \brief  获取本地的下载路径。
			string getDownloadPath();
			//! \brief  获取本地的上传路径。
			string getUploadPath();
	};
	//! \brief	socket版本的文件传输类。
	class CSSocketFileTransfer : public CSFileTransmission {
		public:
			CSSocketFileTransfer( const string &_localPath, const string &_remotePath, const CSRemoteAddrInfo &_remoteAddrInfo );
			CSSocketFileTransfer( const string &_downloadPath, const string &_uploadPath, const string &_remoteDown, const string &_remoteUp,
					const CSRemoteAddrInfo &_remoteAddrInfo );
			virtual ~CSSocketFileTransfer();
		public:
			//! \brief	下载文件。
			virtual bool download( const string &pattern );
			//! \brief	上传文件。
			virtual bool upload( const string &pattern );
	};
} // namespace Universal{
#endif 

