/****************************************************************************
File name: MapFragment.hpp

Description: Class containing road/building data for a given window of a city.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/

#ifndef MAP_FRAGMENT_HPP
#define MAP_FRAGMENT_HPP

#include <algorithm>
#include <vector>

#include "MathTools.hpp"

#include "Building.hpp"
#include "Config.hpp"
#include "Path.hpp"
#include "RoadGraph.hpp"

// Road point which lies on the fragment's boundary.
struct RoadEnd{

	RoadEnd(float loc, float size, float angle, float* coordPos) :
		location(loc),
		size(size),
		angle(angle),
		coordPos(coordPos)
	{};

	// Location is distance relative to the edge's top (vertical edge) or left (horizontal edge) point.
	float location;
	float size;
	float angle;

	// Pointer to the original path point, to allow modification of the road end's actual location,
	// rather than cached location.
	float* coordPos;

	static bool compare(const RoadEnd& lhs, const RoadEnd& rhs){
		return lhs.location < rhs.location;
	};
};


class MapFragment{

	public:
		MapFragment(float xMin = 0, float yMin = 0, float xMax = 0, float yMax = 0) :
			empty(true),
			xMin(xMin),
			yMin(yMin),
			xMax(xMax),
			yMax(yMax){};

		// Flag for determining the edge to use when comparing fragments.
		enum CompareDirection {RIGHT, BELOW};

		bool isEmpty() const {return empty;};

		// Add elements.
		void addPath(const Path& path);
		void addBuilding(const Building* building);

		// Compare compatibility of two fragments, with a given relative positioning.
		float compareTo(const MapFragment& other, CompareDirection dir) const;

		// Element counts.
		size_t pathCount()     const {return clippedPaths.size();};
		size_t buildingCount() const {return buildings.size();};

		size_t topRoadCount()    const {return topRoads.size();};
		size_t bottomRoadCount() const {return bottomRoads.size();};
		size_t leftRoadCount()   const {return leftRoads.size();};
		size_t rightRoadCount()  const {return rightRoads.size();};

		// Element access.
		std::vector<RoadEnd>& getTopRoads()    {return topRoads;};
		std::vector<RoadEnd>& getBottomRoads() {return bottomRoads;};
		std::vector<RoadEnd>& getLeftRoads()   {return leftRoads;};
		std::vector<RoadEnd>& getRightRoads()  {return rightRoads;};

		std::vector<const Building*>& getBuildings() {return buildings;};
		std::vector<Path>&      getPaths()     {return clippedPaths;};

		// Const element access.
		const std::vector<RoadEnd>& getTopRoads()    const {return topRoads;};
		const std::vector<RoadEnd>& getBottomRoads() const {return bottomRoads;};
		const std::vector<RoadEnd>& getLeftRoads()   const {return leftRoads;};
		const std::vector<RoadEnd>& getRightRoads()  const {return rightRoads;};

		const std::vector<const Building*>& getBuildings() const {return buildings;};
		const std::vector<Path>&     getPaths()     const {return clippedPaths;};

		// Position access.
		float getXMin() const {return xMin;};
		float getXMax() const {return xMax;};
		float getYMin() const {return yMin;};
		float getYMax() const {return yMax;};

		// Finds the boundary points within the list of paths, and caches them to avoid repeated lookup.
		void cacheEdgePoints();

	private:
		std::vector<const Building*> buildings;
		std::vector<Path> clippedPaths;

		// Cached end points.
		std::vector<RoadEnd> topRoads;
		std::vector<RoadEnd> bottomRoads;
		std::vector<RoadEnd> leftRoads;
		std::vector<RoadEnd> rightRoads;

		bool empty;

		float xMin, yMin;
		float xMax, yMax;
};

#endif
