/**********************************************************
 * \file logic/other/Exception.h
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _Exception_H
#define _Exception_H

#include <exception>

#include "common_define.h"

using namespace std;

/*
 * BaseException
 */
namespace Universal{
	enum ExceptionLevel{ 
		exceptionLv_debug,
		exceptionLv_info, 
		exceptionLv_warning, 
		exceptionLv_error,
		exceptionLv_critical
	};
	class Exception : public exception {
		public:
			Exception( const string &_message);
			Exception( const char *_message);
			virtual ~Exception() throw() ;
		protected:
			const string& getMessage() const ;
		private:
			string message;
	};
}  // namespace Exception

/*
 * Exception
 */
namespace Universal{
	class LogException : public Exception{
		public:
			LogException( ExceptionLevel _exceptionLv = exceptionLv_error,  
					string _file = __FILE__, int _line= __LINE__, string _funcname = __FUNCTION__ );
			LogException( const string &_message, ExceptionLevel _exceptionLv = exceptionLv_error,  
					string _file = __FILE__, int _line= __LINE__, string _funcname = __FUNCTION__ );
			virtual ~LogException() throw();
		private:
			ExceptionLevel exceptionLv;
			string file;
			int line;
			string funcname;
		public:
			virtual const char* what() const throw();  //override
	};
} // namespace Exception

#endif

