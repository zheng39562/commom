/**********************************************************
 * \file source/tool/StrUtil.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "string_util.h"

#include <sstream>
#include <cctype>
#include <algorithm>

#include "common_tool.h"

using namespace std;
using namespace Inside;
using namespace Universal;

namespace Inside{

	long strToInt( const string &number ){
		stringstream strStream;
		strStream << number;

		long numberTmp;
		strStream >> numberTmp;
		return numberTmp;
	}

	std::string intToStr( const long &number ){
		stringstream strStream;
		strStream << number;
		return strStream.str();
	}
}
//
// 字符和其他格式之间的转换。
//
namespace Universal{

	string doubleToStr( const double &number ){
		stringstream strStream;
		strStream << number;
		return strStream.str();
	}
	

	double strToDouble( const string &number ){
		stringstream strStream;
		strStream << number;

		double numberTmp;
		strStream >> numberTmp;
		return numberTmp;
	}

	string strToXmlStr( string content ){
		// & -> &amp;
		strReplaceAll( content, "&", "&amp;" );
		// < -> &lt;
		strReplaceAll( content, "<", "&lt;" );
		// > -> &gt;
		strReplaceAll( content, ">", "&gt;" );
		// ' -> &apos;
		strReplaceAll( content, "'", "&apos;" );
		// " -> &quot;
		strReplaceAll( content, "\"", "quot;" );

		return content;
	}
	

	string xmlStrToStr( string content ){
		// & -> &amp;
		strReplaceAll( content, "&amp;", "&" );
		// < -> &lt;
		strReplaceAll( content, "&lt;", "<" );
		// > -> &gt;
		strReplaceAll( content, "&gt;", ">" );
		// ' -> &apos;
		strReplaceAll( content, "&apos;", "'" );
		// " -> &quot;
		strReplaceAll( content, "quot;", "\"" );

		return content;
	}


	string ConvertUnicodeToUtf8( int &unicode ){
		// unciode to utf8
		int partHigh = ( unicode & 0x0000F000 ) >> 12;  // XX XX 11110000 XX
		int partMiddle = ( unicode & 0x00000FC0 ) >> 6; // XX XX X1111 1100X
		int partLow = ( unicode & 0x0000003F ) ; // XX XX XX 00111111
		partHigh += 0xE0;
		partMiddle += 0x80;
		partLow += 0x80;

		string oneWord;
		oneWord += (char)(partHigh);
		oneWord += (char)(partMiddle);
		oneWord += (char)(partLow);

		return oneWord;
	}


	void ConvertToUtf8( string &str, const eCharacterEncoding &originalEncoding ){ ; }


	void convertFullToHalf( string &str, eCharacterEncoding encoding ){
		ConvertToUtf8( str, encoding );

	    std::string temp; 
		for (size_t i = 0; i < str.size(); i++) {
			if (((str[i] & 0xF0) ^ 0xE0) == 0) {
				int old_char = (str[i] & 0xF) << 12 | ((str[i + 1] & 0x3F) << 6 | (str[i + 2] & 0x3F));

				if (old_char == 0x3000) { // blank
					char new_char = 0x20;
					temp += new_char;
				} 
				else if (old_char >= 0xFF01 && old_char <= 0xFF5E) { // full char
					char new_char = old_char - 0xFEE0;
					temp += new_char;
				} 
				else { // other 3 bytes char
					temp += str[i];
					temp += str[i + 1];
					temp += str[i + 2];
				}
				i = i + 2;
			} else {
				temp += str[i];
			}
		}
		str = temp;
	}


	void convertHalfToFull( string &str, eCharacterEncoding encoding ){
		ConvertToUtf8( str, encoding );

		string temp;
		int new_char;
		for (size_t i = 0; i < str.size(); ++i ) {
			new_char = str[i];
			if( str[i] >= 0x21 && str[i] <= 0x7E ){
				new_char += 0xFEE0;
				temp += ConvertUnicodeToUtf8( new_char );
			}
			else if( str[i] == 0x20 ){
				new_char = 0x3000;
				temp += ConvertUnicodeToUtf8( new_char );
			}
			else{
				temp += str[i];
			}
		}
		str = temp;
	}


	size_t getStrSize( const string &str, eCharacterEncoding encoding ){
		if( !str.empty() ){
			size_t size = 0;

			size_t currentPos = 0;
			while( currentPos < str.size() ){
				if( (str[currentPos] & 0xFE) == 0xFC ){ currentPos += 6; } 
				else if( ( str[currentPos] & 0xFC ) == 0xF8 ){ currentPos += 5; } 
				else if( ( str[currentPos] & 0xF8 ) == 0xF0 ){ currentPos += 4; } 
				else if( ( str[currentPos] & 0xF0 ) == 0xE0 ){ currentPos += 3; } 
				else if( ( str[currentPos] & 0xE0) == 0xC0 ){ currentPos += 2; } 
				else{ currentPos += 1; }

				++size;
			}

			return size;
		}
		return 0;
	}
} // namespace Universal{


//
// 字符本身的转换。
//
namespace Universal{
	string strToUpper( const string &str ){
		string strTmp = str;
		transform( strTmp.begin(), strTmp.end(), strTmp.begin(), (int (*)(int))tolower );
		return strTmp;
	}


	string strToLower( const string &str ){
		string strTmp = str;
		transform( strTmp.begin(), strTmp.end(), strTmp.begin(), (int (*)(int))toupper );
		return strTmp;
	}


} // namespace Universal{


//
// 字符操作：合并，分割，替换。。。
//
namespace Universal{
	bool strReplaceAll( string &content, const string &replace, const string &dest ){
		string::size_type pos = 0;
		string::size_type replaceSize = replace.size();
		string::size_type destSize = dest.size();
		while( (pos = content.find(replace, pos) ) != string::npos ){
			content.replace( pos, replaceSize, dest );
			pos+=destSize;
		}

		return true;
	}
	bool splitString( const string &strValue, const string &splittingKey, vector<string> &result ){
		result.clear();
		
		if( strValue.empty() ){  
			result.push_back("");
			return true;  
		}

		int count = 0;
		string::size_type curindex = 0;
		string::size_type preindex = 0;
		while( 1){
			if( ++count > strValue.size() )  break;
			
			curindex = strValue.find_first_of( splittingKey, preindex );
			if( curindex == string::npos ){
				if( preindex != string::npos )
					result.push_back( strValue.substr(preindex) );
				return true;
			}
			if( preindex != curindex ){
				result.push_back( strValue.substr(preindex, curindex-preindex ) );
			}
			else{
				result.push_back("");
			}
			preindex = curindex + splittingKey.size();
		}	
		return false;
	}
	bool splitString( const string &strValue, const string &splittingKey, list<string> &result ){
		result.clear();
		
		if( strValue.empty() ){  
			result.push_back("");
			return true;  
		}
		
		int count = 0;
		string::size_type curindex = 0;
		string::size_type preindex = 0;
		while( 1){
			if( ++count > strValue.size() )  break;
			
			curindex =	strValue.find_first_of( splittingKey, preindex);
			if( curindex == string::npos ){
				if( preindex != string::npos )
					result.push_back( strValue.substr(preindex) );
				return true;
			}
			if( preindex != curindex ){
				result.push_back( strValue.substr(preindex, curindex-preindex ) );
			}
			preindex = curindex + splittingKey.size();
		}	
		return false;
	}
	string mergeString( const vector<string> &strings, const string &mergerKey ){
		string strTmp;
		for( vector<string>::const_iterator citer = strings.begin(); citer != strings.end();){
			strTmp += *citer;
			if( ++citer != strings.end() )
				 strTmp += mergerKey;
		}
		return strTmp;
	}


	string trimRight( const string &str ){
		string trimStr = str.substr( 0, str.find_last_not_of(" ")+1 );
		trimStr = str.substr( 0, str.find_last_not_of("　")+1 );
		trimStr = str.substr( 0, str.find_last_not_of("\t")+1 );
		if( !trimStr.empty() ){
			if( trimStr.at( trimStr.length() - 1 ) == '\n' ){
				trimStr = trimStr.substr( 0, trimStr.length() - 1 );
				if( trimStr.at( trimStr.length() - 1 ) == '\r' ){
					trimStr = trimStr.substr( 0, trimStr.length() - 1 );
				}
			}
		}

		return trimStr;
	}


	string trimLeft( const string &str ){
		string trimStr = str.substr( str.find_first_not_of(" ") );
		trimStr = str.substr( str.find_first_not_of("　") );
		trimStr = str.substr( str.find_first_not_of("\t") );
		trimStr = str.substr( str.find_first_not_of("\r\n") );
		trimStr = str.substr( str.find_first_not_of("\n") );
		return trimStr;
	}


}  // namepsace : Universal




