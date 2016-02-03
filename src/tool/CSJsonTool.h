/**********************************************************
 * \file logic/CSJsonTool.h
 * \brief 
 * \note 
 * \note	注意事项：
 *			1	类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2	第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _CSJsonTool_H
#define _CSJsonTool_H

#include "CSDefine.h"
#include "json/json.h"
#include "CSCommonTool.h"
#include "CSFile.h"

#include <algorithm>
using namespace std;
using namespace Json;

namespace Universal{
	//! \breif	JSON的处理类。基于JSONCPP库。
	//! \note	此类目的：封装JSONCPP。增加复杂的操作功能 以及 增加前置判断来减少JSONCPP的崩溃行为。
	//! \note	Get开头的函数为值传递，Find或其他开头的为引用/指针传递。
	//! \todo	解决解析问题：111{sdsa}ssds解析结果为true，jsonvalue中保存111
	//! \todo	解决错误索引和未知JSON访问导致异常的问题。（阅读源码，查找解决思路）
	class CSJsonTool{
		public:
			CSJsonTool( );
			~CSJsonTool( );
		private:
			static string indexOfSplittingPath;			//! 索引路径的默认分隔符
			static string indexOfSplittingKey;			//! 关键字列表的默认分隔符
		public:
			//! \brief	JSON字符串解析函数。讲JSON字符串解析为Json::Value的类对象。
			//! \note	解析本质由三方库jsoncpp提供解析。存在部分瑕疵。
			//! \note	已知问题：111{sdsa}ssds解析结果为true，jsonvalue中保存111
			//! \param[in] jsStr 需要被解析的JSON字符串
			//! \param[out] jsValue 解析返回的对象。
			//! \return	false 表示解析失败。
			static bool parseJs( const string &jsStr, Json::Value &jsValue );
			//! \brief	通过路径获取对应的JSON对象。
			//! \param[in] path JSON的索引路径。默认由 '.' 分隔。暂未提供更改的接口。
			static Json::Value getJsValue( const Json::Value &jsValue, const string &path );
			//! \brief	通过key-value来索引JSON对象。此函数可在数组中查找所需要的JSON对象并返回。
			//! \param[in] jsArray JSON对象。此功能通常用来索引数据。
			//! \param[in] key JSON对象中包含的KEY值。
			//! \param[in] value JSON对象中包含的Value值。 
			//! \retval	通常返回索引对象。查询不到是则为空对象
			static Json::Value getJsValue( const Json::Value &jsArray, const string &key, const string &value );
			
			//! \brief	与GetJsValue功能基本一致。但Find返回的是当前对象的指针。
			//! \note	操作指针会影响对应的对象。如果仅需要值，建议使用Get函数
			static Json::Value* findJsValue( Json::Value &jsValue, const string &path );
			static Json::Value* findJsValue( Json::Value &jsArray, const string &key, const string &value );

			//! \brief	获取JS的value值。
			//! \todo	检测并不完善。使用此函数尽量确认其为正确的字符类型。
			static string getJsString( const Json::Value &jsValue, string path = "" );
			//! \brief	JSON对象中获取对应Key值的所有value值。
			//! \note	该函数目标为JSON数组对象。但其可以对一般JSON对象使用。
			//! \param[out] strList 返回的value值数组
			static bool getJsStrList( const Json::Value &jsArray, const string &key, vector<string> &strList );

			//! \brief	转换JSON对象到map对象。
			//! \param[out] mapResult 保存JSON数据的map对象
			//! \param[in] keyList 根据key的列表来转换对应的key。默认为空时，表示所有的key都将被转换。
			static bool convertJsToMap( const Json::Value &jsValue, map<string, string> &mapResult, string keyList = "" );
			//! \brief	转换JSON对象到map对象数组。
			//! \param[out] mapListResult 保存JSON数据的map对象数组。
			static bool convertJsToMapList( const Json::Value &jsValue, vector<map<string, string> > &mapListResult, string keyList = "" );
			static bool convertJsToMapList( const Json::Value &jsValue, list<map<string, string> > &mapListResult, string keyList = "" );

			//! \brief	map 转换为 JSON对象。
			//! \param[in] mapData 将被转换的map对象
			static string convertMapToJsStr( const map<string, string> mapData );
			//! \brief	map数组 转换为 JSON对象
			//! \param[in] mapList 将被转换的map对象数组。
			//! \param[in] arrayName 可以对此数组进行额外命名。默认为空表示不进行命名
			static string convertMapListToJsStr( const vector<map<string, string> > &mapList, string arrayName = "" );
			static string convertMapListToJsStr( const list<map<string, string> > &mapList, string arrayName = "" );
		private:
			//! \brief	解析key的索引路径
			//! \param[out] keys 保存对象的key值
			static bool parseKeyPath( const string &path, vector<string> &keys );
			//! \brief	解析key值的列表。并保存到key的数组中。
			static bool parseKeyList( const string &keyList, vector<string> &keys );
			//! \brief	通过key找到对象的JSON对象。
			static Json::Value* findJsValueByKey( Json::Value &jsValue, string &key );
	};
}

#endif
