#ifndef PATH_HPP
#define PATH_HPP

#include <vector>

#include "ContainerTools.hpp"
#include "MathTools.hpp"

class Path{

	public:
		Path(const std::vector<float>& pathPoints, float size);
		
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

	private:
		std::vector<float> pathPoints;
		std::vector<float> vertices;

		float size;
		bool pathExport;
};

#endif
