/**********************************************************
 * \file SqlOperator.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "sql_operator.h"

#include "boost/variant.hpp" 

#include "cppconn/connection.h"
#include "cppconn/statement.h"
#include "cppconn/resultset.h"
#include "cppconn/resultset_metadata.h"
#include "fr_public/pub_string.h"


using namespace std;
using namespace boost;
using namespace sql;
using namespace Universal;
//
// SqlOperator
//
namespace SqlSpace{
	SqlOperator::SqlOperator( const string &_host, const int &_port, const string &_user, const string &_pwd, const string &_dbName ) 
		:m_p_Connection(),
		 m_p_Statement()
	{
		setConnection( _host, _port, _user, _pwd, _dbName, 60, 60 );
	}
	SqlOperator::SqlOperator( const string &_host, const int &_port, const string &_user, const string &_pwd, const string &_dbName, 
			const int &_timeout )
		:m_p_Connection(),
		 m_p_Statement()
	{
		setConnection( _host, _port, _user, _pwd, _dbName, _timeout, _timeout );
	}
	SqlOperator::SqlOperator( const string &_host, const int &_port, const string &_user, const string &_pwd, const string &_dbName, 
			const int &_readTimeout, const int &_writeTimeout )
		:m_p_Connection(),
		 m_p_Statement()
	{
		setConnection( _host, _port, _user, _pwd, _dbName, _readTimeout, _writeTimeout );
	}
	SqlOperator::~SqlOperator(){ ; }


	bool SqlOperator::setConnection( const string &host, const int &port, const string &user, const string &pwd, const string &dbName, 
			const int &_readTimeout, const int &_writeTimeout  ){
		sql::mysql::MySQL_Driver *driver;

		driver = sql::mysql::get_mysql_driver_instance();

		ConnectOptionsMap options;
		options.insert( pair< sql::SQLString, ConnectPropertyVal >("hostName", host ) );
		options.insert( pair< sql::SQLString, ConnectPropertyVal >("password", pwd ) );
		options.insert( pair< sql::SQLString, ConnectPropertyVal >("userName", user ) );
		options.insert( pair< sql::SQLString, ConnectPropertyVal >("port", port ) );
		options.insert( pair< sql::SQLString, ConnectPropertyVal >("schema", dbName ) );
		options.insert( pair< sql::SQLString, ConnectPropertyVal >("OPT_READ_TIMEOUT", _readTimeout ) );
		options.insert( pair< sql::SQLString, ConnectPropertyVal >("OPT_WRITE_TIMEOUT", _writeTimeout ) );
		m_p_Connection = boost::shared_ptr<sql::Connection>( driver->connect(options) );
		m_p_Statement = boost::shared_ptr<sql::Statement>( m_p_Connection->createStatement() );

		return m_p_Connection != NULL;
	}


	SQLWhere SqlOperator::convertCond( const map<string, string> &sqlWhere ){
		SQLWhere sqlWhereTmp;
		for( map<string, string>::const_iterator citer = sqlWhere.begin(); citer != sqlWhere.end(); ++citer ){
			sqlWhereTmp += " and " + citer->first + " = " + quotedStr( citer->second );
		}
		return sqlWhereTmp;
	}


	bool SqlOperator::execSQL( const string &sqlCmd ){
		return m_p_Statement->execute( sqlCmd );
	}


	bool SqlOperator::execQuery( const string &sqlCmd, TableDatas &datas ){
		scoped_ptr<sql::ResultSet> pResult( m_p_Statement->executeQuery( sqlCmd ) );

		sql::ResultSetMetaData* pResultMetaData = pResult->getMetaData();

		while( pResult->next() ){
			TableData sqlData;
			for( unsigned int columnIndex = 1; columnIndex <= pResultMetaData->getColumnCount(); ++columnIndex ){
				sqlData.insert( TableFieldData( pResultMetaData->getColumnName(columnIndex), pResult->getString(columnIndex) ) );
			}
			datas.push_back( sqlData );
		}
		return true;
	}
} // namespace Universal{


