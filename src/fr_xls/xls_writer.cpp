/**********************************************************
 * \file logic/tool/XlsWriter.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "xls_writer.h"

namespace Universal{

	XlsWriter::XlsWriter(){ ; }
	XlsWriter::~XlsWriter(){ ; }


	color_name_t XlsWriter::getXlsColor( const XlsColor &colorIndex ){
		switch( colorIndex ){
			case xlsColor_Red :
				return CLR_RED;
			case xlsColor_Black :
				return CLR_BLACK;
			default:
				return CLR_WHITE;
		}
		return CLR_WHITE;
	}


	bool XlsWriter::writeToFile( const string &filePath, const XlsBookStruct &xlsBook ){
		string folderPath = filePath.substr( 0, filePath.find_last_of('/', filePath.size() )+1 );
		if( !createDir( folderPath ) )  return false;
	
		workbook wb;
		worksheet *ws;
		xf_t *xf = wb.xformat();
		ws = wb.sheet("sheet1");

		for( XlsBookStruct::const_iterator citerSheet = xlsBook.begin(); citerSheet != xlsBook.end(); ++citerSheet ){
			int line( 0), column(0);
			for( XlsSheetStruct::const_iterator citerRow = citerSheet->begin(); citerRow != citerSheet->end(); ++citerRow ){
				++line;
				column = 0;
				for( XlsRowStruct::const_iterator citer = citerRow->begin(); citer != citerRow->end(); ++citer ){
					ws->label( line, column++, *citer, xf);
				}
			}
		}

		wb.Dump( filePath);

		return true;
	}
	bool XlsWriter::writeToFile( const string &filePath, const XlsSheetStruct &xlsSheet, SheetFormat sheetFormat ){
		string folderPath = filePath.substr( 0, filePath.find_last_of('/', filePath.size() )+1 );
		if( !createDir( folderPath ) )  return false;
	
		workbook wb;
		worksheet *ws;
		xf_t *xf = wb.xformat();
		ws = wb.sheet("sheet1");

		cell_t *p_Cell;
		int line(0), column(0);
		for( XlsSheetStruct::const_iterator citerRow = xlsSheet.begin(); citerRow != xlsSheet.end(); ++citerRow ){
			column = 0;
			for( XlsRowStruct::const_iterator citer = citerRow->begin(); citer != citerRow->end(); ++citer ){
				p_Cell = ws->label( line, column, *citer, xf );
				//p_Cell->fontcolor( (xlslib_core::color_name_t)getXlsColor( sheetFormat.getFontColor( line, column ) ) );
				int long_color = getXlsColor( sheetFormat.getFontColor( line, column ) );
//				int long_color = CLR_RED;
				p_Cell->fontcolor( (xlslib_core::color_name_t)(long_color) );
				++column;
			}
			++line;
		}

		wb.Dump( filePath);

		return true;
	}
} // namespace Universal

