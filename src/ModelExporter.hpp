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

		void exportModel(const std::string& filePath);

		void clear(){buildings.clear(); paths.clear();};

	private:
		std::vector<Building> buildings;
		std::vector<Path> paths;

};

#endif
