/**********************************************************
 * \file logic/tool/File.h
 * \brief 
 * \note	因为个人在linux下编程较多，所以win32版本未进行测试。不保证windows环境下完全正确。
 * \note	windows下的大致思路是一样的，但头文件和部分函数名会存在些许差异。
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
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
	//! \brief	读取对应文件的数据。
	//! \param[in] path	文件路径。绝对路径和相对路径皆可。不存在路径返回未定义字符串（通常表达错误的含义）
	string readFile(  const string &path );
	//! \brief	使用覆盖的方式写入数据。
	//! \param[in]	content	写入的内容。
	bool writeFile(  const string &path, const string &content );
	//! \brief	使用追加的方式写入内容
	//! \param[in]	content	写入的内容。
	bool addContentToFile(  const string &path, const string &content );
	//! \brief	逐层创建对应目录（如果目录不存在的话）。
	//!	\param[in] path	需要创建的目录。
	//! \param[in] mode	目录权限的配置项。
	bool createFolder( string path, mode_t mode= S_IRWXU|S_IRWXG|S_IRWXO);
	//! \brief	清空目录下的所有文件和子目录。
	bool clearFolder( string path );
	//! \brief  删除对应文件。
	bool deleteFile( const string &filePath );
	//! \brief	找到目录下所有文件。（忽略子目录）
	//! \note	防错机制并未完善。并且读取顺序为乱序（有序但排序方式未知，所以理解为乱序）
	//! \param[in] path	具体目录的索引
	//! \param[out] filename	需要返回的文件名集合。
	//! \param[in] filePattern	正则表达式，用来获取不匹配的文件。
	bool findFileFromDir( string path, vector<string> &fileNames );
	bool findFileFromDir( string path, const string &filePattern, vector<string> &fileNames );
	//! \brief	利用popen/pclose函数调用用shell指令来完成绝对路径的获取。
	//! \note	路径最大值不能超过300.
	//! \note	此函数效率不高，不建议大量的重复调用。
	//! \param[in]	filepath	相对路径。
	string getAbsPath( string filepath = "." );
	//! \brief	从路径中提取文件名。支持绝对路径和相对路径。
	//! \note	仅支持分隔符为 '/'。
	//! \param[in] filepath	文件路径。支持绝对路径和相对路径。但分隔符必须为 '/'
	string getFileNameByPath( const string &filepath );
	//! \brief	补全路径中缺失的斜杠。(如需要可扩充语义一致的相关功能)
	string completePath( const string &path );
}  // namespace : Universal

#endif


