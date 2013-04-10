#include "Building.hpp"

Building::Building(const std::vector<float>& plotPoints) : plotPoints(plotPoints){
	//recalculateVertices();
};

void Building::recalculateVertices(){
	vertices.clear();
	std::size_t size = plotPoints.size();

	float xSum = 0, ySum = 0;
	float minX = FLT_MAX, minY = FLT_MAX;
	float maxX = -1, maxY = -1;

	for(std::size_t i = 0; i < size / 2.0f; i++){
		xSum += plotPoints[i * 2];
		ySum += plotPoints[i * 2 + 1];

		if(plotPoints[i * 2] < minX)
			minX = plotPoints[i * 2];
		if(plotPoints[i * 2 + 1] < minY)
			minY = plotPoints[i * 2 + 1];
		if(plotPoints[i * 2] > maxX)
			maxX = plotPoints[i * 2];
		if(plotPoints[i * 2 + 1] > maxY)
			maxY = plotPoints[i * 2 + 1];
	}

	float height = std::pow((maxX - minX) * (maxY - minY), 0.5f);

	float xCenter = xSum / (size / 2.0f);
	float yCenter = ySum / (size / 2.0f);

	for(std::size_t i = 0; i < plotPoints.size(); i += 2){
		addVertex<float>(vertices, plotPoints[i], plotPoints[i + 1], 0.0f);
		addVertex<float>(vertices, plotPoints[(i + 2) % size], plotPoints[(i + 3) % size], 0.0f);
		addVertex<float>(vertices, plotPoints[i], plotPoints[i + 1], height);

		addVertex<float>(vertices, plotPoints[(i + 2) % size], plotPoints[(i + 3) % size], 0.0f);
		addVertex<float>(vertices, plotPoints[(i + 2) % size], plotPoints[(i + 3) % size], height);
		addVertex<float>(vertices, plotPoints[i], plotPoints[i + 1], height);

		//TODO: Calculate building roof/floor properly by triangulating polygon.
		vertices.push_back(plotPoints[i]);
		vertices.push_back(plotPoints[i + 1]);
		vertices.push_back(height);

		vertices.push_back(plotPoints[(i + 2) % size]);
		vertices.push_back(plotPoints[(i + 3) % size]);
		vertices.push_back(height);

		vertices.push_back(xCenter);
		vertices.push_back(yCenter);
		vertices.push_back(height);
	}
};
