/**********************************************************
 * \file logic/tool/CSXlsStruct.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "CSXlsStruct.h"

//
// SheetFormat
//
namespace Universal{
	SheetFormat::SheetFormat()
		:m_ColumnFormat( map<unsigned long, CSXlsColorFormat>() ),
		 m_RowFormat( map<unsigned long, CSXlsColorFormat>() ),
		 m_DefaultBackColor( xlsColor_White ),
		 m_DefaultFontColor( xlsColor_Black )
	{ ; }
	SheetFormat::~SheetFormat(){ ; }


	void SheetFormat::addColor( map<unsigned long, CSXlsColorFormat> &format,  const unsigned long &index, const CSXlsColorFormat &colorFormat ){
		if( format.find(index) != format.end() ){
			format.find(index)->second = colorFormat;
		}
		else{
			format.insert(pair<unsigned long, CSXlsColorFormat>( index, colorFormat ) );
		}
	}


	CSXlsColor SheetFormat::getBackColor( const unsigned long &row, const unsigned long &column ){
		return getColor( row, column ).backColor;
	}


	CSXlsColor SheetFormat::getFontColor( const unsigned long &row, const unsigned long &column ){
		return getColor( row, column ).fontColor;
	}


	CSXlsColorFormat SheetFormat::getColor( const unsigned long &row, const unsigned long &column ){
		if( m_RowFormat.find(row) != m_RowFormat.end() ){
			return m_RowFormat.find(row)->second;
		}
		else if( m_ColumnFormat.find(column) != m_ColumnFormat.end() ){
			return m_ColumnFormat.find(column)->second;
		}
		else{
			CSXlsColorFormat colorFormat;
			colorFormat.backColor = m_DefaultBackColor;
			colorFormat.fontColor = m_DefaultFontColor;
			return colorFormat;
		}
	}


	void SheetFormat::setRowColor( const unsigned long &row, const CSXlsColorFormat &colorFormat ){
		addColor( m_RowFormat, row, colorFormat );
	}


	void SheetFormat::setColumnColor( const unsigned long &column, const CSXlsColorFormat &colorFormat ){
		addColor( m_ColumnFormat, column, colorFormat );
	}
} // namespace Universal

//
// CSXlsSheet
//
namespace Universal{
#define _FUNCPOS_XLS_SHEET							CS_COMMON_FUNC_( "CSXlsSheet")

	CSXlsSheet::CSXlsSheet( const xlsWorkSheet &_xlsSheet )
		:m_MaxRows( 0),
		 m_MaxColumns( 0),
		 m_XlsContent( )
	{
		parseXlsSheet( _xlsSheet );
	}
	CSXlsSheet::CSXlsSheet( )
		:m_MaxRows( 0),
		 m_MaxColumns( 0),
		 m_XlsContent( )
	{;}
	CSXlsSheet::~CSXlsSheet( ){ ; }


	void CSXlsSheet::parseXlsSheet( const xlsWorkSheet &xlsSheet ){
		m_MaxRows = xlsSheet.rows.lastrow + 1;

		m_MaxColumns = xlsSheet.rows.lastcol;
#ifdef COMMON_DEBUG
		DEBUG_D( "%s: rows [%d], columns [%d] ", _FUNCPOS_XLS_SHEET, m_MaxRows, m_MaxColumns );
#endif

		CSXlsRowStruct tmpRow;
		tmpRow.reserve( m_MaxColumns );
		struct st_row::st_row_data *p_Row;
		for( int indexOfRow = 0; indexOfRow < m_MaxRows; ++indexOfRow ){
			tmpRow.clear();
			p_Row = &( xlsSheet.rows.row[indexOfRow] );
			for( int indexOfColumn = 0; indexOfColumn < m_MaxColumns; ++indexOfColumn ){
				if( p_Row->cells.cell[indexOfColumn].str != NULL ){
					string xlsValue = reinterpret_cast<char*>( p_Row->cells.cell[indexOfColumn].str );
					// 三方库转换数字问题：例如，1被转换为1.000000。需要去除后面额外添加的零。
					if( checkNumber( xlsValue ) ){
						xlsValue = xlsValue.substr( 0, xlsValue.find(".") );
					}
					tmpRow.push_back( xlsValue );

#ifdef DETAIL_DEBUG
					DEBUG_D( "%s: orignal content [%s]", _FUNCPOS_XLS_SHEET, p_Row->cells.cell[indexOfColumn].str );
					DEBUG_D( "%s: convert content [%s]", _FUNCPOS_XLS_SHEET, xlsValue.c_str() );
#endif
				}
				else{
					tmpRow.push_back("");
				}
			}
			m_XlsContent.push_back( tmpRow );
		}
#ifdef DETAIL_DEBUG
		for( CSXlsSheetStruct::const_iterator citerRow = m_XlsContent.begin(); citerRow != m_XlsContent.end(); ++citerRow ){
			string line;
			for( CSXlsRowStruct::const_iterator citerColumn = citerRow->begin(); citerColumn != citerRow->end(); ++citerColumn ){
				line += "  [" + *citerColumn + "]  ";
			}
			DEBUG_D( "%s: one line === %s ===", _FUNCPOS_XLS_SHEET, line.c_str() );
		}
#endif
		DEBUG_D( "%s: end sheet parse", _FUNCPOS_XLS_SHEET );
	}
} // namespace Universal


