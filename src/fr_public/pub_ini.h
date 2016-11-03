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
#include "fr_template/single_mode.hpp"

namespace Universal{
	//! \brief 读取，解析ini的类
	class IniCfg{
		public:
			IniCfg();
			~IniCfg();
		public:
			//! \brief	读取ini文件，并且解析对应的文件。
			//! \note	自动过滤key/value前后的空格对应的空格,不会对数据中间的空格做处理。
			//! \note	section没有做任何处理
			//! \attetion	sction名称不能存在重复。第二个section无法被解析，其中的key=value会被自行增加到前一个section中。
			bool initFile(const std::string &filePath);

			//! \brief	获取字符串数据。
			std::string getString(const std::string &section, const std::string &key, std::string defaultValue = "");
			//! \brief	获取int数据：需要自己保证可转换。否则行为未定义
			int getInt(const std::string &section, const std::string &key, int defaultValue = 0);
			//! \brief	获取double数据：需要自己保证可转换。否则行为未定义
			double getDouble(const std::string &section, const std::string &key, double defaultValue = 0.0);
		private:
			//! \brief	 该行是否是一个妞section
			bool isNewSection(const std::string &section);
		private:
			std::map<std::string, std::map<std::string, std::string> > m_IniInfo;	//! 
	};
	typedef DesignMode::SingleMode<IniCfg> SingleIniCfg;
}
#endif 

