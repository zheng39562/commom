/**********************************************************
 * \file CSFileTransmission.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "CSFileTransmission.h"

#include "CSCommonTool.h"

namespace Universal{
	CSRemoteAddrInfo::CSRemoteAddrInfo( const string &_ip, const string &_port, const string &_user, const string &_pwd )
		:ip( _ip ),
		 port( _port ),
		 user( _user ),
		 pwd( _pwd )
	{ ; }
	CSRemoteAddrInfo::~CSRemoteAddrInfo()
	{ ; }
} // namespace Universal{
namespace Universal{
	CSFileTransmission::CSFileTransmission( const string &_localPath, const string &_remotePath, const CSRemoteAddrInfo &_remoteAddrInfo )
		:m_DownloadPath( completePath( _localPath ) ), 
		 m_UploadPath( completePath( _localPath ) ), 
		 m_RemoteDownloadPath( completePath( _remotePath ) ), 
		 m_RemoteUploadPath( completePath( _remotePath ) ),
		 m_RemoteAddrInfo( _remoteAddrInfo )
	{ ; }
	CSFileTransmission::CSFileTransmission( const string &_downloadPath, const string &_uploadPath, const string &_remoteDown, const string &_remoteUp,
			const CSRemoteAddrInfo &_remoteAddrInfo )
		:m_DownloadPath( completePath( _downloadPath ) ), 
		 m_UploadPath( completePath( _uploadPath ) ), 
		 m_RemoteDownloadPath( completePath( _remoteDown ) ), 
		 m_RemoteUploadPath( completePath( _remoteUp ) ),
		 m_RemoteAddrInfo( _remoteAddrInfo )
	{ ; }
	CSFileTransmission::~CSFileTransmission(){ ; }


	string CSFileTransmission::getDownloadPath(){ return m_DownloadPath; }


	string CSFileTransmission::getUploadPath(){ return m_UploadPath; }
} // namespace Universal


namespace Universal{
	CSSocketFileTransfer::CSSocketFileTransfer( const string &_localPath, const string &_remotePath, const CSRemoteAddrInfo &_remoteAddrInfo )
		:CSFileTransmission( _localPath, _remotePath, _remoteAddrInfo )
	{ ; }
	CSSocketFileTransfer::CSSocketFileTransfer( const string &_downloadPath, const string &_uploadPath, const string &_remoteDown, const string &_remoteUp, 
			const CSRemoteAddrInfo &_remoteAddrInfo )
		:CSFileTransmission( _downloadPath, _uploadPath, _remoteDown, _remoteUp, _remoteAddrInfo )
	{ ; }
	CSSocketFileTransfer::~CSSocketFileTransfer() { ; }


	bool CSSocketFileTransfer::download( const string &pattern ){
		return true;
	}


	bool CSSocketFileTransfer::upload( const string &pattern ){
		return true;
	}

} // namespace Universal
