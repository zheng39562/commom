/**********************************************************
 * \file sql_operator_pool.h
 * \brief 
 * \note	ע�����
 *			1,���еĳ�Ա�����е�ͬ�������ĺ�����ȫ��ͬ������ע������һ���������������������ظ�ע�͡������Ĳ������岻ͬʱ�������ע�⡣
 *			2,��1���Ĺ���ͬ�������ڷ���ֵ�ĺ��塣
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef sql_operator_pool_H
#define sql_operator_pool_H

#include <mutex>
#include "sql_operator.h"
#include "frtemplate/single_mode.hpp"

namespace frsql{
	typedef std::shared_ptr<SqlOperator> SqlOperatorPtr;
	//! \brief	���ݿ����ӳ�
	//! \note	�÷���
	//				* ���ó�ʼ����Initialize(...);
	//				* ��ѯor����
	class SqlOprtPool{
		public:
			SqlOprtPool();
			~SqlOprtPool();
		public:
			void Initialize(const std::string &host, const int &port, const std::string &user, const std::string &pwd, const std::string &db_name, 
					const int read_timeout, const int write_timeout, int pool_size);
			//! \brief	��ѯ���ʹ��
			//! \note	�ú��������ӳ�Ϊ��ʱ����ס�̡߳�ͨ�������������л��ѡ�
			//!			���̵߳����ʱ��ͨ��Ϊ��ʱʱ��(readTimerout)�������Գ�����
			//! \param[in] datas ����ӵ�з���ֵ��select�����Ч������������䷵�ؿա�
			bool ExecQuery(const std::string &sql_cmd, std::map<std::string, std::string> &datas);
			bool ExecQuery(const std::string &sql_cmd, std::vector<std::map<std::string, std::string> > &datas);
			//! \brief	��update/insert/delete�Լ��������ݵ������ʹ��.
			//! \note	�ú��������ӳ�Ϊ��ʱ����ס�̡߳�ͨ�������������л��ѡ�
			//!			���̵߳����ʱ��ͨ��Ϊ��ʱʱ��(writeTimerout)�������Գ�����
			//! \retval	����Ӱ������
			bool ExecUpdate(const std::string &sql_cmd, int &updateCount);

			bool IsExist(const std::string& table_name, const std::map<std::string, std::string>& condition);
			bool QueryItem(const std::string& table_name, const std::map<std::string, std::string>& condition, std::map<std::string, std::string>& fields);
			bool QueryItems(const std::string& table_name, const std::map<std::string, std::string>& condition, std::vector<std::map<std::string, std::string> >& fields);
			bool CreateItem(const std::string& table_name, const std::map<std::string, std::string>& fields);
			bool UpdateItem(const std::string& table_name, const std::map<std::string, std::string>& condition, const std::map<std::string, std::string>& fields);
			bool DeleteItem(const std::string& table_name, const std::map<std::string, std::string>& condition);
		private:
			 SqlOperatorPtr popSqlOprtPtr(int timeout);
			 void pushSqlOprtPtr(SqlOperatorPtr pSqlOperator);
		private:
			std::list<SqlOperatorPtr > sql_operator_pool_;
			std::mutex mutex_sql_operator_;
			int read_timeout_;
			int write_timeout_;
	};
	typedef frtemplate::SingleMode<SqlOprtPool> SingleSqlOprtPool;
}

void ConnectSql(const std::string &host, const int &port, const std::string &user, const std::string &pwd, const std::string &db_name, const int read_timeout, const int write_timeout, int pool_size);
std::shared_ptr<frsql::SqlOprtPool> GetSqlOprtPool();

#endif 

