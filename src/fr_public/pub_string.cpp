/**********************************************************
 * \file source/tool/StrUtil.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "pub_string.h"

#include <sstream>
#include <cctype>
#include <algorithm>

#include "pub_tool.h"

using namespace std;
using namespace fr_public;

//
// 字符和其他格式之间的转换。
//
namespace fr_public{
	string StrToXmlStr(string content){
		// & -> &amp;
		StrReplaceAll(content, "&", "&amp;");
		// < -> &lt;
		StrReplaceAll(content, "<", "&lt;");
		// > -> &gt;
		StrReplaceAll(content, ">", "&gt;");
		// ' -> &apos;
		StrReplaceAll(content, "'", "&apos;");
		// " -> &quot;
		StrReplaceAll(content, "\"", "quot;");

		return content;
	}
	

	string XmlStrToStr(string content){
		// & -> &amp;
		StrReplaceAll(content, "&amp;", "&");
		// < -> &lt;
		StrReplaceAll(content, "&lt;", "<");
		// > -> &gt;
		StrReplaceAll(content, "&gt;", ">");
		// ' -> &apos;
		StrReplaceAll(content, "&apos;", "'");
		// " -> &quot;
		StrReplaceAll(content, "quot;", "\"");

		return content;
	}
	
	string StrToJsonStr(string content){
		StrReplaceAll(content, "\"", "\\\"");
		return content;
	}

	string JsonStrToStr(string content){
		StrReplaceAll(content, "\\\"", "\"");
		return content;
	}


	string ConvertUnicodeToUtf8(int &unicode){
		// unciode to utf8
		int partHigh = (unicode & 0x0000F000) >> 12;  // XX XX 11110000 XX
		int partMiddle = (unicode & 0x00000FC0) >> 6; // XX XX X1111 1100X
		int partLow = (unicode & 0x0000003F) ; // XX XX XX 00111111
		partHigh += 0xE0;
		partMiddle += 0x80;
		partLow += 0x80;

		string oneWord;
		oneWord += (char)(partHigh);
		oneWord += (char)(partMiddle);
		oneWord += (char)(partLow);

		return oneWord;
	}


	void ConvertToUtf8(string &str, const eCharacterEncoding &originalEncoding){ ; }


	void ConvertFullToHalf(string &str, eCharacterEncoding encoding){
		ConvertToUtf8(str, encoding);

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


	void ConvertHalfToFull(string &str, eCharacterEncoding encoding){
		ConvertToUtf8(str, encoding);

		string temp;
		int new_char;
		for (size_t i = 0; i < str.size(); ++i) {
			new_char = str[i];
			if(str[i] >= 0x21 && str[i] <= 0x7E){
				new_char += 0xFEE0;
				temp += ConvertUnicodeToUtf8(new_char);
			}
			else if(str[i] == 0x20){
				new_char = 0x3000;
				temp += ConvertUnicodeToUtf8(new_char);
			}
			else{
				temp += str[i];
			}
		}
		str = temp;
	}


	size_t GetStrSize(const string &str, eCharacterEncoding encoding){
		if(!str.empty()){
			size_t size = 0;

			size_t currentPos = 0;
			while(currentPos < str.size()){
				if((str[currentPos] & 0xFE) == 0xFC){ currentPos += 6; } 
				else if((str[currentPos] & 0xFC) == 0xF8){ currentPos += 5; } 
				else if((str[currentPos] & 0xF8) == 0xF0){ currentPos += 4; } 
				else if((str[currentPos] & 0xF0) == 0xE0){ currentPos += 3; } 
				else if((str[currentPos] & 0xE0) == 0xC0){ currentPos += 2; } 
				else{ currentPos += 1; }

				++size;
			}

			return size;
		}
		return 0;
	}
} // namespace fr_public{


//
// 字符本身的转换。
//
namespace fr_public{
	string StrToUpper(const string &str){
		string strTmp = str;
		transform(strTmp.begin(), strTmp.end(), strTmp.begin(), (int (*)(int))toupper);
		return strTmp;
	}


	string StrToLower(const string &str){
		string strTmp = str;
		transform(strTmp.begin(), strTmp.end(), strTmp.begin(), (int (*)(int))tolower);
		return strTmp;
	}


} // namespace fr_public{


//
// 字符操作：合并，分割，替换。。。
//
namespace fr_public{
	bool StrReplaceAll(string &content, const string &replace, const string &dest){
		string::size_type pos = 0;
		string::size_type replaceSize = replace.size();
		string::size_type destSize = dest.size();
		while((pos = content.find(replace, pos)) != string::npos){
			content.replace(pos, replaceSize, dest);
			pos+=destSize;
		}

		return true;
	}
	bool SplitString(const string &strValue, const string &splittingKey, vector<string> &result){
		result.clear();
		
		if(strValue.empty()){  
			result.push_back("");
			return true;  
		}

		int count = 0;
		string::size_type curindex = 0;
		string::size_type preindex = 0;
		while(1){
			if(++count > strValue.size())  break;
			
			curindex = strValue.find_first_of(splittingKey, preindex);
			if(curindex == string::npos){
				if(preindex != string::npos)
					result.push_back(strValue.substr(preindex));
				return true;
			}
			if(preindex != curindex){
				result.push_back(strValue.substr(preindex, curindex-preindex));
			}
			else{
				result.push_back("");
			}
			preindex = curindex + splittingKey.size();
		}	
		return false;
	}
	bool SplitString(const string &strValue, const string &splittingKey, list<string> &result){
		result.clear();
		
		if(strValue.empty()){  
			result.push_back("");
			return true;  
		}
		
		int count = 0;
		string::size_type curindex = 0;
		string::size_type preindex = 0;
		while(1){
			if(++count > strValue.size())  break;
			
			curindex =	strValue.find_first_of(splittingKey, preindex);
			if(curindex == string::npos){
				if(preindex != string::npos)
					result.push_back(strValue.substr(preindex));
				return true;
			}
			if(preindex != curindex){
				result.push_back(strValue.substr(preindex, curindex-preindex));
			}
			preindex = curindex + splittingKey.size();
		}	
		return false;
	}
	string MergeString(const vector<string> &strings, const string &mergerKey){
		string strTmp;
		for(vector<string>::const_iterator citer = strings.begin(); citer != strings.end();){
			strTmp += *citer;
			if(++citer != strings.end())
				 strTmp += mergerKey;
		}
		return strTmp;
	}


	string TrimRight(const string &str){
		string trimStr = str.substr(0, str.find_last_not_of(" ")+1);
		trimStr = trimStr.substr(0, trimStr.find_last_not_of("\t")+1);
		trimStr = trimStr.substr(0, trimStr.find_last_not_of("\r\n")+1);
		trimStr = trimStr.substr(0, trimStr.find_last_not_of("\n")+1);
		return trimStr;
	}


	string TrimLeft(const string &str){
		string trimStr = str.substr(str.find_first_not_of(" ") == string::npos ? str.size()-1 : str.find_first_not_of(" "));
		trimStr = trimStr.substr(trimStr.find_first_not_of("\t") == string::npos ? str.size()-1 : trimStr.find_first_not_of("\t"));
		trimStr = trimStr.substr(trimStr.find_first_not_of("\r\n") == string::npos ? str.size()-1 : trimStr.find_first_not_of("\r\n"));
		trimStr = trimStr.substr(trimStr.find_first_not_of("\n") == string::npos ? str.size()-1 : trimStr.find_first_not_of("\n"));
		return trimStr;
	}

	string Trim(const string &str){
		return str.empty() ? str : TrimRight(TrimLeft(str));
	}

}  // namepsace : fr_public




