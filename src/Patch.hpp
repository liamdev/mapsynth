/****************************************************************************
File name: Patch.hpp

Description: 'Solid' copy of a MapFragment object. Contains building data instead of pointers.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/

#ifndef PATCH_HPP
#define PATCH_HPP

#include "MapFragment.hpp"

class Patch{

	public:
		explicit Patch(const MapFragment& frag);

		// Alter patch's positioning, including that of the road and building data within.
		void positionAtZero();
		void offset(float xOffset, float yOffset);

		void updateVertices();

		void cacheEdgePoints();

		// Element access.
		std::vector<Building>& getBuildings(){return buildingCopies;};
		std::vector<Path>&     getPaths(){return pathCopies;};

		const std::vector<Building>& getBuildings() const {return buildingCopies;};
		const std::vector<Path>&     getPaths()     const {return pathCopies;};

		const MapFragment& getBaseFragment() const {return baseFragment;};

		std::vector<RoadEnd>& getTopRoads()    {return topRoads;};
		std::vector<RoadEnd>& getBottomRoads() {return bottomRoads;};
		std::vector<RoadEnd>& getLeftRoads()   {return leftRoads;};
		std::vector<RoadEnd>& getRightRoads()  {return rightRoads;};

		// Level of modification (i.e. translation of endpoints during smoothing).
		float getModification() const {return modifications;};
		
		void addModification(float amount){modifications += amount;};

		float getBuildingDensity() const;
		float getPathDensity() const;

	private:
		MapFragment baseFragment;

		std::vector<Building> buildingCopies;
		std::vector<Path> pathCopies;

		std::vector<RoadEnd> topRoads;
		std::vector<RoadEnd> bottomRoads;
		std::vector<RoadEnd> leftRoads;
		std::vector<RoadEnd> rightRoads;

		float xMin, yMin;
		float xMax, yMax;

		float modifications;
};

#endif