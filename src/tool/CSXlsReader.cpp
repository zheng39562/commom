/**********************************************************
 * \file logic/tool/CSXlsReader.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "CSXlsReader.h"

#include "xls.h"

using namespace xls;

//
// CSXlsReader
//
namespace Universal{
	#define _FUNCPOS_XLS_READER							CS_COMMON_FUNC_( "CSXlsReader")
	CSXlsReader::CSXlsReader()throw(CSLogException){ ; }
	CSXlsReader::CSXlsReader( const string &_filePath)throw(CSLogException ){
		parseXls( _filePath );
	}
	CSXlsReader::~CSXlsReader( ){ ; }


	void CSXlsReader::parseXls( const string &filePath )throw(CSLogException ){
		m_SheetArray.clear();

		xlsWorkBook *p_Wb;
		xlsWorkSheet *p_Ws;

		p_Wb = xls_open( filePath.c_str(), "UTF-8" );
		if(  p_Wb == NULL ){
			DEBUG_I( "%s: parse failed [%s] ", _FUNCPOS_XLS_READER, filePath.c_str() );
			throw CSLogException( "parse failed : " + filePath );
		}

		for(  int indexOfSheet = 0; indexOfSheet < p_Wb->sheets.count; ++indexOfSheet ){
			p_Ws = xls_getWorkSheet( p_Wb, indexOfSheet );
			xls_parseWorkSheet( p_Ws );

			m_SheetArray.push_back(  CSXlsSheet(*p_Ws) );

			xls_close_WS( p_Ws );  // 源码示例中没有进行delete，只有close函数。
		}

		xls_close_WB( p_Wb);  // 源码示例中没有进行delete，只有close函数。
	}


	CSXlsSheet& CSXlsReader::getSheet( const long &indexOfSheet )throw(CSLogException ) {
		if(  m_SheetArray.empty() )
			throw CSLogException( string("this xls is empty.") + _FUNCPOS_XLS_READER, exceptionLv_info );
		if(  indexOfSheet >= m_SheetArray.size() )
			throw CSLogException( string("Array bounds (check indexOfSheet).") + _FUNCPOS_XLS_READER, exceptionLv_error );

		return m_SheetArray[indexOfSheet ];
	}

	bool CSXlsReader::read( const string &filePath ){
		parseXls( filePath );
		return true;
	}


	CSXlsBookStruct& CSXlsReader::getXlsBook( ){ return m_SheetArray; }
} // namespace Universal

