/****************************************************************************
File name: Building.hpp

Description: Class representing a building's 2D and 3D positioning.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/

#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <cfloat>
#include <cmath>

#include "ContainerTools.hpp"

class Building{

	public:
		explicit Building(const std::vector<float>& plotPoints):
			plotPoints(plotPoints),
			buildingExport(true){};
		
		//Element access
		std::vector<float>& getVertices()  {return vertices;};
		std::vector<float>& getPlotPoints(){return plotPoints;};

		//Const element access
		const std::vector<float>& getVertices() const {return vertices;};
		const std::vector<float>& getPlotPoints() const {return plotPoints;};

		bool exportStatus(){return buildingExport;}; 

		void setExportStatus(bool status){buildingExport = status;};

		void recalculateVertices();

		float getArea() const;

	private:
		// Building's 2D outline.
		std::vector<float> plotPoints;
		//Vertices for 3D model version of the building.
		std::vector<float> vertices;

		// Flag for whether or not to include in SVG/OBJ exporting.
		bool buildingExport;

};

#endif
