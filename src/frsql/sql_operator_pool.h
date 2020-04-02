/**********************************************************
 * \file sql_operator_pool.h
 *  
 * 	ע�����
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
	// ���ݿ����ӳ�
	// �÷����飺sql_example.cpp
	class SqlOprtPool{
		public:
			SqlOprtPool(const std::string& name, const SqlConnectOption& option, int pool_size);
			~SqlOprtPool();
		public:
			// 	��ѯ���ʹ��
			// 	�ú��������ӳ�Ϊ��ʱ����ס�̡߳�ͨ�������������л��ѡ�
			//			���̵߳����ʱ��ͨ��Ϊ��ʱʱ��(readTimerout)�������Գ�����
			// \param[in] datas ����ӵ�з���ֵ��select�����Ч������������䷵�ؿա�
			bool ExecQuery(const std::string &sql_cmd, std::map<std::string, std::string> &datas);
			bool ExecQuery(const std::string &sql_cmd, std::vector<std::map<std::string, std::string> > &datas);
			// 	��update/insert/delete�Լ��������ݵ������ʹ��.
			// 	�ú��������ӳ�Ϊ��ʱ����ס�̡߳�ͨ�������������л��ѡ�
			//			���̵߳����ʱ��ͨ��Ϊ��ʱʱ��(writeTimerout)�������Գ�����
			// \retval	����Ӱ������
			bool ExecUpdate(const std::string &sql_cmd, int &updateCount);

			bool IsExist(const std::string& table_name, const std::map<std::string, std::string>& condition);
			bool QueryItem(const std::string& table_name, const std::map<std::string, std::string>& condition, std::map<std::string, std::string>& fields);
			bool QueryItems(const std::string& table_name, const std::map<std::string, std::string>& condition, 
					std::vector<std::map<std::string, std::string> >& fields);
			bool CreateItem(const std::string& table_name, const std::map<std::string, std::string>& fields);
			bool UpdateItem(const std::string& table_name, const std::map<std::string, std::string>& condition, 
					const std::map<std::string, std::string>& fields);
			bool DeleteItem(const std::string& table_name, const std::map<std::string, std::string>& condition);

			inline const std::string& name()const{ return name_; }
		private:
			 SqlOperator* popSqlOprtPtr();
			 void pushSqlOprtPtr(SqlOperator* pSqlOperator);
		private:
			const std::string name_;
			std::list<SqlOperator*> sql_operator_pool_;
			std::mutex mutex_sql_operator_;
	};
}

#endif 

