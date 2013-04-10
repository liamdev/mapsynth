#include "Main.hpp"

int main(int argc, char* argv[]){
	srand((unsigned int)time(NULL));
	rand();

	float windowSize = DEFAULT_WINDOW_SIZE;
	float offsetSize = DEFAULT_OFFSET;
	
	unsigned int xTiles = DEFAULT_OUTPUT_WIDTH;
	unsigned int yTiles = DEFAULT_OUTPUT_HEIGHT;

	//Argument parsing.
	switch(argc){
		case 1:
			break;
		case 4: //Cascade down the cases to apply all arguments.
			xTiles = std::atoi(argv[3]);
			yTiles = std::atoi(argv[3]);
		case 3:
			offsetSize = (float)std::atof(argv[2]);
		case 2:
			windowSize = (float)std::atof(argv[1]);
			break;
		default:
			std::cout << "Usage: mapsynth [window size [offset size [output size]]]\n";
			return -1;
	}

	if(windowSize == 0 || offsetSize == 0 || xTiles == 0 || yTiles == 0){
		std::cerr << "Invalid window, offset or output size.\n";
		return -1;
	}

	//Clear the details file.
	std::ofstream clearDetails(DETAILS_OUTPUT_PATH.c_str());
	clearDetails << "File name\tWeight\tWorst\tAlter\tFloating\n";
	clearDetails.close();

	//Create lookup table from example maps.
	std::vector<std::string> imgPaths = lsSVGFiles(RES_DIR);
	std::vector<City> cities;
	MapLUT lut;

	for(size_t i = 0; i < imgPaths.size(); ++i){
		//Load in city from file.
		std::cout << "Reading map: " << imgPaths[i] << "\n";
		cities.push_back(City(imgPaths[i]));
		
		//Divide city into fragments, store in the LUT.
		std::cout << "Extracting fragments from " << imgPaths[i] << "\n";
		lut.addMap(cities[i], windowSize, offsetSize);
		std::cout << "Fragments extracted!\n\n";

		PatchMap cityMap(lut, xTiles, yTiles, windowSize, windowSize);

		cityMap.fill(cities[i]);

		cityMap.exportDetails(DETAILS_OUTPUT_PATH, imgPaths[i].substr(imgPaths[i].size() - 15));
	}

	std::cout << "Stitching together final images...\n\n";

	std::string fileName = "map0.svg";
	for(int i = 0; i < 10; ++i){
		//Create patch image.
		PatchMap map(lut, xTiles, yTiles, windowSize, windowSize);

		map.fill();

		map.smoothSeams();

		map.generateRoadGraph();
		//map.removeFloatingRoads();

		map.exportSVG(fileName);

		map.exportDetails(DETAILS_OUTPUT_PATH, fileName);

		//map.updateVertices();
		//map.exportOBJ(fileName + ".obj");
		
		fileName[3] += 1;
	}

	return 0;
};
