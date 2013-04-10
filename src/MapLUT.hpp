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
		void addMap(const City& city,
					float windowSize = DEFAULT_WINDOW_SIZE,
					float offset = DEFAULT_OFFSET);

		const std::vector<MapFragment>& getMaps() const {return pieces;};

		const Patch getRandomPatch() const;

		const Patch getRightPatch(const Patch& left) const;
		const Patch getBelowPatch(const Patch& above) const;
		const Patch getDownRightPatch(const Patch& left, const Patch& above) const;

		float getAverageBuildingDensity() const;

		bool isEmpty() const {return pieces.size() == 0;};

	private:
		std::vector<MapFragment> pieces;

		std::vector<float> buildingDensities;

};

#endif