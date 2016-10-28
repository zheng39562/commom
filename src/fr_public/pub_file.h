/**********************************************************
 * \file logic/tool/File.h
 * \brief 
 * \note	ÒòÎª¸öÈËÔÚlinuxÏÂ±à³Ì½Ï¶à£¬ËùÒÔwin32°æ±¾Î´½øÐÐ²âÊÔ¡£²»±£Ö¤windows»·¾³ÏÂÍêÈ«ÕýÈ·¡£
 * \note	windowsÏÂµÄ´óÖÂË¼Â·ÊÇÒ»ÑùµÄ£¬µ«Í·ÎÄ¼þºÍ²¿·Öº¯ÊýÃû»á´æÔÚÐ©Ðí²îÒì¡£
 * \note	×¢ÒâÊÂÏî£º
 *			1,ÀàÖÐµÄ³ÉÔ±º¯ÊýÖÐµÄÍ¬Ãû²ÎÊýµÄº¬ÒåÍêÈ«ÏàÍ¬¡£½ö»á×¢ÊÍÆäÖÐÒ»¸öº¯Êý£¬ÆäËûº¯ÊýÔò²»ÔÙÖØ¸´×¢ÊÍ¡£ÖØÃûµÄ²ÎÊýÒâÒå²»Í¬Ê±£¬»á¶ÀÁ¢×¢½â¡£
 *			2,µÚ1ÌõµÄ¹æÔòÍ¬ÑùÊÊÓÃÓÚ·µ»ØÖµµÄº¬Òå¡£
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _File_H
#define _File_H

#include <dirent.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <unistd.h>

#include "pub_define.h"

using namespace std;

namespace Universal{
	//! \brief	¶ÁÈ¡¶ÔÓ¦ÎÄ¼þµÄÊý¾Ý¡£
	//! \param[in] path	ÎÄ¼þÂ·¾¶¡£¾ø¶ÔÂ·¾¶ºÍÏà¶ÔÂ·¾¶½Ô¿É¡£²»´æÔÚÂ·¾¶·µ»ØÎ´¶¨Òå×Ö·û´®£¨Í¨³£±í´ï´íÎóµÄº¬Òå£©
	string readFile(const string &path);
	//! \brief	Ê¹ÓÃ¸²¸ÇµÄ·½Ê½Ð´ÈëÊý¾Ý¡£
	//! \param[in]	content	Ð´ÈëµÄÄÚÈÝ¡£
	bool writeFile(const string &path, const string &content);
	//! \brief	Ê¹ÓÃ×·¼ÓµÄ·½Ê½Ð´ÈëÄÚÈÝ
	//! \param[in]	content	Ð´ÈëµÄÄÚÈÝ¡£
	bool addContentToFile(const string &path, const string &content);
	//! \brief	Öð²ã´´½¨¶ÔÓ¦Ä¿Â¼£¨Èç¹ûÄ¿Â¼²»´æÔÚµÄ»°£©¡£
	//!	\param[in] path	ÐèÒª´´½¨µÄÄ¿Â¼¡£
	//! \param[in] mode	Ä¿Â¼È¨ÏÞµÄÅäÖÃÏî¡£
	bool createFolder(string path, mode_t mode= S_IRWXU|S_IRWXG|S_IRWXO);
	//! \brief	Çå¿ÕÄ¿Â¼ÏÂµÄËùÓÐÎÄ¼þºÍ×ÓÄ¿Â¼¡£
	bool clearFolder(string path);
	//! \brief  É¾³ý¶ÔÓ¦ÎÄ¼þ¡£
	bool deleteFile(const string &filePath);
	//! \brief	ÕÒµ½Ä¿Â¼ÏÂËùÓÐÎÄ¼þ¡££¨ºöÂÔ×ÓÄ¿Â¼£©
	//! \note	·À´í»úÖÆ²¢Î´ÍêÉÆ¡£²¢ÇÒ¶ÁÈ¡Ë³ÐòÎªÂÒÐò£¨ÓÐÐòµ«ÅÅÐò·½Ê½Î´Öª£¬ËùÒÔÀí½âÎªÂÒÐò£©
	//! \param[in] path	¾ßÌåÄ¿Â¼µÄË÷Òý
	//! \param[out] filename	ÐèÒª·µ»ØµÄÎÄ¼þÃû¼¯ºÏ¡£
	//! \param[in] filePattern	ÕýÔò±í´ïÊ½£¬ÓÃÀ´»ñÈ¡²»Æ¥ÅäµÄÎÄ¼þ¡£
	bool findFileFromDir(string path, vector<string> &fileNames, string filePattern = "");
	//! \brief	ÀûÓÃpopen/pcloseº¯Êýµ÷ÓÃÓÃshellÖ¸ÁîÀ´Íê³É¾ø¶ÔÂ·¾¶µÄ»ñÈ¡¡£
	//! \note	Â·¾¶×î´óÖµ²»ÄÜ³¬¹ý300.
	//! \note	´Ëº¯ÊýÐ§ÂÊ²»¸ß£¬²»½¨Òé´óÁ¿µÄÖØ¸´µ÷ÓÃ¡£
	//! \param[in]	filepath	Ïà¶ÔÂ·¾¶¡£
	string getAbsPath(string filepath = ".");
	//! \brief	´ÓÂ·¾¶ÖÐÌáÈ¡ÎÄ¼þÃû¡£Ö§³Ö¾ø¶ÔÂ·¾¶ºÍÏà¶ÔÂ·¾¶¡£
	//! \note	½öÖ§³Ö·Ö¸ô·ûÎª '/'¡£
	//! \param[in] filepath	ÎÄ¼þÂ·¾¶¡£Ö§³Ö¾ø¶ÔÂ·¾¶ºÍÏà¶ÔÂ·¾¶¡£µ«·Ö¸ô·û±ØÐëÎª '/'
	string getFileNameByPath(const string &filepath);
	//! \brief	²¹È«Â·¾¶ÖÐÈ±Ê§µÄÐ±¸Ü¡£(ÈçÐèÒª¿ÉÀ©³äÓïÒåÒ»ÖÂµÄÏà¹Ø¹¦ÄÜ)
	string completePath(const string &path);
}  // namespace : Universal

#endif


