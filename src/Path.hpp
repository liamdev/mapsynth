/****************************************************************************
File name: Path.hpp

Description: Class containing the nodes which make up a road. This project uses path/road interchangeably.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/

#ifndef PATH_HPP
#define PATH_HPP

#include <vector>

#include "ContainerTools.hpp"
#include "MathTools.hpp"

class Path{

	public:
		Path(const std::vector<float>& pathPoints, float size) :
			pathPoints(pathPoints),
			size(size),
			pathExport(true){};
		
		//Element access.
		std::vector<float>& getVertices()  {return vertices;};
		std::vector<float>& getPathPoints(){return pathPoints;};

		//Const element access.
		const std::vector<float>& getVertices() const {return vertices;};
		const std::vector<float>& getPathPoints() const {return pathPoints;};

		const float getSize() const {return size;};
		
		bool exportStatus(){return pathExport;}; 

		void setSize(float s){size = s;};

		void setExportStatus(bool status){pathExport = status;};

		void recalculateVertices();

		// Total calculated from the length of each individual segment.
		float getLength() const;

	private:
		std::vector<float> pathPoints;
		std::vector<float> vertices;
		
		// The road's width.
		float size;

		bool pathExport;
};

#endif
