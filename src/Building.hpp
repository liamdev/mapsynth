#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <cfloat>
#include <cmath>
#include <vector>

#include "ContainerTools.hpp"

class Building{

	public:
		explicit Building(const std::vector<float>& plotPoints);
		
		//Element access
		std::vector<float>& getVertices()  {return vertices;};
		std::vector<float>& getPlotPoints(){return plotPoints;};

		//Const element access
		const std::vector<float>& getVertices() const {return vertices;};
		const std::vector<float>& getPlotPoints() const {return plotPoints;};

		bool exportStatus(){return buildingExport;}; 

		void setExportStatus(bool status){buildingExport = status;};

		void recalculateVertices();

	private:
		std::vector<float> plotPoints;
		std::vector<float> vertices;

		bool buildingExport;

};

#endif
