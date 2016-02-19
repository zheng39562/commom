/**********************************************************
 * \file logic/other/Exception.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "common_exception.h"

/*
 * Exception
 */
namespace Universal{
	Exception::Exception( const string &_message)
		:message( _message)
	{;}
	Exception::Exception( const char *_message)
		:message( _message)
	{;}
	Exception::~Exception() throw() { ; }


	const string& Exception::getMessage()const {
		return message;
	}
}  // namespace Exception

/*
 * Exception
 */
namespace Universal{
	LogException::LogException( ExceptionLevel _exceptionLv, string _file, int _line, string _funcname )
		:Exception( "" ),
		 exceptionLv( _exceptionLv ),
		 file( _file ),
		 line( _line ),
		 funcname( _funcname )
	{ 
//		DEBUG_E("%s: %s", funcname.c_str(), getMessage().c_str() );
	}
	LogException::LogException( const string &_message, ExceptionLevel _exceptionLv, string _file, int _line, string _funcname )
		:Exception( _message ),
		 exceptionLv( _exceptionLv ),
		 file( _file ),
		 line( _line ),
		 funcname( _funcname )
	{ 
//		DEBUG_E("%s: %s", funcname.c_str(), getMessage().c_str() );
	}
	LogException::~LogException() throw() {;}


	const char* LogException::what() const throw(){
		return getMessage().c_str();
	}
} // namespace Exception

