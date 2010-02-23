/**********************************************************
 * \file logic/tool/XlsReader.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "xls_reader.h"

#include "xls.h"

using namespace xls;

//
// XlsReader
//
namespace Universal{
	XlsReader::XlsReader()throw(LogException){ ; }
	XlsReader::XlsReader( const string &_filePath)throw(LogException ){
		parseXls( _filePath );
	}
	XlsReader::~XlsReader( ){ ; }


	void XlsReader::parseXls( const string &filePath )throw(LogException ){
		m_SheetArray.clear();

		xlsWorkBook *p_Wb;
		xlsWorkSheet *p_Ws;

		p_Wb = xls_open( filePath.c_str(), "UTF-8" );
		if(  p_Wb == NULL ){
			DEBUG_I("parse failed [ " << filePath << "] ");
			throw LogException( "parse failed : " + filePath );
		}

		for(  int indexOfSheet = 0; indexOfSheet < p_Wb->sheets.count; ++indexOfSheet ){
			p_Ws = xls_getWorkSheet( p_Wb, indexOfSheet );
			xls_parseWorkSheet( p_Ws );

			m_SheetArray.push_back(  XlsSheet(*p_Ws) );

			xls_close_WS( p_Ws );  // 源码示例中没有进行delete，只有close函数。
		}

		xls_close_WB( p_Wb);  // 源码示例中没有进行delete，只有close函数。
	}


	XlsSheet& XlsReader::getSheet( const long &indexOfSheet )throw(LogException ) {
		if(  m_SheetArray.empty() )
			throw LogException( string("this xls is empty."), exceptionLv_info );
		if(  indexOfSheet >= m_SheetArray.size() )
			throw LogException( string("Array bounds (check indexOfSheet)."), exceptionLv_error );

		return m_SheetArray[indexOfSheet ];
	}

	bool XlsReader::read( const string &filePath ){
		parseXls( filePath );
		return true;
	}


	XlsBookStruct& XlsReader::getXlsBook( ){ return m_SheetArray; }
} // namespace Universal

