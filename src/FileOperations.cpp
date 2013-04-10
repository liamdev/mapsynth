#include "FileOperations.hpp"

namespace bfs = boost::filesystem;
typedef bfs::directory_iterator dirIter;

std::vector<std::string> lsSVGFiles(const std::string& dirPath){
	std::vector<std::string> filePaths;

	//Load all available SVG files.
	if(!bfs::exists(dirPath)){
		std::cerr << "Image directory not found: " << dirPath << ".\n";
		return filePaths;
	}

	try{
		//Find all images in resource directory.
		dirIter end;
		for(dirIter iter(dirPath); iter != end; ++iter){
			if(bfs::is_regular_file(*iter)){
				//Check for image extension.
				if(iter->path().extension().string() == ".svg")
					filePaths.push_back(iter->path().string());
			}
		}
	} catch(const bfs::filesystem_error& err){
		std::cerr << err.what() << "\n";
	}

	return filePaths;
};

std::vector<std::string> lsImageFiles(const std::string& dirPath){
	std::vector<std::string> filePaths;

	//Load all available image files.
	if(!bfs::exists(dirPath)){
		std::cerr << "Image directory not found: " << dirPath << ".\n";
		return filePaths;
	}

	try{
		//Find all images in resource directory.
		dirIter end;
		for(dirIter iter(dirPath); iter != end; ++iter){
			if(bfs::is_regular_file(*iter)){
				//Check for image extension.
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
}
