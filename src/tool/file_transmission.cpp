/**********************************************************
 * \file FileTransmission.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "file_transmission.h"

#include "common_tool.h"

namespace Universal{
	RemoteAddrInfo::RemoteAddrInfo( const string &_ip, const string &_port, const string &_user, const string &_pwd )
		:ip( _ip ),
		 port( _port ),
		 user( _user ),
		 pwd( _pwd )
	{ ; }
	RemoteAddrInfo::~RemoteAddrInfo()
	{ ; }
} // namespace Universal{
namespace Universal{
	FileTransmission::FileTransmission( const string &_localPath, const string &_remotePath, const RemoteAddrInfo &_remoteAddrInfo )
		:m_DownloadPath( completePath( _localPath ) ), 
		 m_UploadPath( completePath( _localPath ) ), 
		 m_RemoteDownloadPath( completePath( _remotePath ) ), 
		 m_RemoteUploadPath( completePath( _remotePath ) ),
		 m_RemoteAddrInfo( _remoteAddrInfo )
	{ ; }
	FileTransmission::FileTransmission( const string &_downloadPath, const string &_uploadPath, const string &_remoteDown, const string &_remoteUp,
			const RemoteAddrInfo &_remoteAddrInfo )
		:m_DownloadPath( completePath( _downloadPath ) ), 
		 m_UploadPath( completePath( _uploadPath ) ), 
		 m_RemoteDownloadPath( completePath( _remoteDown ) ), 
		 m_RemoteUploadPath( completePath( _remoteUp ) ),
		 m_RemoteAddrInfo( _remoteAddrInfo )
	{ ; }
	FileTransmission::~FileTransmission(){ ; }


	string FileTransmission::getDownloadPath(){ return m_DownloadPath; }


	string FileTransmission::getUploadPath(){ return m_UploadPath; }
} // namespace Universal


namespace Universal{
	SocketFileTransfer::SocketFileTransfer( const string &_localPath, const string &_remotePath, const RemoteAddrInfo &_remoteAddrInfo )
		:FileTransmission( _localPath, _remotePath, _remoteAddrInfo )
	{ ; }
	SocketFileTransfer::SocketFileTransfer( const string &_downloadPath, const string &_uploadPath, const string &_remoteDown, const string &_remoteUp, 
			const RemoteAddrInfo &_remoteAddrInfo )
		:FileTransmission( _downloadPath, _uploadPath, _remoteDown, _remoteUp, _remoteAddrInfo )
	{ ; }
	SocketFileTransfer::~SocketFileTransfer() { ; }


	bool SocketFileTransfer::download( const string &pattern ){
		return true;
	}


	bool SocketFileTransfer::upload( const string &pattern ){
		return true;
	}

} // namespace Universal
