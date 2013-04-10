#include "RoadGraph.hpp"


/* --------------------- Road Graph functions --------------------- */

void RoadGraph::addRoad(Path& p){
	RoadSet set;
	
	const std::vector<float>& pathPoints = p.getPathPoints();

	for(size_t i = 0; i < pathPoints.size() - 1; i += 2){
		RoadNode node(pathPoints[i], pathPoints[i + 1]);
		bool found = false;
		
		for(size_t j = 0; j < roadSets.size(); ++j){
			if(roadSets[j].contains(node)){
				set = set.join(roadSets[j]);
				found = true;
				roadSets.erase(roadSets.begin() + j);
				--j;
			}
		}

		if(!found){
			set.addNode(node);
			if(node.first <= (xStart + JUNCTION_EQ_DISTANCE) || node.first >= (xLimit - JUNCTION_EQ_DISTANCE) ||
				node.second <= (yStart + JUNCTION_EQ_DISTANCE) || node.second >= (yLimit - JUNCTION_EQ_DISTANCE))
				set.setExternallyConnected(true);
		}
	}

	set.addPath(&p);

	roadSets.push_back(set);
};

void RoadGraph::clear(){
	roadSets.clear();
};

int RoadGraph::numFloatingRoads() const{
	int count = 0;

	for(size_t i = 0; i < roadSets.size(); ++i)
		if(!roadSets[i].isExternallyConnected())
			++count;

	return count;
};

void RoadGraph::printRoadSets() const{
	for(size_t i = 0; i < roadSets.size(); ++i){
		std::cout << "Road set " << (i + 1) << ":\n";

		NodeSet::const_iterator iter;
		for(iter = roadSets[i].begin(); iter != roadSets[i].end(); ++iter)
			std::cout << iter->first << ", " << iter->second << "\n";

		std::cout << "\n";
	}
};

void RoadGraph::exportRoadSets(const std::string& filePath) const{
	std::ofstream fOut(filePath.c_str());

	if(fOut.is_open()){
		for(size_t i = 0; i < roadSets.size(); ++i){
			fOut << "Road set " << (i + 1) << ":\n";
			fOut << "Floating: ";
			if(roadSets[i].isExternallyConnected())
				fOut << "no\n";
			else
				fOut << "yes\n";

			NodeSet::const_iterator iter;
			for(iter = roadSets[i].begin(); iter != roadSets[i].end(); ++iter)
				fOut << iter->first << ", " << iter->second << "\n";

			fOut << "\n";
		}
	}

	fOut.close();
};

void RoadGraph::removeFloatingPaths(){
	for(size_t i = 0; i < roadSets.size(); ++i){
		if(!roadSets[i].isExternallyConnected())
			roadSets[i].setPathExport(false);
	}
};

/* --------------------- Road Set functions --------------------- */

void RoadSet::addNode(const RoadNode& node){
	nodes.insert(node);
};

void RoadSet::addPath(Path* path){
	paths.push_back(path);
}

bool RoadSet::contains(const RoadNode& node){
	return nodes.count(node) > 0;
};

void RoadSet::setPathExport(bool pExport){
	for(size_t i = 0; i < paths.size(); ++i)
		paths[i]->setExportStatus(false);
};

RoadSet& RoadSet::join(const RoadSet& other){
	nodes.insert(other.begin(), other.end());

	for(size_t i = 0; i < other.getPaths().size(); ++i)
		paths.push_back(other.getPaths()[i]);

	if(other.isExternallyConnected())
		setExternallyConnected(true);

	return *this;
};
