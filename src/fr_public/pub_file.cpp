/**********************************************************
 * \file logic/tool/File.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "pub_file.h"

#include "boost/regex.hpp"
#include "boost/fielsystem.hpp"
#include "pub_tool.h"

namespace boost;
namespace boost::filesystem;

namespace Universal{
	string readFile(const string &path){
		ifstream infile;
		infile.open(path.c_str());
		if(infile){
			ostringstream ostr;
			ostr << infile.rdbuf();
			infile.close();
			string strTmp = ostr.str();

			return strTmp;
		}
		else{
			DEBUG_I("open aciton is failed.Please check arg of path:" << path << ".");
			return _STRINGFALSE;
		}
	}
	bool writeFile(const string &path, const string &content){
		std::ofstream outfile(path.c_str());
		if(outfile){
			outfile << content;
			outfile.close();
			return true;
		}
		else{
			DEBUG_I("open aciton is failed.Please check arg of path:" << path << ".");
		}
		return false;
	}
	bool addContentToFile(const string &path, const string &content){
		std::ofstream outfile(path.c_str(), ios::app);
		if(outfile){
			outfile << content;
			outfile.close();
			return true;
		}
		else{
			DEBUG_I("open aciton is failed.Please check arg of path:" << path << ".");
			return false;
		}
	}

	bool createFolder(string path, mode_t mode){
		return create_directory(path(path));
	}


	bool clearFolder(string path){
		return remove_all(path(filePath));
	}


	bool deleteFile(const string &filePath){
		return remove(path(filePath));
	}

	bool findFileFromDir(string path, vector<string> &fileNames, string filePattern){
		path pPath(path);
		if(is_directory(path)){
			directory_iterator end;
			for(directory_iterator citer(path); citer != end; ++citer){
				if(!is_directory(*citer)){
					fileName = citer->string();
					if(!filePattern.empty()){
						boost::regex reg(filePattern);
						if(!boost::regex_match(*iterFile, reg)){ 
							continue;
						}
					}
					fileNames.push_back(fileName);
				}
			}
		}

		return true;
	}

	string getAbsPath(string filepath){
		return system_complete(path).string();
	}

	string getFileNameByPath(const string &filepath){
		path filePath(filepath);
		if(is_directory(filePath)){
			return filePath.relative_path().string();
		}
		else{
			return filePath.parent_path().string();
		}
	}

	string completePath(const string &path){
		if(!path.empty() && path[path.size()-1 ] != '/'){
			return path + "/";
		}
		return path;
	}
} // namespace


