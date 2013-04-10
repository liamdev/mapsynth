#include "Patch.hpp"

#include <iostream>
Patch::Patch(const MapFragment& frag) : baseFragment(frag), modifications(0){
	for(size_t i = 0; i < frag.getBuildings().size(); ++i)
		buildingCopies.push_back(Building(*(frag.getBuildings()[i])));


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

	xMax = xMax - xMin;
	yMax = yMax - yMin;
	xMin = 0;
	yMin = 0;
};

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

void Patch::updateVertices(){
	for(size_t i = 0; i < buildingCopies.size(); ++i)
		buildingCopies[i].recalculateVertices();

	for(size_t i = 0; i < pathCopies.size(); ++i)
		pathCopies[i].recalculateVertices();
};


//Essentially duplicated from MapFragment.
//TODO: Re-design the MapFragment/Patch relationship.
void Patch::cacheEdgePoints(){
	topRoads.clear();
	bottomRoads.clear();
	rightRoads.clear();
	leftRoads.clear();
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

	std::sort(topRoads.begin(), topRoads.end(), &RoadEnd::compare);
	std::sort(bottomRoads.begin(), bottomRoads.end(), &RoadEnd::compare);
	std::sort(leftRoads.begin(), leftRoads.end(), &RoadEnd::compare);
	std::sort(rightRoads.begin(), rightRoads.end(), &RoadEnd::compare);
};
