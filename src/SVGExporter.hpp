/****************************************************************************
File name: SVGExporter.hpp

Description: Class for exporting a 2D SVG representation of a map.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/

#ifndef SVG_EXPORTER_HPP
#define SVG_EXPORTER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Building.hpp"
#include "Config.hpp"
#include "Path.hpp"

struct SVGGrid{

	// Square grid.
	SVGGrid(float tSize = 0, int x = 0, int y = 0, float lSize = 0.0f):
		tileWidth(tSize),
		tileHeight(tSize),
		xTiles(x),
		yTiles(y),
		lineSize(lSize)
	{};

	// Rectangular grid.
	SVGGrid(float tWidth, float tHeight, int x = 0, int y = 0, float lSize = 0.0f):
		tileWidth(tWidth),
		tileHeight(tHeight),
		xTiles(x),
		yTiles(y),
		lineSize(lSize)
	{};

	float tileWidth;
	float tileHeight;

	int xTiles;
	int yTiles;

	float lineSize;
};

class SVGExporter{

	public:
		SVGExporter():
			exportGrid(true)
		{};

		void addBuilding(const Building& building);

		void addPath(const Path& path);

		// Export to SVG format. Note: does not check for SVG file extension.
		void exportSVG(const std::string& filePath);

		void clear(){buildings.clear(); paths.clear();};

		// Modify grid overlay.
		void setGrid(SVGGrid g){grid = g;};
		void enableGrid() {exportGrid = true;};
		void disableGrid(){exportGrid = false;};

	private:
		std::vector<Building> buildings;
		std::vector<Path> paths;

		SVGGrid grid;

		bool exportGrid;

};

#endif
