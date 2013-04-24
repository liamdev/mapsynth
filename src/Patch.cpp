#include "Patch.hpp"

// Construct a patch from a given MapFragment.
Patch::Patch(const MapFragment& frag) : baseFragment(frag), modifications(0){
	
	// Get a solid copy of each building.
	for(size_t i = 0; i < frag.getBuildings().size(); ++i)
		buildingCopies.push_back(Building(*(frag.getBuildings()[i])));

	// Copy across map fragment data.
	pathCopies = frag.getPaths();

	topRoads = frag.getTopRoads();
	bottomRoads = frag.getBottomRoads();
	leftRoads = frag.getLeftRoads();
	rightRoads = frag.getRightRoads();

	xMin = frag.getXMin();
	xMax = frag.getXMax();
	yMin = frag.getYMin();
	yMax = frag.getYMax();
}

// Normalise road/building coordinates to a patch which starts at 0,0.
void Patch::positionAtZero(){
	
	for(size_t i = 0; i < pathCopies.size(); ++i){
		std::vector<float>& points = pathCopies[i].getPathPoints();
		for(size_t j = 0; j < points.size() - 1; j += 2){
			points[j] -= xMin;
			points[j + 1] -= yMin;
		}
	}

	for(size_t i = 0; i < buildingCopies.size(); ++i){
		std::vector<float>& points = buildingCopies[i].getPlotPoints();
		for(size_t j = 0; j < points.size() - 1; j += 2){
			points[j] -= xMin;
			points[j + 1] -= yMin;
		}
	}

	// Update cached min/max values.
	xMax = xMax - xMin;
	yMax = yMax - yMin;
	xMin = 0;
	yMin = 0;
};

// Translate patch contents by a given amount.
void Patch::offset(float xOffset, float yOffset){
	for(size_t i = 0; i < pathCopies.size(); ++i){
		std::vector<float>& points = pathCopies[i].getPathPoints();
		for(size_t j = 0; j < points.size() - 1; j += 2){
			points[j] += xOffset;
			points[j + 1] += yOffset;
		}
	}

	for(size_t i = 0; i < buildingCopies.size(); ++i){
		std::vector<float>& points = buildingCopies[i].getPlotPoints();
		for(size_t j = 0; j < points.size() - 1; j += 2){
			points[j] += xOffset;
			points[j + 1] += yOffset;
		}
	}

	xMin += xOffset;
	yMin += yOffset;
	xMax += xOffset;
	yMax += yOffset;
};

// Calculate the vertices for each of the patch's contents.
void Patch::updateVertices(){
	for(size_t i = 0; i < buildingCopies.size(); ++i)
		buildingCopies[i].recalculateVertices();

	for(size_t i = 0; i < pathCopies.size(); ++i)
		pathCopies[i].recalculateVertices();
};


//Essentially duplicated from MapFragment.
//TODO: Re-design the MapFragment/Patch relationship.
void Patch::cacheEdgePoints(){
	
	// Clear in the event of re-caching.
	topRoads.clear();
	bottomRoads.clear();
	rightRoads.clear();
	leftRoads.clear();

	// Detect each path node which lies on the boundary, and create a RoadEnd object for it.
	for(size_t i = 0; i < pathCopies.size(); ++i){
		std::vector<float>& points = pathCopies[i].getPathPoints();
		
		if(fEq(points[0], xMin))
			leftRoads.push_back(RoadEnd(points[1] - yMin, pathCopies[i].getSize(),
								getDirectionRad(points[2], points[3], points[0], points[1]),
								&points[0]));
		else if(fEq(points[0], xMax))
			rightRoads.push_back(RoadEnd(points[1] - yMin, pathCopies[i].getSize(),
								getDirectionRad(points[2], points[3], points[0], points[1]),
								&points[0]));
		else if(fEq(points[1], yMin))
			topRoads.push_back(RoadEnd(points[0] - xMin, pathCopies[i].getSize(),
								getDirectionRad(points[2], points[3], points[0], points[1]),
								&points[0]));
		else if(fEq(points[1], yMax))
			bottomRoads.push_back(RoadEnd(points[0] - xMin, pathCopies[i].getSize(),
								getDirectionRad(points[2], points[3], points[0], points[1]),
								&points[0]));
		
		size_t endP = points.size() - 2;
		if(fEq(points[endP], xMin))
			leftRoads.push_back(RoadEnd(points[endP + 1] - yMin, pathCopies[i].getSize(),
							getDirectionRad(points[endP - 2], points[endP - 1], points[endP], points[endP + 1]),
							&points[endP]));
		else if(fEq(points[endP], xMax))
			rightRoads.push_back(RoadEnd(points[endP + 1] - yMin, pathCopies[i].getSize(),
							getDirectionRad(points[endP - 2], points[endP - 1], points[endP], points[endP + 1]),
							&points[endP]));
		else if(fEq(points[endP + 1], yMin))
			topRoads.push_back(RoadEnd(points[endP] - xMin, pathCopies[i].getSize(),
							getDirectionRad(points[endP - 2], points[endP - 1], points[endP], points[endP + 1]),
							&points[endP]));
		else if(fEq(points[endP + 1], yMax))
			bottomRoads.push_back(RoadEnd(points[endP] - xMin, pathCopies[i].getSize(),
							getDirectionRad(points[endP - 2], points[endP - 1], points[endP], points[endP + 1]),
							&points[endP]));
	}

	// Sort each edge's road ends.
	std::sort(topRoads.begin(), topRoads.end(), &RoadEnd::compare);
	std::sort(bottomRoads.begin(), bottomRoads.end(), &RoadEnd::compare);
	std::sort(leftRoads.begin(), leftRoads.end(), &RoadEnd::compare);
	std::sort(rightRoads.begin(), rightRoads.end(), &RoadEnd::compare);
};


float Patch::getBuildingDensity() const{
	float sizeTotal = 0;

	//Get a rough estimate of building sizes (non-aligned bounding box).
	for(size_t i = 0; i < buildingCopies.size(); ++i)
		sizeTotal += buildingCopies[i].getArea();

	float area = (xMax - xMin) * (yMax - yMin);

	if(area == 0)
		return 0;

	return sizeTotal / area;
};

// Path density given as the path's length.
// TODO: Incorporate path's width to get a more accurate density.
float Patch::getPathDensity() const{
	float sizeTotal = 0;

	for(size_t i = 0; i < pathCopies.size(); ++i)
		sizeTotal += pathCopies[i].getLength();

	float area = (xMax - xMin) * (yMax - yMin);
	
	if(area == 0)
		return 0;

	return sizeTotal / area;
};
