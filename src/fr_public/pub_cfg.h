/**********************************************************
 *  \file pub_cfg.h
 *  \brief	配置类：用于从文件(or DB or other)中读取配置。
 *  \note	封装了读取的方式。
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _pub_cfg_H
#define _pub_cfg_H

#include "pub_define.h"

namespace Universal{
	enum eCfgDataType{
		eCfgDataType_Ini,
		eCfgDataType_DB
	};
	//! \brief	配置器
	//! \note	使用：
	//				* 构造对象：建议直接使用模板单例(SingleConfigurator)。
	//				* 调用readCfg进行初始化。
	//				* 根据接口读取配置。
	class Configurator{
		public:
			Configurator();
			~Configurator();
		public:
			//! \brief	配置读取。
			//! \param[in] param 读取参数，根据type的不同表示不同的信息。
			//! \param[in] type 配置的数据存储方式。
			bool readCfg(const std::string &param, const eCfgDataType &type);

			//! \brief	
			std::string getCfg(const std::string &section, const std::string &key);
	};
}
#endif 

