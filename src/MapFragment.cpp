#include "MapFragment.hpp"

void MapFragment::addPath(const Path& path){
	empty = false;

	clippedPaths.push_back(path);
};


void MapFragment::addBuilding(const Building* building){
	empty = false;
	buildings.push_back(building);
};

// The algorithm's cost function. Calculate the weight between two fragments (dictated by the end points on the seam).
float getWeightingBetween(const std::vector<RoadEnd>& set1, const std::vector<RoadEnd>& set2){
	float diff = 0;

	/*  TODO: Implement a method for handling fragments with incompatible road counts?.
	 *  If so, use a 'velcro' algorithm, where the endpoints lock-on/compare with the nearest
	 *  endpoint on the other fragment.
	 *	For now, return some arbitrary large number.
	 */
	if(set1.size() != set2.size())
		return 10000;


	for(size_t i = 0; i < set1.size(); ++i){

		// Distance factor.
		float dist = fabs(set1[i].location - set2[i].location);

		// Size ratio factor.
		float sizeRatio = set1[i].size / set2[i].size;
		sizeRatio = (sizeRatio < 1) ? 1 / sizeRatio : sizeRatio;

		// Road hierarchy factor.
		float sizeModifier = set1[i].size * set2[i].size;

		diff += (dist * dist * sizeRatio * sizeRatio) * sizeModifier;
	}
	
	return diff;
};


float MapFragment::compareTo(const MapFragment& other, CompareDirection dir) const{
	// Direction is the position of 'this' in comparison to 'other'.
	// e.g. (dir == RIGHT) implies that 'other' is on the left of this fragment, so we need its right endpoints.

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

// Store the points which lie along the fragment's boundaries.
void MapFragment::cacheEdgePoints(){
	// In case of re-caching.
	topRoads.clear();
	bottomRoads.clear();
	rightRoads.clear();
	leftRoads.clear();

	// For each path, go through the nodes and check if they lie on the boundary.
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

	// Sort by distance, to avoid crossing endpoints.
	std::sort(topRoads.begin(), topRoads.end(), &RoadEnd::compare);
	std::sort(bottomRoads.begin(), bottomRoads.end(), &RoadEnd::compare);
	std::sort(leftRoads.begin(), leftRoads.end(), &RoadEnd::compare);
	std::sort(rightRoads.begin(), rightRoads.end(), &RoadEnd::compare);
};
