/**********************************************************
 * \file logic/JsonTool.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "json_tool.h"

#include "common/common_tool.h"

namespace Universal{
#define _JSONTOOL_FUNC_								_COMMON_FUNC_( "JsonTool")
	string JsonTool::indexOfSplittingPath = _SPLITKEY_POINT;
	string JsonTool::indexOfSplittingKey = _SPLITKEY_COMMA;

	JsonTool::JsonTool( ){;}
	JsonTool::~JsonTool( ){;}
	bool JsonTool::parseJs( const string &jsStr, Json::Value &jsValue ){
		Json::Reader jsReader;
		return jsReader.parse(  jsStr, jsValue );
	}


	Json::Value JsonTool::getJsValue( const Json::Value &jsValue, const string &path ){
		Json::Value jsValueTmp = jsValue;
		Json::Value* jsValueTmp_p = findJsValue( jsValueTmp, path );
		if(  jsValueTmp_p != NULL )
			return *jsValueTmp_p;
		else
			return Json::Value( );
	}

	
	Json::Value JsonTool::getJsValue( const Json::Value &jsArray, const string &key, const string &value ){
		Json::Value jsArrayTmp = jsArray;
		Json::Value* jsValueTmp_p = findJsValue( jsArrayTmp, key, value );
		if(  jsValueTmp_p != NULL )
			return *jsValueTmp_p;
		else
			return Json::Value( );
	}


	Json::Value* JsonTool::findJsValue( Json::Value &jsValue, const string &path ){
		if(  path.size() == 0 ) return &jsValue;

		vector<string> keys;
		if(  !parseKeyPath( path, keys ) )  return NULL;

		string error( "");
		Json::Value* jsFoundValue_p = &jsValue;
		for(  vector<string>::iterator iterKey = keys.begin(); iterKey != keys.end(); ++iterKey ){
			error += " key --> " + *iterKey + " ||";
			jsFoundValue_p = findJsValueByKey( *jsFoundValue_p, *iterKey );
			if(  jsFoundValue_p == NULL ){
				DEBUG_D("found a empty json value.key path is " << path << ".already go [" << error << "]");
				return NULL;
			}
		}  //for
		return jsFoundValue_p;
	}


	Json::Value* JsonTool::findJsValue( Json::Value &jsArray, const string &key, const string &value ){
		if(  jsArray.isArray() ){
			long jsCount = jsArray.size( );
			for(  int i=0; i<jsCount; ++i ){
				Json::Value::Members vKeys = jsArray[i].getMemberNames( );
				for(  Json::Value::Members::iterator iterJson = vKeys.begin(); iterJson != vKeys.end(); ++iterJson ){
					if(  key == (*iterJson) ){
						if(  jsArray[i][*iterJson].isString() && jsArray[i][*iterJson].asString() == value )
							return &jsArray[i];
					}
				} //for
			} //for
		}
		return NULL;
	}


	string JsonTool::getJsString( const Json::Value &jsValue, string path ){
		Json::Value jsValueTmp = getJsValue( jsValue, path );
		string value( _STRINGFALSE );
		if( jsValueTmp.isString() )
			value = jsValueTmp.asString( );
		else if( jsValueTmp.isUInt() || jsValueTmp.isInt() )
			value = intToStr( jsValueTmp.asUInt() );
		else if( jsValueTmp.isDouble() )
			value = intToStr( (long)jsValueTmp.asDouble() );
		else
			return value;
		return value;
	}
	bool JsonTool::getJsStrList( const Json::Value &jsArray, const string &key, vector<string> &strList ){
		strList.clear( );
		int jsCount = jsArray.size( );
		for(  int i=0; i<jsCount; ++i )
			strList.push_back( getJsString(jsArray[i], key ) );
		return true;
	}
	bool JsonTool::convertJsToMap( const Json::Value &jsValue, map<string, string> &mapResult, string keyList ){
		if(  jsValue.isArray() ){
			mapResult.clear( );
			mapResult.insert( pair<string,string>("array", getJsString(jsValue)) );
			DEBUG_D( "Json is a array.");
			return false;
		}
		if(  jsValue.isNull() ){
			DEBUG_D( "Json data has empty.");
			return false;
		}
		
		bool doFilterData = false;
		vector<string> needKeys;
		if(  keyList.size() != 0 ){
			doFilterData = true;
			if(  !parseKeyList(keyList, needKeys ) )  return false;
		}

		mapResult.clear( );
		Json::Value::Members vKeys = jsValue.getMemberNames( );
		for(  Json::Value::Members::iterator iterKey = vKeys.begin(); iterKey != vKeys.end(); ++iterKey ){
			if(  !doFilterData )
				mapResult.insert( pair<string,string>(*iterKey, getJsString(jsValue[*iterKey])) );
			else if(  std::find(needKeys.begin(), needKeys.end(), *iterKey ) != needKeys.end() )
				mapResult.insert( pair<string,string>(*iterKey, getJsString(jsValue[*iterKey])) );
		}  // for 
		return true;
	}
	bool JsonTool::convertJsToMapList( const Json::Value &jsValue, vector<map<string, string> > &mapListResult, string keyList ){
		mapListResult.clear( );
		map<string, string> content;
		if(  jsValue.isArray() ){
			for(  int i=0; i<jsValue.size(); ++i ){
				convertJsToMap( jsValue[i], content, keyList );
				mapListResult.push_back( content);
			}
		}
		else{
			DEBUG_D( "Json value is not a array." );
			return false;
		}
		return true;
	}
	bool JsonTool::convertJsToMapList( const Json::Value &jsValue, list<map<string, string> > &mapListResult, string keyList ){
		mapListResult.clear( );
		map<string, string> content;
		if(  jsValue.isArray() ){
			for(  int i=0; i<jsValue.size(); ++i ){
				convertJsToMap( jsValue[i], content, keyList );
				mapListResult.push_back( content);
			}
		}
		else{
			DEBUG_D( "Json value is not a array." );
			return false;
		}
		return true;
	}
	string JsonTool::convertMapToJsStr( const map<string, string> mapData ){
		string defaultJsStr = "\t{\n";

		map<string, string>::const_iterator citerJsItem = mapData.begin( );
		while(  citerJsItem != mapData.end() ){
			string valueTmp = citerJsItem->second;
			Universal::strReplaceAll( valueTmp, "\"", "\\\"" );
			Universal::strReplaceAll( valueTmp, "\r", "\\r" );
			Universal::strReplaceAll( valueTmp, "\n", "\\n" );
			convertFullToHalf( valueTmp );

			defaultJsStr += "\t\t\"" + citerJsItem->first + "\" : \"" + valueTmp + "\"";
			if(  ++citerJsItem == mapData.end() )
				defaultJsStr += "\n";
			else
				defaultJsStr += ",\n";
		}

		defaultJsStr += "\t}";
		
		return defaultJsStr;
	}
	string JsonTool::convertMapListToJsStr( const vector<map<string, string> > &mapList, string arrayName ){
		if(  mapList.size() == 0 )  return "";

		string defaultJsStr;
		defaultJsStr += "[\n";
		for( vector<map<string,string> >::const_iterator citerJsBlock = mapList.begin(); citerJsBlock != mapList.end(); ){
			defaultJsStr += JsonTool::convertMapToJsStr( *citerJsBlock );
			if(  ++citerJsBlock != mapList.end() )
				defaultJsStr += ",";
		}
		defaultJsStr += "\n]\n";

		if(  arrayName.size() != 0 ){
			defaultJsStr = "{\n\t\"" + arrayName + "\" : " + defaultJsStr + "}";
		}
		return defaultJsStr;
	}
	string JsonTool::convertMapListToJsStr( const list<map<string, string> > &mapList, string arrayName ){
		if(  mapList.size() == 0 )  return "";

		string defaultJsStr;
		defaultJsStr += "[\n";
		for( list<map<string,string> >::const_iterator citerJsBlock = mapList.begin(); citerJsBlock != mapList.end(); ){
			defaultJsStr += JsonTool::convertMapToJsStr( *citerJsBlock );
			if(  ++citerJsBlock != mapList.end() )
				defaultJsStr += ",";
		}
		defaultJsStr += "\n]\n";

		if(  arrayName.size() != 0 ){
			defaultJsStr = "{\n\t\"" + arrayName + "\" : " + defaultJsStr + "}";
		}
		return defaultJsStr;
	}
	bool JsonTool::parseKeyPath( const string &path, vector<string> &keys ){
		return Universal::splitString( path, indexOfSplittingPath, keys );
	}
	bool JsonTool::parseKeyList( const string &keyList, vector<string> &keys ){
		if(  !Universal::splitString(keyList, indexOfSplittingKey, keys ) ){
			DEBUG_D( "key list format is wrong.key list is [ " << keyList << " ]");
			return false;
		}
		return true;
	}
	Json::Value* JsonTool::findJsValueByKey( Json::Value &jsValue, string &key ){
		Json::Value *jsValue_p = &jsValue;
		if(  jsValue_p->isString() || jsValue_p->empty() || jsValue_p->isNull() )
			return NULL;
		if(  jsValue_p->isArray() ){
			if( !checkNumber(key) ){
				DEBUG_D( "Key is not a number. Key name is " << key);
				return NULL;
			}
			int index = strToInt( key);
			if(  index >= jsValue_p->size() ) return NULL;
			return &( *jsValue_p)[index];
		}
		else{
			Json::Value::Members vKeys = jsValue_p->getMemberNames( );
			for(  Json::Value::Members::iterator iterJson = vKeys.begin(); iterJson != vKeys.end(); ++iterJson ){
				if(  key == (*iterJson) ){
					return &( *jsValue_p)[key];
				}
			} //for
		}
		return NULL;
	}
} // namespace Universal 

