#ifndef MAP_FRAGMENT_HPP
#define MAP_FRAGMENT_HPP

#include <algorithm>
#include <vector>

#include "MathTools.hpp"

#include "Building.hpp"
#include "Config.hpp"
#include "Path.hpp"
#include "RoadGraph.hpp"

struct RoadEnd{

	RoadEnd(float loc, float size, float angle, float* coordPos) :
		location(loc),
		size(size),
		angle(angle),
		coordPos(coordPos)
	{};

	float location;
	float size;
	float angle;

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

		enum CompareDirection {RIGHT, BELOW};

		bool isEmpty() const {return empty;};

		//Add elements.
		void addPath(const Path& path);
		void addBuilding(const Building* building);

		//Compare compatibility of two fragments, with a given relative positioning.
		float compareTo(const MapFragment& other, CompareDirection dir) const;

		//Element counts
		size_t pathCount()     const {return clippedPaths.size();};
		size_t buildingCount() const {return buildings.size();};

		std::vector<RoadEnd>& getTopRoads()    {return topRoads;};
		std::vector<RoadEnd>& getBottomRoads() {return bottomRoads;};
		std::vector<RoadEnd>& getLeftRoads()   {return leftRoads;};
		std::vector<RoadEnd>& getRightRoads()  {return rightRoads;};

		std::vector<const Building*>& getBuildings() {return buildings;};
		std::vector<Path>&      getPaths()     {return clippedPaths;};

		//Const element access
		const std::vector<RoadEnd>& getTopRoads()    const {return topRoads;};
		const std::vector<RoadEnd>& getBottomRoads() const {return bottomRoads;};
		const std::vector<RoadEnd>& getLeftRoads()   const {return leftRoads;};
		const std::vector<RoadEnd>& getRightRoads()  const {return rightRoads;};

		const std::vector<const Building*>& getBuildings() const {return buildings;};
		const std::vector<Path>&     getPaths()     const {return clippedPaths;};

		size_t topRoadCount()    const {return topRoads.size();};
		size_t bottomRoadCount() const {return bottomRoads.size();};
		size_t leftRoadCount()   const {return leftRoads.size();};
		size_t rightRoadCount()  const {return rightRoads.size();};

		float getXMin() const {return xMin;};
		float getXMax() const {return xMax;};
		float getYMin() const {return yMin;};
		float getYMax() const {return yMax;};

		void cacheEdgePoints();

	private:
		std::vector<const Building*> buildings;
		std::vector<Path> clippedPaths;

		std::vector<RoadEnd> topRoads;
		std::vector<RoadEnd> bottomRoads;
		std::vector<RoadEnd> leftRoads;
		std::vector<RoadEnd> rightRoads;

		bool empty;

		float xMin, yMin;
		float xMax, yMax;
};

#endif
