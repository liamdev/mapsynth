#include "MapLUT.hpp"

// Convert city to extracted fragments.
void MapLUT::addMap(const City& map, float windowSize, float offset){

	// Maximum top-left corner for fragment extraction.
	float widthTrim = (float)map.getWidth() - windowSize;
	float heightTrim = (float)map.getHeight() - windowSize;

	// Calculate number of extractable tiles.
	int xTiles = (int)(widthTrim / (int)offset + 1);
	int yTiles = (int)(heightTrim / (int)offset + 1);

	if(widthTrim < 0 || heightTrim < 0 || xTiles < 1 || yTiles < 1){
		std::cerr << "Input map too small to extract tiles.\n";
		return;
	}

	// Add tile fragments to the lookup table.
	int numPieces = pieces.size();
	for(int x = 0; x < xTiles; ++x){
		for(int y = 0; y < yTiles; ++y){
			// Set up LUT storage space first, use that reference, avoids passing around large data structures.
			pieces.push_back(MapFragment());
			map.fillFragment(pieces[numPieces],
							(float)(x * offset), (float)(y * offset),
							(float)windowSize, (float)windowSize);
			// Don't store empty fragments.
			// TODO: Consider allowing empty fragments, otherwise sparse maps get more crowded than they should be.
			if(pieces[numPieces].pathCount() == 0){
				pieces.pop_back();
				continue;
			} else {
				++numPieces;
			}
		}
	}
};


const Patch MapLUT::getRandomPatch() const{
	if(pieces.empty())
		return Patch(MapFragment());

	// Only return a random patch which has at least one path.
	while(true){
		int index = (int)(((float)rand() / RAND_MAX) * pieces.size());
		if(pieces[index].pathCount() != 0)
			return Patch(pieces[index]);
	}
};

// Get a new patch to go on the right of the given patch.
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
	
	// Choose one of the patches which had the lowest cost.
	int selection = (int)(((float)rand() / RAND_MAX) * choices.size());
	return Patch(pieces[choices[selection]]);
};

// Get a new patch to go below the given patch.
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
	
	// Choose one of the patches which had the lowest cost.
	int selection = (int)(((float)rand() / RAND_MAX) * choices.size());
	return Patch(pieces[choices[selection]]);
};

// Get a new patch to go to the right of the first given patch, and below the second.
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
	
	// Choose one of the patches which had the lowest cost.
	int selection = (int)(((float)rand() / RAND_MAX) * choices.size());
	return Patch(pieces[choices[selection]]);
};
