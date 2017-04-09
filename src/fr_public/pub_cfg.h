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

#include <mutex>
#include <thread>
#include "fr_public/pub_define.h"
#include "fr_template/single_mode.hpp"

namespace Universal{
	enum eCfgDataType{
		eCfgDataType_Unknow,
		eCfgDataType_Ini,
		eCfgDataType_DB
	};
	//! \brief	配置器
	//! \note	使用：
	//				* 构造对象：建议直接使用模板单例(SingleConfigurator)。
	//				* 调用readCfg进行初始化。
	//				* 根据接口读取配置。
	//! \note	因为配置能够被更新，所以增加锁的限制。
	//! \note	如果整个项目不需要对cfg进行修改，则可以关闭锁(未完成，可自行删除锁相关代码)
	class Configurator{
		public:
			typedef std::string CfgKey;
		public:
			Configurator();
			~Configurator();
		public:
			//! \brief	配置添加。
			//! \param[in] param 读取参数，根据type的不同表示不同的信息。
			//! \param[in] type 配置的数据存储方式。
			bool addCfg(const CfgKey &cfgKey, const std::string &param, const eCfgDataType &type);
			void saveCfg(const CfgKey &cfgKey);
			inline void setDefaultCfgKey(const CfgKey &cfgKey){ if(m_DefaultCfgKey.empty()){ m_DefaultCfgKey = cfgKey; } }

			//! \brief	获取int数据：需要自己保证可转换。否则行为未定义
			std::string getString(const CfgKey &cfgKey, const std::string &section, const std::string &key, const std::string &defaultValue);
			inline std::string getString(const std::string &section, const std::string &key, const std::string &defaultValue){ return getString(m_DefaultCfgKey, section, key, defaultValue); }
			//! \brief	获取int数据：需要自己保证可转换。否则行为未定义
			int getInt(const CfgKey &cfgKey, const std::string &section, const std::string &key, const int &defaultValue);
			inline int getInt(const std::string &section, const std::string &key, const int &defaultValue){ return getInt(m_DefaultCfgKey, section, key, defaultValue); }
			//! \brief	获取double数据：需要自己保证可转换。否则行为未定义
			double getDouble(const CfgKey &cfgKey, const std::string &section, const std::string &key, const double &defaultValue);
			inline double getDouble(const std::string &section, const std::string &key, const double &defaultValue){ return getDouble(m_DefaultCfgKey, section, key, defaultValue); }

			//! \brief	设置ini文件的值
			//! \note	如果section or key不存在，则不做任何操作(会有日志信息).
			bool setString(const CfgKey &cfgKey, const std::string &section, const std::string &key, const std::string &value);
			inline bool setString(const std::string &section, const std::string &key, const std::string &value){ return setString(m_DefaultCfgKey, section, key, value); }
			//! \brief	设置ini文件的值
			bool setInt(const CfgKey &cfgKey, const std::string &section, const std::string &key, int32 value);
			inline bool setInt(const std::string &section, const std::string &key, int32 value){ return setInt(m_DefaultCfgKey, section, key, value); }
			//! \brief	设置ini文件的值
			bool setDouble(const CfgKey &cfgKey, const std::string &section, const std::string &key, double value);
			inline bool setDouble(const std::string &section, const std::string &key, double value){ return setInt(m_DefaultCfgKey, section, key, value); }
		private:
			void* getCfgPtr(const CfgKey &cfgKey);
		private:
			std::map<CfgKey, void*> m_CfgMap;	//! 根据配置(eCfgDataType)保存不同的文件指针。collection 表示不同的文件集合
			CfgKey m_DefaultCfgKey;
			eCfgDataType m_CfgType;	//! 
			std::mutex m_Mutex;
	};
	typedef DesignMode::SingleMode<Configurator> SingleConfigurator;
}

#endif 

