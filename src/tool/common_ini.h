/**********************************************************
 *  \!file common_ini.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _common_ini_H
#define _common_ini_H

#include <string>
#include <map>
#include <vector>
#include "tool/single_mode.hpp"

namespace Universal{

	using namespace std;
	class IniCfg{
		public:
			IniCfg();
			~IniCfg();
		public:
			void InitFile(const string &filePath);

			string getString(const string &section, const string &key, string defaultValue = "");
			int getInt(const string &section, const string &key, int defaultValue = 0);
			double getDouble(const string &section, const string &key, double defaultValue = 0.0);
		private:
			map<string, map<string, string> > m_IniInfo;
	};
	typedef DesignMode::SingleMode<IniCfg> SingleIniCfg;
}
#endif 

