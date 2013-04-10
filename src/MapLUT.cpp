#include "MapLUT.hpp"

void MapLUT::addMap(const City& map, float windowSize, float offset){
	float widthTrim = map.getWidth() - windowSize;
	float heightTrim = map.getHeight() - windowSize;

	//Calculate number of extractable tiles.
	int xTiles = (int)std::floor(widthTrim / (int)offset + 1);
	int yTiles = (int)std::floor(heightTrim / (int)offset + 1);
	
	if(widthTrim < 0 || heightTrim < 0 || xTiles < 1 || yTiles < 1){
		std::cerr << "Input map too small to extract tiles.\n";
		return;
	}

	//Add tile fragments to the lookup table.
	//Fragments handle calculation of their edge properties upon construction.
	int numPieces = pieces.size();
	for(int x = 0; x < xTiles; ++x){
		for(int y = 0; y < yTiles; ++y){
			pieces.push_back(MapFragment());
			map.fillFragment(pieces[numPieces],
							(float)(x * offset), (float)(y * offset),
							(float)windowSize, (float)windowSize);
			if(pieces[numPieces].pathCount() == 0){
				pieces.pop_back();
				continue;
			} else {
				++numPieces;
			}
		}
	}

	buildingDensities.push_back(map.getBuildingDensity());
};

const Patch MapLUT::getRandomPatch() const{
	if(pieces.empty())
		return Patch(MapFragment());

	while(true){
		int index = (int)(((float)rand() / RAND_MAX) * pieces.size());
		if(pieces[index].pathCount() != 0)
			return Patch(pieces[index]);
	}
};


const Patch MapLUT::getRightPatch(const Patch& left) const{
	float minDiff = FLT_MAX;
	std::vector<int> choices;

	for(size_t i = 0; i < pieces.size(); ++i){
		if(left.getBaseFragment().rightRoadCount() == pieces[i].leftRoadCount()){
			float comp = pieces[i].compareTo(left.getBaseFragment(), MapFragment::RIGHT);
			if(comp < minDiff){
				choices.clear();
				choices.push_back(i);
				minDiff = comp;
			} else if(comp == minDiff){
				choices.push_back(i);
			}
		}
	}

	if(choices.size() == 0)
		return getRandomPatch();
	
	int selection = (int)(((float)rand() / RAND_MAX) * choices.size());
	return Patch(pieces[choices[selection]]);
};

const Patch MapLUT::getBelowPatch(const Patch& above) const{
	float minDiff = FLT_MAX;
	std::vector<int> choices;

	for(size_t i = 0; i < pieces.size(); ++i){
		if(above.getBaseFragment().bottomRoadCount() == pieces[i].topRoadCount()){
			float comp = pieces[i].compareTo(above.getBaseFragment(), MapFragment::BELOW);
			if(comp < minDiff){
				choices.clear();
				choices.push_back(i);
				minDiff = comp;
			} else if(comp == minDiff){
				choices.push_back(i);
			}
		}
	}

	if(choices.size() == 0)
		return getRandomPatch();
	
	int selection = (int)(((float)rand() / RAND_MAX) * choices.size());
	return Patch(pieces[choices[selection]]);
};

const Patch MapLUT::getDownRightPatch(const Patch& left, const Patch& above) const{
	float minDiff = FLT_MAX;
	std::vector<int> choices;

	for(size_t i = 0; i < pieces.size(); ++i){
		if(above.getBaseFragment().bottomRoadCount() == pieces[i].topRoadCount() &&
			left.getBaseFragment().rightRoadCount() == pieces[i].leftRoadCount()){
			float comp = pieces[i].compareTo(above.getBaseFragment(), MapFragment::BELOW) +
							pieces[i].compareTo(left.getBaseFragment(), MapFragment::RIGHT);
			if(comp < minDiff){
				choices.clear();
				choices.push_back(i);
				minDiff = comp;
			} else if(comp == minDiff){
				choices.push_back(i);
			}
		}
	}

	if(choices.size() == 0)
		return getRandomPatch();
	
	int selection = (int)(((float)rand() / RAND_MAX) * choices.size());
	return Patch(pieces[choices[selection]]);
};

float MapLUT::getAverageBuildingDensity() const{
	float total = 0;
	size_t n = buildingDensities.size();

	if(n == 0)
		return 0;

	for(size_t i = 0; i < n; ++i)
		total += buildingDensities[i];

	return total / n;
};