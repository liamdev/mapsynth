#include "Main.hpp"

// Help message for command line use.
void printUsage(){
	std::cout << "Usage: mapsynth [window size] [offset size] [output size] [map count]";
	std::cout << "[remove floating roads] [export SVG] [export grid] [export OBJ]\n";
};

int main(int argc, char* argv[]){
	// Initialise RNG.
	srand((unsigned int)time(NULL));
	rand();

	// Set default values for various program parameters.
	float windowSize = DEFAULT_WINDOW_SIZE;
	float offsetSize = DEFAULT_OFFSET;
	
	unsigned int xTiles = DEFAULT_OUTPUT_WIDTH;
	unsigned int yTiles = DEFAULT_OUTPUT_HEIGHT;

	int numOutputs = 10;

	bool removeFRoads = false;
	bool exportSVG = true;
	bool exportGrid = false;
	bool exportOBJ = false;

	// Argument parsing.
	switch(argc){
		case 1:
			break;
		case 9: //Cascade down the cases to apply all arguments entered.
			exportOBJ = std::atoi(argv[8]) > 0;
		case 8:
			exportGrid = std::atoi(argv[7]) > 0;
		case 7:
			exportSVG = std::atoi(argv[6]) > 0;
		case 6:
			removeFRoads = std::atoi(argv[5]) > 0;
		case 5:
			numOutputs = std::atoi(argv[4]);
		case 4:
			xTiles = std::atoi(argv[3]);
			yTiles = std::atoi(argv[3]);
		case 3:
			offsetSize = (float)std::atof(argv[2]);
		case 2:
			if(std::string(argv[1]) != "--help"){ // Print usage message for --help command.
				windowSize = (float)std::atof(argv[1]);
				break;
			}
		default:
			printUsage();
			return -1;
	}

	// Check validity of input.
	if(windowSize == 0 || offsetSize == 0 || xTiles == 0 || yTiles == 0){
		std::cerr << "Invalid window, offset or output size.\n";
		return -1;
	}

	//Clear the details file.
	std::ofstream clearDetails(DETAILS_OUTPUT_PATH.c_str());
	clearDetails << "File name\tWeights\tW avg\tAlter\tA avg\tBD\t";
	clearDetails << "BD Avg\tPD\tPD Avg\tFR\tWorst\n";
	clearDetails.close();

	// Create lookup table from example maps.
	std::vector<std::string> imgPaths = lsSVGFiles(RES_DIR);
	std::vector<City> cities;
	MapLUT lut;

	// Load each detected SVG city file and extract its fragments.
	for(size_t i = 0; i < imgPaths.size(); ++i){
		std::cout << "Reading map: " << imgPaths[i] << "\n";
		cities.push_back(City(imgPaths[i]));
		
		std::cout << "Extracting fragments from " << imgPaths[i] << "\n";
		lut.addMap(cities[i], windowSize, offsetSize);
		std::cout << "Fragments extracted!\n\n";

		PatchMap cityMap(lut, xTiles, yTiles, windowSize, windowSize);

		cityMap.fill(cities[i]);

		// Get clipped map name for the metrics file.
		std::string abbrevPath = imgPaths[i].size() > 15 ? imgPaths[i].substr(imgPaths[i].size() - 15) : imgPaths[i];
		cityMap.exportDetails(DETAILS_OUTPUT_PATH, abbrevPath);
	}

	std::cout << "Stitching together final images...\n\n";

	// Generate a bunch of outputs.
	for(int i = 0; i < numOutputs; ++i){
		PatchMap map(lut, xTiles, yTiles, windowSize, windowSize);

		// Synthesise map, smooth the seams.
		map.fill();
		map.smoothSeams();

		// Optional: remove floating roads.
		if(removeFRoads){
			map.generateRoadGraph();
			map.removeFloatingRoads();
		}

		std::stringstream ss;
		ss << "map" << i << ".svg";

		// Optional: export SVG.
		if(exportSVG)
			map.exportSVG(ss.str(), exportGrid);

		map.exportDetails(DETAILS_OUTPUT_PATH, ss.str());

		// Optional: export OBJ.
		if(exportOBJ){
			map.updateVertices();

			ss.clear();
			ss.str("");
			ss << "map" << i << ".obj";

			map.exportOBJ(ss.str());
		}
	}

	return 0;
};
