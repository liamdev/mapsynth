#include "FileTools.hpp"

// Simplify the horrifically long namespace prefix.
namespace bfs = boost::filesystem;
typedef bfs::directory_iterator dirIter;


// Lists all SVG files found within the specified directory.
std::vector<std::string> lsSVGFiles(const std::string& dirPath){
	std::vector<std::string> filePaths;

	// Check directory existence.
	if(!bfs::exists(dirPath)){
		std::cerr << "Image directory not found: " << dirPath << ".\n";
		return filePaths;
	}

	try{
		//Find all images in resource directory.
		dirIter end;
		for(dirIter iter(dirPath); iter != end; ++iter){
			if(bfs::is_regular_file(*iter)){
				// Check for SVG extension.
				if(iter->path().extension().string() == ".svg")
					filePaths.push_back(iter->path().string());
			}
		}
	} catch(const bfs::filesystem_error& err){
		std::cerr << err.what() << "\n";
	}

	return filePaths;
};

// Lists all image files within the specified directory.
std::vector<std::string> lsImageFiles(const std::string& dirPath){
	std::vector<std::string> filePaths;

	// Check directory existence.
	if(!bfs::exists(dirPath)){
		std::cerr << "Image directory not found: " << dirPath << ".\n";
		return filePaths;
	}

	try{
		// Find all images in resource directory.
		dirIter end;
		for(dirIter iter(dirPath); iter != end; ++iter){
			if(bfs::is_regular_file(*iter)){
				// Check for image extension.
				std::string ext = iter->path().extension().string();
				if(ext == ".jpg" || ext == ".png" || ext == ".gif"){
					filePaths.push_back(iter->path().string());
				}
			}
		}
	} catch(const bfs::filesystem_error& err){
		std::cerr << err.what() << "\n";
	}

	return filePaths;
};

std::string readFile(const std::string& filePath){
	std::string xml = "";

	std::ifstream xmlFile(filePath.c_str());
	if(xmlFile.is_open()){
		xml = std::string((std::istreambuf_iterator<char>(xmlFile)),std::istreambuf_iterator<char>());
	}

	return xml;
};