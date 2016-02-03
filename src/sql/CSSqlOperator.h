/**********************************************************
 * \file CSSqlOperator.h
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _CSSqlOperator_H
#define _CSSqlOperator_H

#include "CSDefine.h"
#include "CSException.h"

namespace Universal{
	//! \brief	数据库的管理器：获取数据库操作指针。
	//! \note	采用线程池方案。
	class CSSqlOprtManager{
		public:
			CSSqlOprtManager()=default;
			~CSSqlOprtManager()=default;
	};

	typedef std::map<string, string>	CSTableData;
	typedef std::vector<CSTableData>	CSTableDatas;
	typedef std::string					CSSQLWhere;
	//! \brief	数据库操作者。
	//! \note	提供两种设置连接的方式：构造函数 和 设定函数。
	//!			注意：未成功连接数据库时，任何sql操作都会返回异常。
	//! \note	设计理念不支持跨库查询。但保留部分跨库接口: execSQL
	//! \todo	陆续支持所有mysql相关操作集。
	//! \todo	优先mysql。后续会增加其他数据库标准草组。
	class CSSqlOperator{
		public:
			CSSqlOperator();
			CSSqlOperator( const string &_host, const string &_port, const string &_user, const string &_pwd, const string &_dbName ) throw(CSException) ;
			~CSSqlOperator()=default;
		private:
		public:
			//! \brief	设置数据库链接。
			bool setConnection( const string &host, const string &port, const string &user, const string &pwd, const string &dbName ) throw(CSException) ;

			//! \brief	sql条件转换函数。
			CSSQLWhere convertCond( const map<string, string> &sqlWhere ) throw(CSException) ;

			//! \brief	查询
			bool selectData( const string &tbName, const CSSQLWhere &sqlWhere, CSTableData &data ) throw(CSException) ;

			//! \brief	创建。
			bool insertData( const string &tbName, const CSTableData &data ) throw(CSException) ;

			//! \brief	删除
			bool deleteData( const string &tbName, const CSSQLWhere &sqlWhere ) throw(CSException) ;

			//! \brief	更新
			bool updateData( const string &tbName, const CSSQLWhere &sqlWhere, const CSTableData &data ) throw(CSException) ;

			//! \brief	执行SQL语句：可跨库查询。
			//! \note	不限制跨库语句。但需要当前用户拥有对应权限。
			bool execSQL( const string &sql ) throw(CSException) ;
			//! \param[in] datas 仅对拥有返回值的select语句有效。其他复合语句返回空。
			bool execSQL( const string &sql, CSTableDatas &datas ) throw(CSException) ;
	};
} //  namespace Universal{


#endif 

