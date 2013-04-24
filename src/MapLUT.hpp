/****************************************************************************
File name: MapLUT.hpp

Description: Class for storing and retrieving extracted fragments.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/

#ifndef MAP_LUT_HPP
#define MAP_LUT_HPP

#include <cstdlib>
#include <iostream>
#include <vector>

#include "City.hpp"
#include "Config.hpp"
#include "Patch.hpp"
#include "SVGExporter.hpp"

class MapLUT{

	public:
		// Extract fragments from a city and store in the LUT.
		void addMap(const City& city,
					float windowSize = DEFAULT_WINDOW_SIZE,
					float offset = DEFAULT_OFFSET);

		const std::vector<MapFragment>& getMaps() const {return pieces;};

		const Patch getRandomPatch() const;

		// Look for the best patch to go in a space, given its neighbours.
		const Patch getRightPatch(const Patch& left) const;
		const Patch getBelowPatch(const Patch& above) const;
		const Patch getDownRightPatch(const Patch& left, const Patch& above) const;

		bool isEmpty() const {return pieces.size() == 0;};

	private:
		std::vector<MapFragment> pieces;

};

#endif