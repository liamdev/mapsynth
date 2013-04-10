#ifndef FILE_OPERATIONS_HPP
#define FILE_OPERATIONS_HPP

#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

//Search for files within a directory, with various extensions.
std::vector<std::string> lsSVGFiles(const std::string& dirPath);
std::vector<std::string> lsImageFiles(const std::string& dirPath);

#endif
