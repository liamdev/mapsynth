/****************************************************************************
File name: FileTools.hpp

Description: Provides functions for file lookup through Boost's Filesystem library.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/


#ifndef FILE_TOOLS_HPP
#define FILE_TOOLS_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

//Search for files within a directory, with various extension types.
std::vector<std::string> lsSVGFiles(const std::string& dirPath);
std::vector<std::string> lsImageFiles(const std::string& dirPath);

std::string readFile(const std::string& filePath);

#endif