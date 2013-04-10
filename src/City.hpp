#ifndef CITY_HPP
#define CITY_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "ContainerTools.hpp"
#include "FileTools.hpp"
#include "XMLTools.hpp"

#include "Building.hpp"
#include "MapFragment.hpp"
#include "Path.hpp"

class City{

	public:
		City() : density(0){};
		City(const std::string& filePath);

		int getWidth()  const {return width;};
		int getHeight() const {return height;};

		void fillFragment(MapFragment& frag, float xPos, float yPos, float width, float height) const;

		std::vector<float> getSizes() const {return sizes;};

		float getBuildingDensity() const {return density;};

	private:
		//Original data (fixed).
		std::vector<Building*> buildings;
		std::vector<Path> paths;

		std::vector<float> sizes;

		int width;
		int height;

		float density;
};

#endif
