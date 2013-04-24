/****************************************************************************
File name: City.hpp

Description: Class holding the road and building data associated with a city.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/

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
		explicit City(){};
		explicit City(const std::string& filePath);

		int getWidth()  const {return width;};
		int getHeight() const {return height;};

		void fillFragment(MapFragment& frag, float xPos, float yPos, float width, float height) const;

		// Get list enumerating each unique road size in the city.
		std::vector<float> getSizes() const {return sizes;};

	private:
		// Original data (fixed).
		std::vector<Building*> buildings;
		std::vector<Path> paths;

		std::vector<float> sizes;

		int width;
		int height;
};

#endif
