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
#include "pub_define.h"
#include "fr_template/single_mode.hpp"

namespace universal{
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
			bool AddCfg(const CfgKey &cfg_key, const std::string &param, const eCfgDataType &type);
			void SaveCfg(const CfgKey &cfg_key);
			inline void SaveCfg(){ SaveCfg(default_cfg_key_); }
			inline void SetDefaultCfgKey(const CfgKey &cfg_key){ if(default_cfg_key_.empty()){ default_cfg_key_ = cfg_key; } }

			//! \brief	获取int数据：需要自己保证可转换。否则行为未定义
			std::string GetString(const CfgKey &cfg_key, const std::string &section, const std::string &key, const std::string &default_value);
			inline std::string GetString(const std::string &section, const std::string &key, const std::string &default_value){ return GetString(default_cfg_key_, section, key, default_value); }
			//! \brief	获取int数据：需要自己保证可转换。否则行为未定义
			int GetInt(const CfgKey &cfg_key, const std::string &section, const std::string &key, const int &default_value);
			inline int GetInt(const std::string &section, const std::string &key, const int &default_value){ return GetInt(default_cfg_key_, section, key, default_value); }
			//! \brief	获取double数据：需要自己保证可转换。否则行为未定义
			double GetDouble(const CfgKey &cfg_key, const std::string &section, const std::string &key, const double &default_value);
			inline double GetDouble(const std::string &section, const std::string &key, const double &default_value){ return GetDouble(default_cfg_key_, section, key, default_value); }

			//! \brief	设置ini文件的值
			//! \note	如果section or key不存在，则不做任何操作(会有日志信息).
			bool SetString(const CfgKey &cfg_key, const std::string &section, const std::string &key, const std::string &value);
			inline bool SetString(const std::string &section, const std::string &key, const std::string &value){ return SetString(default_cfg_key_, section, key, value); }
			//! \brief	设置ini文件的值
			bool SetInt(const CfgKey &cfg_key, const std::string &section, const std::string &key, int value);
			inline bool SetInt(const std::string &section, const std::string &key, int value){ return SetInt(default_cfg_key_, section, key, value); }
			//! \brief	设置ini文件的值
			bool SetDouble(const CfgKey &cfg_key, const std::string &section, const std::string &key, double value);
			inline bool SetDouble(const std::string &section, const std::string &key, double value){ return SetInt(default_cfg_key_, section, key, value); }
		private:
			void* GetCfgPtr(const CfgKey &cfg_key);
		private:
			std::map<CfgKey, void*> cfg_map_;	//! 根据配置(eCfgDataType)保存不同的文件指针。collection 表示不同的文件集合
			CfgKey default_cfg_key_;
			eCfgDataType cfg_type_;	//! 
			std::mutex mutex_;
	};
	typedef universal::SingleMode<Configurator> SingleConfigurator;
}

inline bool CFG_GET_BOOL(const std::string& section, const std::string& key, const bool &default_value){ 
	return (bool)universal::SingleConfigurator::GetInstance()->GetInt(section, key, (int)default_value); 
}
inline bool CFG_GET_BOOL(const universal::Configurator::CfgKey& cfg_key, const std::string& section, const std::string& key, const bool &default_value){ 
	return (bool)universal::SingleConfigurator::GetInstance()->GetInt(cfg_key, section, key, (int)default_value); 
}

inline int CFG_GET_INT(const std::string& section, const std::string& key, const int &default_value){ 
	return universal::SingleConfigurator::GetInstance()->GetInt(section, key, default_value); 
}
inline int CFG_GET_INT(const universal::Configurator::CfgKey& cfg_key, const std::string& section, const std::string& key, const int &default_value){ 
	return universal::SingleConfigurator::GetInstance()->GetInt(cfg_key, section, key, default_value); 
}

inline double CFG_GET_DOUBLE(const std::string& section, const std::string& key, const double &default_value){ 
	return universal::SingleConfigurator::GetInstance()->GetDouble(section, key, default_value); 
}
inline double CFG_GET_DOUBLE(const universal::Configurator::CfgKey& cfg_key, const std::string& section, const std::string& key, const double &default_value){ 
	return universal::SingleConfigurator::GetInstance()->GetDouble(cfg_key, section, key, default_value); 
}

inline std::string CFG_GET_STRING(const std::string& section, const std::string& key, const std::string &default_value){ 
	return universal::SingleConfigurator::GetInstance()->GetString(section, key, default_value); 
}
inline std::string CFG_GET_STRING(const universal::Configurator::CfgKey& cfg_key, const std::string& section, const std::string& key, const std::string &default_value){ 
	return universal::SingleConfigurator::GetInstance()->GetString(cfg_key, section, key, default_value); 
}


inline bool CFG_SET_BOOL(const std::string& section, const std::string& key, bool value){ 
	if(universal::SingleConfigurator::GetInstance()->SetInt(section, key, (int)value)){
		universal::SingleConfigurator::GetInstance()->SaveCfg();
		return true;
	}
	return false; 
}
inline bool CFG_SET_BOOL(const universal::Configurator::CfgKey& cfg_key, const std::string& section, const std::string& key, bool value){ 
	if(universal::SingleConfigurator::GetInstance()->SetInt(cfg_key, section, key, (int)value)){
		universal::SingleConfigurator::GetInstance()->SaveCfg(cfg_key);
		return true;
	}
	return false; 
}

inline bool CFG_SET_INT(const std::string& section, const std::string& key, int value){ 
	if(universal::SingleConfigurator::GetInstance()->SetInt(section, key, value)){
		universal::SingleConfigurator::GetInstance()->SaveCfg();
		return true;
	}
	return false; 
}
inline bool CFG_SET_INT(const universal::Configurator::CfgKey& cfg_key, const std::string& section, const std::string& key, int value){ 
	if(universal::SingleConfigurator::GetInstance()->SetInt(cfg_key, section, key, value)){
		universal::SingleConfigurator::GetInstance()->SaveCfg(cfg_key);
		return true;
	}
	return false; 
}

inline bool CFG_SET_DOUBLE(const std::string& section, const std::string& key, double value){ 
	if(universal::SingleConfigurator::GetInstance()->SetDouble(section, key, value)){
		universal::SingleConfigurator::GetInstance()->SaveCfg();
		return true;
	}
	return false; 
}
inline bool CFG_SET_DOUBLE(const universal::Configurator::CfgKey& cfg_key, const std::string& section, const std::string& key, double value){ 
	if(universal::SingleConfigurator::GetInstance()->SetDouble(cfg_key, section, key, value)){
		universal::SingleConfigurator::GetInstance()->SaveCfg(cfg_key);
		return true;
	}
	return false; 
}

inline bool CFG_SET_STRING(const std::string& section, const std::string& key, const std::string &value){ 
	if(CFG_SET_STRING(section, key, value)){
		universal::SingleConfigurator::GetInstance()->SaveCfg();
		return true;
	}
	return false; 
}
inline bool CFG_SET_STRING(const universal::Configurator::CfgKey& cfg_key, const std::string& section, const std::string& key, const std::string &value){ 
	if(CFG_SET_STRING(cfg_key, section, key, value)){
		universal::SingleConfigurator::GetInstance()->SaveCfg(cfg_key);
		return true;
	}
	return false; 
}


#endif 

