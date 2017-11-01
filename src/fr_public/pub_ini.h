/**********************************************************
 *  \!file pub_ini.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _pub_ini_H
#define _pub_ini_H

#include "pub_define.h"
#include "single_mode.hpp"

namespace universal{
	//! \brief	读取，解析，保存ini的类
	//! \note	只支持\n换行。任何其他形式的(例如\r\n)都会被转换成\n。并且，在会写程序时也不会转换回\r\n
	//! \note	ini限制：非标准ini的限制，仅是该ini解析类的限制.违反限制的结果未定义,但尽可能会保持可用.
	//				* 任意行都不支持以空格作为起始字节：不遵守则无法使用
	//				* key不允许以 "[" or 空格 开头。
	//				* section行的起始不能有空格
	//				* 支持行内注释。
	//				* key=value 在等号之间不要增加空格，否则key将变为 "key "
	class IniCfg{
		public:
			IniCfg();
			~IniCfg();
		public:
			//! \brief	读取ini文件，并且解析对应的文件。
			//! \note	自动过滤key/value前后的空格对应的空格,不会对数据中间的空格做处理。
			//! \note	section没有做任何处理
			//! \attetion	sction名称不能存在重复。第二个section无法被解析，其中的key=value会被自行增加到前一个section中。
			bool InitFile(const std::string &filePath);
			//! \brief	保存ini文件
			//! \note	在回写ini文件时，能够完整保存对应的注释信息和换行
			void SaveFile(const std::string &filePath);
			inline void SaveFile(){ SaveFile(file_path_); }

			//! \brief	获取字符串数据。
			std::string GetString(const std::string &section, const std::string &key, std::string defaultValue = "");
			//! \brief	获取int数据：需要自己保证可转换。否则行为未定义
			int GetInt(const std::string &section, const std::string &key, int defaultValue = 0);
			//! \brief	获取double数据：需要自己保证可转换。否则行为未定义
			double GetDouble(const std::string &section, const std::string &key, double defaultValue = 0.0);

			//! \brief	设置ini文件的值
			//! \note	如果section or key不存在，则不做任何操作(会有日志信息).
			bool SetString(const std::string &section, const std::string &key, const std::string &value);
			//! \brief	设置ini文件的值
			bool SetInt(const std::string &section, const std::string &key, int value);
			//! \brief	设置ini文件的值
			bool SetDouble(const std::string &section, const std::string &key, double value);
		private:
			//! \brief	该行是否是一个妞section
			bool IsNewSection(const std::string &section);
			//! \brief	是否是注释行。
			bool IsNote(const std::string &str);
			//! \brief	更新文件中的value值
			bool UpdateIniContent(const std::string &section, const std::string &key, const std::string &value);
		private:
			Path file_path_;
			std::string ini_content_;	//! ini文本的内容。用户回写ini文本.
			std::map<std::string, std::map<std::string, std::string> > ini_info_;	//! 
	};
	typedef SingleMode<IniCfg> SingleIniCfg;
}
#endif 

