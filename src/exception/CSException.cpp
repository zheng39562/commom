/**********************************************************
 * \file logic/other/CSException.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "CSException.h"

/*
 * CSException
 */
namespace CSException{
	CSException::CSException( const string &_message)
		:message( _message)
	{;}
	CSException::CSException( const char *_message)
		:message( _message)
	{;}
	CSException::~CSException() throw() { ; }


	const string& CSException::getMessage()const {
		return message;
	}
}  // namespace CSException

/*
 * CSException
 */
namespace CSException{
	CSLogException::CSLogException( CSExceptionLevel _exceptionLv, string _file, int _line, string _funcname )
		:CSException( "" ),
		 exceptionLv( _exceptionLv ),
		 file( _file ),
		 line( _line ),
		 funcname( _funcname )
	{ 
//		DEBUG_E("%s: %s", funcname.c_str(), getMessage().c_str() );
	}
	CSLogException::CSLogException( const string &_message, CSExceptionLevel _exceptionLv, string _file, int _line, string _funcname )
		:CSException( _message ),
		 exceptionLv( _exceptionLv ),
		 file( _file ),
		 line( _line ),
		 funcname( _funcname )
	{ 
//		DEBUG_E("%s: %s", funcname.c_str(), getMessage().c_str() );
	}
	CSLogException::~CSLogException() throw() {;}


	const char* CSLogException::what() const throw(){
		return getMessage().c_str();
	}
} // namespace CSException

