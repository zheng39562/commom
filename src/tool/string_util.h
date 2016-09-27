/**********************************************************
 * \file source/tool/StrUtil.h
 * \brief	字符串的通用函数集
 * \note	尽可能使用C++标准库的函数，但不保证windows下100%可用（未在window下测试）。
 * \note	大部分寻找网络上能找到的最好实现，实在找不到，才会自己进行实现。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _StrUtil_H
#define _StrUtil_H

#include "common_define.h"

using namespace std;

//
// 字符和其他格式之间的转换。
//
namespace Universal{
	//! \brief	给str增加单引号(主要用于SQL)
	inline string quotedStr( const string &str ){ return "'" + str + "'"; }
	inline string doubleQuotedStr( const string &str ){ return "\"" + str + "\""; }
	
	std::string intToStr( const long &number );
	long strToInt( const string &number );
	//! \brief	字符和数字之间的相互转换。使用是stringstream形式。
	std::string doubleToStr( const double &number );

	//! \brief	
	//long strToInt( const string &number );
	//! \brief	
	double strToDouble( const string &number );

	//! \brief	字符串中XML的关键符号转换为实体。
	string strToXmlStr( string content );
	//! \brief	字符串中XML的实体转换为常规符号。
	string xmlStrToStr( string content );
	//! \brief	字符串中json的关键符号转换为实体。
	string strToJsonStr( string content );
	//! \brief	字符串中json的实体转换为常规符号。
	string jsonStrToStr( string content );

	//! \brief	编码格式设置。
	enum eCharacterEncoding{
		eEncoding_FirstInvalid,
		eEncoding_UTF8,
		eEncoding_Unicode,
		eEncoding_GBK,
		eEncoding_LastInvalid,
	};
	//! \brief	未完成。	
	//! \todo	
	void ConvertToUtf8( string &str, const eCharacterEncoding &originalEncoding );
	//! \brief	unicode编码格式 转换为UFT8 。注意unicode为16位。
	string ConvertUnicodeToUtf8( int &unicode );
	//! \biref	字符 全角转半角 的函数。
	//! \note	暂时仅支持UFT8，其他字符的支持需要扩展。
	//! \param[in,out] str 输出及输出。
	//! \param[in] encoding 编码参数，暂时仅支持UFT8
	void convertFullToHalf( string &str, eCharacterEncoding encoding = eEncoding_UTF8 );
	//! \biref	字符 半角转全角 的函数。
	//! \note	暂时仅支持UFT8，其他字符的支持需要扩展。
	//! \param[in,out] str 输出及输出。
	//! \param[in] encoding 编码参数，暂时仅支持UFT8
	void convertHalfToFull( string &str, eCharacterEncoding encoding = eEncoding_UTF8 );
	//! \brief	得到不同编码的字符长度。
	//! \note	暂时仅支持UFT8。
	//! \todo	下一步建议扩展GBK。
	size_t getStrSize( const string &str, eCharacterEncoding encoding = eEncoding_UTF8 );
} // namespace Universal{


//
// 字符本身的转换。
//
namespace Universal{
	//! \brief	将字符串中的小写转换为大写。
	//! \param[in]	str	转换字符串。本身不改变。
	//! \retval	转换后的字符串。
	std::string strToUpper( const string &str );
	//! \brief	将字符串中的大写转换为小写。
	//! \retval	转换后的字符串。
	std::string strToLower( const string &str );
} // namespace Universal{


//
// 字符操作：合并，分割，替换。。。
//
namespace Universal{
	//! \brief	替换字符串中对应的子字符串。
	//! \param[out]	content	替换的字符串。
	//! \param[in]	replace	被替换的子字符串。
	//! \param[in]	dest	换入的子字符串。
	bool strReplaceAll( string &content, const string &replace, const string &dest );
	//! \brief	通过分割符来分割字符串，并将分割好的数据存在数组中。
	//! \note	当输入字符串为空时，结果会返回一个空值的字符串。
	//! \param[in]	strValue	
	//! \param[in]	solittingKey	
	//! \param[out]	result	
	bool splitString( const string &strValue, const string &splittingKey, vector<string> &result );
	bool splitString( const string &strValue, const string &splittingKey, list<string> &result );
	//! \brief	合并字符串。使用分隔符分隔。
	//! \param[in]	strings		字符串数组。
	//! \param[in]	mergerKey	合并时使用的分隔符。
	string mergeString( const vector<string> &strings, const string &mergerKey );
	//! \brief	删除左边(右边)的空白符，制表符，换行符
	//! \note	不处理中间空格，但会删除所有的制表符和换行符。
	string trimRight( const string &str );
	string trimLeft( const string &str );
}  // namespace : Universal


#endif


