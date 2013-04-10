#include "FileTools.hpp"

std::string readFile(const std::string& filePath){
	std::string xml = "";

	std::ifstream xmlFile(filePath.c_str());
	if(xmlFile.is_open()){
		xml = std::string((std::istreambuf_iterator<char>(xmlFile)),std::istreambuf_iterator<char>());
	}

	return xml;
};
