/**********************************************************
 * \file logic/tool/File.h
 * \brief 
 *
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _File_H
#define _File_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "pub_define.h"

namespace Universal{
	//! \brief	读取文件
	std::string readFile(const std::string &path);
	//! \brief	写入文件（覆盖）
	bool writeFile(const std::string &path, const std::string &content);
	//! \brief	写入文件（追加）
	bool addContentToFile(const std::string &path, const std::string &content);
	//! \brief	创建目录。
	//! \return	如果不存在的话会创建，存在或创建失败都会返回false. 
	bool createDir(std::string filePath);
	//! \brief	删除目录下的文件和子目录.
	bool clearDir(std::string filePath);
	//! \brief	删除文件
	bool deleteFile(const std::string &filePath);
	//! \brief	查找目录下的所有文件名。
	//! \param[in] filePattern 过滤条件。是正则表达式方式。默认为空，表示不过滤。
	bool findFileFromDir(std::string filePath, std::vector<std::string> &fileNames, std::string filePattern = "");
	//! \brief	获取绝对路径。
	std::string getAbsPath(std::string filepath = ".");
	//! \brief	获取文件名称（带后缀）
	std::string getFileNameByPath(const std::string &filepath);
	//! \brief	补全路径：如果路径最后一个不是/ 则增加/
	std::string completePath(const std::string &path);
}  // namespace : Universal

#endif


