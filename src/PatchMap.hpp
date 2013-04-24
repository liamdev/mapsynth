/****************************************************************************
File name: PatchMap.hpp

Description: Class containing the system's synthesised result.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/

#ifndef PATCH_MAP_HPP
#define PATCH_MAP_HPP

#include <unordered_map>
#include <vector>

#include "MapFragment.hpp"
#include "MapLUT.hpp"
#include "ModelExporter.hpp"
#include "RoadGraph.hpp"

class PatchMap{

	public:
		explicit PatchMap(const MapLUT& lut, unsigned int x, unsigned int y,
							float windowWidth, float windowHeight);

		// Call step enough times to fill the map, then translate patch data to its correct position.
		void fill();
		// Fill the map with the layout of an input city, rather than synthesising a new one.
		void fill(const City& city);

		// Ensures consistency of patch locations, edge points and vertices.
		void updatePositions();
		void updateEdges();
		void updateVertices();

		// Add a new tile, stepping in a left->right, top->bottom order.
		void step();
		
		void generateRoadGraph();

		const RoadGraph& getRoadGraph() const;

		// Fix steps.
		void smoothSeams();
		void removeFloatingRoads();

		// Export to various formats.
		void exportOBJ(const std::string& filePath) const;
		void exportSVG(const std::string& filePath, bool grid = false) const;
		void exportDetails(const std::string& filePath, const std::string& mapName) const;
		void exportGraphDetails(const std::string& filePath) const;

		void disableGraphGeneration(){genGraph = false;};
		void enableGraphGeneration(){genGraph = true;};

	private:
		const MapLUT& lut;
		
		std::vector<std::vector<Patch> > patches;

		RoadGraph graph;

		float patchWidth;
		float patchHeight;

		unsigned int xTiles;
		unsigned int yTiles;

		unsigned int xPos;
		unsigned int yPos;

		bool genGraph;
};

#endif