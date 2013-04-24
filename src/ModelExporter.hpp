/****************************************************************************
File name: ModelExporter.hpp

Description: Class for exporting a 3D city model.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/

#ifndef MODEL_EXPORTER_HPP
#define MODEL_EXPORTER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Building.hpp"
#include "Path.hpp"

class ModelExporter{

	public:
		void addBuilding(const Building& building);

		void addPath(const Path& path);

		// Export to OBJ format. Note: function does not check that file path has .OBJ extension.
		void exportModel(const std::string& filePath);

		void clear(){buildings.clear(); paths.clear();};

	private:
		std::vector<Building> buildings;
		std::vector<Path> paths;

};

#endif
