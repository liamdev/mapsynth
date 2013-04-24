#include "Building.hpp"

//Turn 2D outline into 3D building model.
void Building::recalculateVertices(){
	vertices.clear();
	size_t size = plotPoints.size();

	float xSum = 0, ySum = 0;
	float minX = FLT_MAX, minY = FLT_MAX;
	float maxX = -1, maxY = -1;

	// Calculate averaged centre point, as well as bounding box corners.
	for(size_t i = 0; i < size / 2.0f; i++){
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

	// Estimate building height from bounding box size.
	float height = std::pow((maxX - minX) * (maxY - minY), 0.5f);

	float xCenter = xSum / (size / 2.0f);
	float yCenter = ySum / (size / 2.0f);

	for(size_t i = 0; i < plotPoints.size(); i += 2){
		// Wall triangle 1.
		addVertex<float>(vertices, plotPoints[i], plotPoints[i + 1], 0.0f);
		addVertex<float>(vertices, plotPoints[(i + 2) % size], plotPoints[(i + 3) % size], 0.0f);
		addVertex<float>(vertices, plotPoints[i], plotPoints[i + 1], height);
		// Wall triangle 2.
		addVertex<float>(vertices, plotPoints[(i + 2) % size], plotPoints[(i + 3) % size], 0.0f);
		addVertex<float>(vertices, plotPoints[(i + 2) % size], plotPoints[(i + 3) % size], height);
		addVertex<float>(vertices, plotPoints[i], plotPoints[i + 1], height);

		// Building roof is triangulated from edges to average centre point.
		// Can cause rendering issues with concave building shapes.
		// TODO: Replace with proper triangulation method.
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

// Return area estimation (based on axis-aligned bounding box).
float Building::getArea() const{
	float minX = FLT_MAX, minY = FLT_MAX;
	float maxX = -1, maxY = -1;

	// Get corners of AABB.
	for(size_t i = 0; i < plotPoints.size() - 1; i += 2){
		if(plotPoints[i] < minX)
			minX = plotPoints[i];
		if(plotPoints[i] > maxX)
			maxX = plotPoints[i];
		if(plotPoints[i + 1] < minY)
			minY = plotPoints[i + 1];
		if(plotPoints[i + 1] > maxY)
			maxY = plotPoints[i + 1];
	}

	return (maxX - minX) * (maxY - minY);
};
