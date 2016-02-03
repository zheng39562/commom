/**********************************************************
 * \file logic/other/CSException.h
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _CSException_H
#define _CSException_H

#include <exception>

#include "CSDefine.h"

using namespace std;

/*
 * CSBaseException
 */
namespace CSException{
	enum CSExceptionLevel{ 
		exceptionLv_critical = CSLOG_LVL_CRITICAL,
		exceptionLv_error = CSLOG_LVL_ERROR,
		exceptionLv_warning = CSLOG_LVL_WARN, 
		exceptionLv_info = CSLOG_LVL_INFO, 
		exceptionLv_debug = CSLOG_LVL_DBG
	};
	class CSException : public exception {
		public:
			CSException( const string &_message);
			CSException( const char *_message);
			virtual ~CSException() throw() ;
		protected:
			const string& getMessage() const ;
		private:
			string message;
	};
}  // namespace CSException

/*
 * CSException
 */
namespace CSException{
	class CSLogException : public CSException{
		public:
			CSLogException( CSExceptionLevel _exceptionLv = exceptionLv_error,  
					string _file = __FILE__, int _line= __LINE__, string _funcname = __FUNCTION__ );
			CSLogException( const string &_message, CSExceptionLevel _exceptionLv = exceptionLv_error,  
					string _file = __FILE__, int _line= __LINE__, string _funcname = __FUNCTION__ );
			virtual ~CSLogException() throw();
		private:
			CSExceptionLevel exceptionLv;
			string file;
			int line;
			string funcname;
		public:
			virtual const char* what() const throw();  //override
	};
} // namespace CSException

#endif

