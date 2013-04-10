#include "MapFragment.hpp"

void MapFragment::addPath(const Path& path){
	empty = false;

	clippedPaths.push_back(path);
};


void MapFragment::addBuilding(const Building* building){
	empty = false;
	buildings.push_back(building);
};


float getWeightingBetween(const std::vector<RoadEnd>& set1, const std::vector<RoadEnd>& set2){
	float diff = 0;

	/*  TODO: Implement a method for handling incompatible fragments.
	 *  Use a 'velcro' algorithm, where the endpoints lock-on and compare with the nearest
	 *  endpoint on the other fragment.
	 */
	if(set1.size() != set2.size())
		return 10000;

	for(size_t i = 0; i < set1.size(); ++i){

		float dist = fabs(set1[i].location - set2[i].location);

		float sizeRatio = set1[i].size / set2[i].size;
		sizeRatio = (sizeRatio < 1) ? 1 / sizeRatio : sizeRatio;

		float sizeModifier = set1[i].size * set2[i].size;

		diff += (dist * dist * sizeRatio * sizeRatio) / sizeModifier;
	}
	
	return diff;
};


float MapFragment::compareTo(const MapFragment& other, CompareDirection dir) const{
	//Direction is the position of 'this' in comparison to 'other'.

	float diff = 0;

	//TODO: Experiment with size ratio weighting.
	switch(dir){
		case RIGHT:
			diff = getWeightingBetween(this->getLeftRoads(), other.getRightRoads());
			break;
		case BELOW:
			diff = getWeightingBetween(this->getTopRoads(), other.getBottomRoads());
			break;
		default:
			break;
	};

	return diff;
};

void MapFragment::cacheEdgePoints(){
	topRoads.clear();
	bottomRoads.clear();
	rightRoads.clear();
	leftRoads.clear();
	for(size_t i = 0; i < clippedPaths.size(); ++i){
		std::vector<float>& points = clippedPaths[i].getPathPoints();
		
		if(fEq(points[0], xMin))
			leftRoads.push_back(RoadEnd(points[1] - yMin, clippedPaths[i].getSize(),
								getDirectionRad(points[2], points[3], points[0], points[1]),
								&points[0]));
		else if(fEq(points[0], xMax))
			rightRoads.push_back(RoadEnd(points[1] - yMin, clippedPaths[i].getSize(),
								getDirectionRad(points[2], points[3], points[0], points[1]),
								&points[0]));
		else if(fEq(points[1], yMin))
			topRoads.push_back(RoadEnd(points[0] - xMin, clippedPaths[i].getSize(),
								getDirectionRad(points[2], points[3], points[0], points[1]),
								&points[0]));
		else if(fEq(points[1], yMax))
			bottomRoads.push_back(RoadEnd(points[0] - xMin, clippedPaths[i].getSize(),
								getDirectionRad(points[2], points[3], points[0], points[1]),
								&points[0]));
		
		size_t endP = points.size() - 2;
		if(fEq(points[endP], xMin))
			leftRoads.push_back(RoadEnd(points[endP + 1] - yMin, clippedPaths[i].getSize(),
							getDirectionRad(points[endP - 2], points[endP - 1], points[endP], points[endP + 1]),
							&points[endP]));
		else if(fEq(points[endP], xMax))
			rightRoads.push_back(RoadEnd(points[endP + 1] - yMin, clippedPaths[i].getSize(),
							getDirectionRad(points[endP - 2], points[endP - 1], points[endP], points[endP + 1]),
							&points[endP]));
		else if(fEq(points[endP + 1], yMin))
			topRoads.push_back(RoadEnd(points[endP] - xMin, clippedPaths[i].getSize(),
							getDirectionRad(points[endP - 2], points[endP - 1], points[endP], points[endP + 1]),
							&points[endP]));
		else if(fEq(points[endP + 1], yMax))
			bottomRoads.push_back(RoadEnd(points[endP] - xMin, clippedPaths[i].getSize(),
							getDirectionRad(points[endP - 2], points[endP - 1], points[endP], points[endP + 1]),
							&points[endP]));
	}

	std::sort(topRoads.begin(), topRoads.end(), &RoadEnd::compare);
	std::sort(bottomRoads.begin(), bottomRoads.end(), &RoadEnd::compare);
	std::sort(leftRoads.begin(), leftRoads.end(), &RoadEnd::compare);
	std::sort(rightRoads.begin(), rightRoads.end(), &RoadEnd::compare);
};
