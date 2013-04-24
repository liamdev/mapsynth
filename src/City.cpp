#include "City.hpp"

//Utility function for converting an SVG <path .../> element to the base points of a building.
std::vector<float> getBuildingPosFromString(const std::string xmlString){
	std::vector<float> buildingPos;

	std::string currentNum = "";

	//Temporary store for two elements (e.g. each x, y pair).
	std::vector<float> coords;

	bool started = false;
	bool horizontal = false;
	bool vertical = false;

	for(size_t i = 0; i < xmlString.size(); ++i){
		if(xmlString[i] == 'M'){ //New path flag
			if(!started)
				started = true;
			else
				break;
		} else if(xmlString[i] == 'h'){ //Horizontal movement flag
			horizontal = true;
		} else if(xmlString[i] == 'v'){ //Vertical movement flag
			vertical = true;
		} else if(xmlString[i] == 'l' || xmlString[i] == 'Z'){ //Ignore characters
			continue;
		} else if(xmlString[i] == ' '){ //Store the current number
			if(currentNum != ""){
				if(horizontal){
					//Add new point with only one co-ordinate (i.e. horizontal step).
					coords.push_back((float)std::atof(currentNum.c_str()));
					coords.push_back(0);
					horizontal = false;
				} else if(vertical){
					//Add new point with only one co-ordinate (i.e. vertical step).
					coords.push_back(0);
					coords.push_back((float)std::atof(currentNum.c_str()));
					vertical = false;
				} else {
					coords.push_back((float)std::atof(currentNum.c_str()));
				}
				if(coords.size() > 1){
					size_t size = buildingPos.size();
					if(size == 0){
						buildingPos.push_back(coords[0]);
						buildingPos.push_back(coords[1]);
					} else {
						buildingPos.push_back(coords[0] + buildingPos[size - 2]);
						buildingPos.push_back(coords[1] + buildingPos[size - 1]);
					}
					coords.clear();
				}
				currentNum = "";
			}
		} else {
			currentNum += xmlString[i];
		}
	}

	//Store the last number pair if no space is included after the last digit.
	if(currentNum != "" && coords.size() == 1 && buildingPos.size() > 1){
		buildingPos.push_back(coords[0] + buildingPos[buildingPos.size() - 2]);
		buildingPos.push_back((float)std::atof(currentNum.c_str()) + buildingPos[buildingPos.size() - 2]);
	}

	return buildingPos;
};


// Utility function to get the value of a given node's style attribute.
std::string getStyleValue(const std::string& styleStr, const std::string& attrName){

	// Check the attribute exists.
	int attrPos = styleStr.find(attrName);
	if(attrPos == -1)
		return "";

	// If the attribute exists, get its value.
	std::string val = styleStr.substr(attrPos + attrName.size() + 1);
	int endPoint = val.find(";");
	if(endPoint != -1)
		val = val.substr(0, endPoint);

	return val;
};

// Populate the city from a given file.
City::City(const std::string& filePath){

	// Get array of characters in the SVG file.
	std::string svg = readFile(filePath);
	std::vector<char> xmlChars(svg.begin(), svg.end());
	xmlChars.push_back('\0');

	try{
		//Attempt to parse the document with RapidXML.
		rxml::xml_document<> doc;
		doc.parse<rxml::parse_no_data_nodes>(&xmlChars[0]);
		
		rxml::xml_node<>* root = doc.first_node();
		if(root == NULL){
			std::cerr << "Invalid XML file: " << filePath << ". No root found.\n";
		}

		/* Assumes the following node id structure to fit with Maperitive's default output:
		 *
		 *	=> Map
		 *		=> Polygons
		 *			=> building__fill (contains building base points)
		 *		=> Line_artwork (contains path nodes)
		 *
		 * Set the XML nodes up appropriately:
		 */

		root = getChildNode(root, "Map");

		rxml::xml_node<>* polygonNode = getChildNode(root, "Polygons");
		rxml::xml_node<>* buildingNode = getChildNode(polygonNode, "building__fill");
		rxml::xml_node<>* pathNode = getChildNode(root, "Line_artwork");

		//Iterate through the buildings.
		rxml::xml_node<>* building = buildingNode->first_node();
		for(size_t i = 0; i < rxml::count_children(buildingNode); ++i){

			//Get the building base points.
			std::string buildingStr = building->first_attribute("d")->value();
			std::vector<float> buildingPos = getBuildingPosFromString(buildingStr);

			//Only store if the building is of sufficient size.
			if(buildingPos.size() > 4)
				buildings.push_back(new Building(buildingPos));

			building = building->next_sibling();
		}

		//Iterate through the paths. Paths are nested within an unknown number of sub-nodes of pathNode.
		//Requires iteration across these sub-nodes, as well as their contents (the paths themselves).
		rxml::xml_node<>* pathGroup = pathNode->first_node();
		rxml::xml_node<>* path = pathGroup->first_node();

		for(size_t i = 0; i < rxml::count_children(pathNode); ++i){
			//Get the stroke-width attribute to determine road width. Use default of 1 if no width provided.
			std::string width = getStyleValue(pathGroup->first_attribute("style")->value(), "stroke-width");
			float size = (float)std::atof(width.c_str());
			if(size == 0)
				size = 1;

			//Build the size hierarchy for the city's paths.
			addUnique(sizes, size);

			//Add paths iteratively, similar to buildings.
			for(size_t j = 0; j < rxml::count_children(pathGroup); ++j){
				std::string pathStr = path->first_attribute("d")->value();

				std::vector<float> pathPos = getBuildingPosFromString(pathStr);

				if(pathPos.size() > 3)
					paths.push_back(Path(pathPos, size));

				path = path->next_sibling();
			}
			
			pathGroup = pathGroup->next_sibling();
			if(pathGroup != NULL)
				path = pathGroup->first_node();
		}

		//Calculate city width/height.
		float maxX = -1;
		float maxY = -1;

		//Check building positions.
		for(size_t i = 0; i < buildings.size(); ++i){
			const std::vector<float>& plotPoints = buildings[i]->getPlotPoints();

			for(size_t j = 0; j < plotPoints.size() - 1; j += 2){
				if(plotPoints[j] > maxX)
					maxX = plotPoints[j];
				if(plotPoints[j + 1] > maxY)
					maxY = plotPoints[j + 1];
			}
		}

		//Check road positions.
		for(size_t i = 0; i < paths.size(); ++i){
			const std::vector<float>& pathPoints = paths[i].getPathPoints();

			for(size_t j = 0; j < pathPoints.size() - 1; j += 2){
				if(pathPoints[j] > maxX)
					maxX = pathPoints[j];
				if(pathPoints[j + 1] > maxY)
					maxY = pathPoints[j + 1];
			}
		}

		//Convert to int (add one for tolerance).
		width = (int)(maxX + 1);
		height = (int)(maxY + 1);

		//Sort the path sizes in descending order.
		std::sort(sizes.begin(), sizes.end(), std::greater<int>());

	} catch(const std::exception& err){
		//Catch exceptions, most likely XMLNodeNotFoundException.
		std::cerr << err.what() << "\n";
	}
};

// Get the road/building data contained within the specified window area.
void City::fillFragment(MapFragment& frag, float xPos, float yPos, float width, float height) const{

	//Define window boundaries.
	float xLimit = xPos + width;
	float yLimit = yPos + height;

	frag = MapFragment((float)xPos, (float)yPos, (float)xLimit, (float)yLimit);

	//Add buildings which fit fully within the window.
	for(size_t i = 0; i < buildings.size(); ++i){
		const std::vector<float>& plotPoints = buildings[i]->getPlotPoints();

		bool invalid = false;
		for(size_t j = 0; j < plotPoints.size() - 1; j += 2){
			if(plotPoints[j] < xPos || plotPoints[j] > xLimit ||
				plotPoints[j + 1] < yPos || plotPoints[j + 1] > yLimit){
				invalid = true;
				break;
			}
		}

		if(!invalid)
			frag.addBuilding(buildings[i]);
	}

	//Add paths to the fragment, clipping those that enter/exit.
	for(size_t i = 0; i < paths.size(); ++i){
		const std::vector<float>& pathPoints = paths[i].getPathPoints();

		std::vector<float> clippedPath;

		for(size_t j = 0; j < pathPoints.size() - 3; j += 2){
			// Get the clipped version of each of the path segments.
			ClipResult clipped = clipLine(pathPoints[j], pathPoints[j + 1],
											pathPoints[j + 2], pathPoints[j + 3],
											(float)xPos, (float)yPos, (float)xLimit, (float)yLimit);
			// Exclude those segments which are fully clipped.
			if(clipped.remove)
				continue;

			size_t size = clippedPath.size();

			// Checks if a 'new' path is required (i.e. the clipped version exits the window then re-enters).
			bool newPath = false;
			if(size < 2){
				newPath = true;
			} else if(clippedPath[size - 2] != clipped.x1 &&
						clippedPath[size - 1] != clipped.y1){
				newPath = true;

				frag.addPath(Path(clippedPath, paths[i].getSize()));
			}

			if(newPath){
				clippedPath.clear();
				clippedPath.push_back(clipped.x0);
				clippedPath.push_back(clipped.y0);
			}

			clippedPath.push_back(clipped.x1);
			clippedPath.push_back(clipped.y1);
		}

		// Exclude empty paths.
		if(clippedPath.size() > 0)
			frag.addPath(Path(clippedPath, paths[i].getSize()));
	}

	// Calculate the edge points within the fragment.
	frag.cacheEdgePoints();
};
