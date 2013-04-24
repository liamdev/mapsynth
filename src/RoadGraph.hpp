/****************************************************************************
File name: RoadGraph.hpp

Description: Class which calculates connectivity of input roads.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/

#ifndef ROAD_GRAPH_HPP
#define ROAD_GRAPH_HPP

#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "Config.hpp"
#include "MathTools.hpp"
#include "Path.hpp"


// Defined as the x,y coordinates of a node (i.e. one path point in SVG format).
typedef std::pair<float, float> RoadNode;

// Structure for overloading equality of two RoadNodes.
struct NodeComp{
	bool operator()(const RoadNode& lhs, const RoadNode& rhs) const {
		float xDist = (lhs.first - rhs.first) * (lhs.first - rhs.first);
		float yDist = (lhs.second - rhs.second) * (lhs.second - rhs.second);

		if((xDist + yDist) < (JUNCTION_EQ_DISTANCE * JUNCTION_EQ_DISTANCE))
			return false;

		return lhs < rhs;
	};
};

typedef std::set<RoadNode, NodeComp> NodeSet;

// Stores a list of connected road nodes.
class RoadSet{
	
	public:
		RoadSet(bool eConn = false) :
			externalConnection(eConn)
		{};

		// Merge with another set.
		RoadSet& join(const RoadSet& other);

		void addNode(const RoadNode& node);
		void addPath(Path* path);

		void setExternallyConnected(bool conn){externalConnection = conn;};

		// Enable/disable export of each path in the set.
		void setPathExport(bool pExport);

		size_t nodeCount() const {return nodes.size();};
		
		bool contains(const RoadNode& node);

		// Whether the contained paths have a link to the map's edge.
		bool isExternallyConnected() const {return externalConnection;};

		NodeSet::iterator begin(){return nodes.begin();};
		NodeSet::iterator end(){return nodes.end();};

		NodeSet::const_iterator begin() const {return nodes.begin();};
		NodeSet::const_iterator end() const {return nodes.end();};

		const std::vector<Path*>& getPaths() const {return paths;};

	private:
		NodeSet nodes;
		std::vector<Path*> paths;

		bool externalConnection;
};

class RoadGraph{

	public:
		explicit RoadGraph(float xLimit, float yLimit) :
			xStart(0),
			yStart(0),
			xLimit(xLimit),
			yLimit(yLimit)
		{};

		explicit RoadGraph(float xStart, float yStart, float xLimit, float yLimit) :
			xStart(xStart),
			yStart(yStart),
			xLimit(xLimit),
			yLimit(yLimit)
		{};

		void addRoad(Path& path);

		void clear();

		size_t roadSetCount() const {return roadSets.size();};

		int numFloatingRoads() const;

		float getXLimit() const {return xLimit;};
		float getYLimit() const {return yLimit;};

		// Debugging purposes.
		void printRoadSets() const;
		void exportRoadSets(const std::string& filePath) const;

		void removeFloatingPaths();

	private:
		std::vector<RoadSet> roadSets;

		//Map top-left.
		float xStart;
		float yStart;
		//Map top-right.
		float xLimit;
		float yLimit;
};

#endif
