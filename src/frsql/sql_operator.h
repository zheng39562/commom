/**********************************************************
 * \file SqlOperator.h
 * \brief 
 * \note	ע�����
 *			1,���еĳ�Ա�����е�ͬ�������ĺ�����ȫ��ͬ������ע������һ���������������������ظ�ע�͡������Ĳ������岻ͬʱ�������ע�⡣
 *			2,��1���Ĺ���ͬ�������ڷ���ֵ�ĺ��塣
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _SqlOperator_H
#define _SqlOperator_H

#include <memory>
#include <thread>
#include <mutex>

#include "mysql_driver.h"
#include "mysql_connection.h"

namespace frsql{
	class SqlOprtPool;

	struct SqlConnectOption{
		public:
			SqlConnectOption(){
				read_timeout = 60 * 1000;
				write_timeout = 60 * 1000;
			}
			~SqlConnectOption()=default;
		public:
			std::string host;
			int port;
			std::string user;
			std::string pwd; 
			std::string db_name;
			int read_timeout;
			int write_timeout; 
	};

	//! \brief	sql����ת��������
	std::string convertCond(const std::map<std::string, std::string> &sqlWhere);
	std::string BuildQueryCmd(const std::string& table_name, const std::map<std::string, std::string>& condition);
	std::string BuildCreateCmd(const std::string& table_name, const std::map<std::string, std::string>& fields);
	std::string BuildUpdateCmd(const std::string& table_name, const std::map<std::string, std::string>& condition, const std::map<std::string, std::string>& fields);
	std::string BuildDeleteCmd(const std::string& table_name, const std::map<std::string, std::string>& condition);

	//! \brief	���ݿ�����࣬����Ϊ���̰߳�ȫ�ࡣ
	//! \note	�ṩ�����������ӵķ�ʽ�����캯�� �� �趨������
	//!			ע�⣺δ�ɹ��������ݿ�ʱ���κ�sql�������᷵���쳣��
	//! \note	������֧�ֿ���ѯ�����������ֿ��ӿ�: execSQL
	//! \todo	½��֧������mysql��ز�������
	//! \todo	����mysql�������������������ݿ��׼������
	class SqlOperator{
		public:
			friend class SqlOprtPool;
		public:
			SqlOperator(const SqlConnectOption& option);
			~SqlOperator();
		public:
			//! \param[in] datas ����ӵ�з���ֵ��select�����Ч������������䷵�ؿա�
			bool ExecQuery(const std::string &sqlCmd, std::map<std::string, std::string> &datas);
			//! \param[in] datas ����ӵ�з���ֵ��select�����Ч������������䷵�ؿա�
			bool ExecQuery(const std::string &sqlCmd, std::vector<std::map<std::string, std::string> > &datas);
			//! \brief	����Ӱ������
			bool ExecUpdate(const std::string &sqlCmd, int &updateCount);

			//! \brief	
			bool IsExist(const std::string& table_name, const std::map<std::string, std::string>& condition);
			//! \brief	
			//! \retval true ��ʾ��ѯ�����ݡ� false �����ǲ�ѯʧ�ܣ�Ҳ�����ǲ�ѯ��������.
			bool QueryItem(const std::string& table_name, const std::map<std::string, std::string>& condition, std::map<std::string, std::string>& fields);
			bool QueryItems(const std::string& table_name, const std::map<std::string, std::string>& condition, std::vector<std::map<std::string, std::string> >& fields);
			//! \brief	
			//! \retval true create success�� false create falied.
			bool CreateItem(const std::string& table_name, const std::map<std::string, std::string>& fields);
			//! \brief	
			bool UpdateItem(const std::string& table_name, const std::map<std::string, std::string>& condition, const std::map<std::string, std::string>& fields);
			//! \brief	
			bool DeleteItem(const std::string& table_name, const std::map<std::string, std::string>& condition);
		private:
			//! \brief	�������ݿ����ӡ�
			void SetConnection(const SqlConnectOption& option);
			//! \brief	�������״̬�������ӶϿ�ʱ�������Զ�����
			bool CheckConnectAndReconnect();
		private:
			boost::shared_ptr<sql::Connection>			connection_;		//! 
			boost::shared_ptr<sql::Statement>			statement_;			//! 
			sql::ConnectOptionsMap						sql_options_;
			std::mutex									mutex_connect_;
	};
} //  namespace universal{


#endif 

