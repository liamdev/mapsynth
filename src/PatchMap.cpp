#include "PatchMap.hpp"

//Create a PatchMap which synthesises a new map.
PatchMap::PatchMap(const MapLUT& lut, unsigned int x, unsigned int y, float windowWidth, float windowHeight):
	lut(lut),
	graph(RoadGraph((float)(xTiles * windowWidth), (float)(yTiles * windowHeight))),
	patchWidth(windowWidth),
	patchHeight(windowHeight),
	xTiles(x),
	yTiles(y),
	xPos(0),
	yPos(0)
{
	//Set up patch array.
	patches = std::vector<std::vector<Patch> >(x, std::vector<Patch>(y, Patch(MapFragment())));
};


/* ------------------- Generation functions ------------------- */

void PatchMap::fill(){
	if(lut.isEmpty())
		std::cout << "Warning: no fragments available. Ensure maps are in the correct input location.\n";

	for(unsigned int i = 0; i < xTiles * yTiles; ++i)
		step();
	
	updatePositions();
	updateEdges();
};

void PatchMap::fill(const City& city){
	patchWidth = (float)city.getWidth() / xTiles;
	patchHeight = (float)city.getHeight() / yTiles;

	for(size_t x = 0; x < xTiles; ++x){
		for(size_t y = 0; y < yTiles; ++y){
			MapFragment frag;
			city.fillFragment(frag, x * patchWidth, y * patchHeight, patchWidth, patchHeight);
			patches[x][y] = Patch(frag);
		}
	}

	updatePositions();
	updateEdges();
};

void PatchMap::step(){
	if(yPos == yTiles)
		return;

	if(xPos == 0 && yPos == 0){
		patches[xPos][yPos] = lut.getRandomPatch();
		++xPos;
	} else {
		if(yPos > 0){
			if(xPos == 0){
				patches[xPos][yPos] = lut.getBelowPatch(patches[xPos][yPos - 1]);
			} else {
				Patch left = patches[xPos - 1][yPos];
				Patch up = patches[xPos][yPos - 1];
				patches[xPos][yPos] = lut.getDownRightPatch(left, up);
			}
		} else {
			patches[xPos][yPos] = lut.getRightPatch(patches[xPos - 1][yPos]);
		}

		++xPos;

		if(xPos == xTiles){
			++yPos;
			xPos = 0;
		} 
	}
};


void PatchMap::updateEdges(){
	for(size_t x = 0; x < xTiles; ++x){
		for(size_t y = 0; y < yTiles; ++y){
			patches[x][y].cacheEdgePoints();
		}
	}
};

void PatchMap::updatePositions(){
	for(size_t x = 0; x < xTiles; ++x){
		for(size_t y = 0; y < yTiles; ++y){
			patches[x][y].positionAtZero();
			patches[x][y].offset((float)(x * patchWidth), (float)(y * patchHeight));	
		}
	}
};

void PatchMap::updateVertices(){
	for(size_t x = 0; x < xTiles; ++x){
		for(size_t y = 0; y < yTiles; ++y){
			patches[x][y].updateVertices();
		}
	}
};


/* ------------------- Graph setup ------------------- */

const RoadGraph& PatchMap::getRoadGraph() const{
	return graph;
};

void PatchMap::generateRoadGraph(){
	graph.clear();

	for(size_t x = 0; x < xTiles; ++x){
		for(size_t y = 0; y < yTiles; ++y){
			std::vector<Path>& paths = patches[x][y].getPaths();

			for(size_t i = 0; i < paths.size(); ++i)
				graph.addRoad(paths[i]);
		}
	}
}


/* ------------------- 'Fixer' functions ------------------- */

void PatchMap::smoothSeams(){
	for(size_t x = 0; x < xTiles; ++x){
		for(size_t y = 0; y < yTiles; ++y){
			if(x < xTiles - 1){
				std::vector<RoadEnd>& rightEnds = patches[x][y].getRightRoads();
				std::vector<RoadEnd>& leftEnds = patches[x + 1][y].getLeftRoads();

				if(rightEnds.size() != leftEnds.size())
					continue;

				for(size_t i = 0; i < rightEnds.size(); ++i){
					float xDistance = *(rightEnds[i].coordPos) - *(leftEnds[i].coordPos);
					float yDistance = rightEnds[i].location - leftEnds[i].location;
					*(rightEnds[i].coordPos) -= xDistance / 2;
					*(leftEnds[i].coordPos) += xDistance / 2;
					*(rightEnds[i].coordPos + 1) -= yDistance / 2;
					*(leftEnds[i].coordPos + 1) += yDistance / 2;
					float modification = fabs(xDistance) + fabs(yDistance);
					patches[x][y].addModification(modification);
					patches[x + 1][y].addModification(modification);
				}
			}

			if(y == 0)
				continue;

			std::vector<RoadEnd>& topEnds = patches[x][y].getTopRoads();
			std::vector<RoadEnd>& bottomEnds = patches[x][y - 1].getBottomRoads();

			if(topEnds.size() != bottomEnds.size())
				continue;

			for(size_t i = 0; i < topEnds.size(); ++i){
				float xDistance = topEnds[i].location - bottomEnds[i].location;
				float yDistance = *(topEnds[i].coordPos + 1) - *(bottomEnds[i].coordPos + 1);
				*(topEnds[i].coordPos) -= xDistance / 2;
				*(bottomEnds[i].coordPos) += xDistance / 2;
				*(topEnds[i].coordPos + 1) -= yDistance / 2;
				*(bottomEnds[i].coordPos + 1) += yDistance / 2;
				float modification = fabs(xDistance) + fabs(yDistance);
				patches[x][y].addModification(modification);
				patches[x][y - 1].addModification(modification);
			}
		}
	}

	updateEdges();
};

void PatchMap::removeFloatingRoads(){
	if(graph.roadSetCount() == 0)
		generateRoadGraph();

	graph.removeFloatingPaths();

	std::unordered_map<Path*, std::vector<Building*> > removeBuildings;
	for(size_t x = 0; x < xTiles; ++x){
		for(size_t y = 0; y < yTiles; ++y){
			std::vector<Building>& buildings = patches[x][y].getBuildings();

			for(size_t b = 0; b < buildings.size(); ++b){
				float centerX = 0, centerY = 0;

				for(size_t bp = 0; bp < buildings[b].getPlotPoints().size() - 1; bp += 2){
					centerX += buildings[b].getPlotPoints()[bp];
					centerY += buildings[b].getPlotPoints()[bp + 1];
				}

				centerX /= (buildings[b].getPlotPoints().size() / 2);
				centerY /= (buildings[b].getPlotPoints().size() / 2);

				Path* nearestPath = NULL;
				float minDist = FLT_MAX;

				for(size_t p = 0; p < patches[x][y].getPaths().size(); ++p){
					const std::vector<float>& pPoints = patches[x][y].getPaths()[p].getPathPoints();

					for(size_t pp = 0; pp < pPoints.size() - 1; pp += 2){
						float dist = (pPoints[pp] - centerX) * (pPoints[pp] - centerX) +
									(pPoints[pp + 1] - centerY) * (pPoints[pp + 1] - centerY);
						if(dist < minDist){
							minDist = dist;
							nearestPath = &(patches[x][y].getPaths()[p]);
						}
					}
				}

				removeBuildings[nearestPath].push_back(&(buildings[b]));
			}
		}
	}

	std::unordered_map<Path*, std::vector<Building*> >::iterator iter;
	for(iter = removeBuildings.begin(); iter != removeBuildings.end(); ++iter){
		if(!iter->first->exportStatus()){
			for(size_t i = 0; i < iter->second.size(); ++i)
				iter->second[i]->setExportStatus(false);
		}
	}
};



/* ------------------- Export functions ------------------- */

void PatchMap::exportOBJ(const std::string& filePath) const{
	ModelExporter objExp;

	for(size_t x = 0; x < xTiles; ++x){
		for(size_t y = 0; y < yTiles; ++y){
			const std::vector<Building> buildings = patches[x][y].getBuildings();
			const std::vector<Path> paths = patches[x][y].getPaths();	

			for(size_t j = 0; j < buildings.size(); ++j)
				objExp.addBuilding(buildings[j]);

			for(size_t j = 0; j < paths.size(); ++j)
				objExp.addPath(paths[j]);
		}
	}

	objExp.exportModel(filePath);
};

void PatchMap::exportSVG(const std::string& filePath) const{
	SVGExporter svgExp;

	svgExp.setGrid(SVGGrid(patchWidth, patchHeight, xTiles, yTiles, DEFAULT_GRID_LINE_WIDTH));
	svgExp.enableGrid();

	for(size_t x = 0; x < xTiles; ++x){
		for(size_t y = 0; y < yTiles; ++y){
			const std::vector<Building> buildings = patches[x][y].getBuildings();
			const std::vector<Path> paths = patches[x][y].getPaths();

			for(size_t j = 0; j < buildings.size(); ++j)
				svgExp.addBuilding(buildings[j]);

			for(size_t j = 0; j < paths.size(); ++j)
				svgExp.addPath(paths[j]);
		}
	}

	svgExp.exportSVG(filePath);
};

void PatchMap::exportDetails(const std::string& filePath, const std::string& mapName) const{
	std::ofstream fOut;
	
	fOut.open(filePath.c_str(), std::ios_base::app);

	if(fOut.is_open()){
		fOut << mapName << "\t";

		float localQuality = 0;

		int worstX = 0, worstY = 0;
		float worstVal = 0;
		float modifications = 0;

		for(size_t x = 0; x < xTiles; ++x){
			for(size_t y = 0; y < yTiles; ++y){
				float upQuality = 0, downQuality = 0, leftQuality = 0, rightQuality = 0;
				int numTiles = 0;

				modifications += patches[x][y].getModification();

				if(x < xTiles - 1){
					++numTiles;
					rightQuality = patches[x + 1][y].getBaseFragment().compareTo(patches[x][y].getBaseFragment(),
																			MapFragment::RIGHT);
				}

				if(y < yTiles - 1){
					++numTiles;
					downQuality = patches[x][y + 1].getBaseFragment().compareTo(patches[x][y].getBaseFragment(),
																			MapFragment::BELOW);
				}

				if(x > 0){
					++numTiles;
					leftQuality = patches[x][y].getBaseFragment().compareTo(patches[x - 1][y].getBaseFragment(),
																			MapFragment::RIGHT);
				}

				if(y > 0){
					++numTiles;
					upQuality = patches[x][y].getBaseFragment().compareTo(patches[x][y - 1].getBaseFragment(),
																			MapFragment::BELOW);
				}

				float tileQuality = (upQuality + downQuality + leftQuality + rightQuality) / numTiles;

				if(tileQuality > worstVal){
					worstX = x;
					worstY = y;
					worstVal = tileQuality;
				}

				localQuality += tileQuality;
				
			}
		}

		localQuality = round(localQuality, 0.01f);
		modifications = round(modifications, 0.01f);

		fOut << localQuality << "\t" << worstX << "," << worstY << "\t" << modifications << "\t";
	}

	const RoadGraph& graph = getRoadGraph();

	fOut << graph.numFloatingRoads() << "\n";

	fOut.close();
};

void PatchMap::exportGraphDetails(const std::string& filePath) const{
	getRoadGraph().exportRoadSets(filePath);
};
