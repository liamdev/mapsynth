#include "Path.hpp"

// Calculate vertices for 3D model output.
void Path::recalculateVertices(){
	vertices.clear();

	// Show road width by offsetting vertex points by half the size in each direction
	// perpendicular to the road's angle.
	float startAngle = getDirectionRad(pathPoints[0], pathPoints[1], pathPoints[2], pathPoints[3]);
	float prevXOffset = (size / 2) * cos(startAngle);
	float prevYOffset = (size / 2) * sin(startAngle);

	for(std::size_t i = 0; i < pathPoints.size() - 2; i += 2){
		float angle = getDirectionRad(pathPoints[i], pathPoints[i + 1], pathPoints[i + 2], pathPoints[i + 3]);
		float xOffset = (size / 2) * cos(angle);
		float yOffset = (size / 2) * sin(angle);

		// Road segment triangle 1.
		addVertex<float>(vertices, pathPoints[i] - prevXOffset, pathPoints[i + 1] - prevYOffset, 0.0f);
		addVertex<float>(vertices, pathPoints[i] + prevXOffset, pathPoints[i + 1] + prevYOffset, 0.0f);
		addVertex<float>(vertices, pathPoints[i + 2] - xOffset, pathPoints[i + 3] - yOffset, 0.0f);

		// Road segment triangle 2.
		addVertex<float>(vertices, pathPoints[i] + prevXOffset, pathPoints[i + 1] + prevYOffset, 0.0f);
		addVertex<float>(vertices, pathPoints[i + 2] + xOffset, pathPoints[i + 3] + yOffset, 0.0f);
		addVertex<float>(vertices, pathPoints[i + 2] - xOffset, pathPoints[i + 3] - yOffset, 0.0f);

		prevXOffset = xOffset;
		prevYOffset = yOffset;
	}
};

// Calculate length by adding individual segment lengths.
float Path::getLength() const{
	if(pathPoints.size() < 4)
		return 0;

	float lengthTotal = 0;

	for(size_t i = 0; i < pathPoints.size() - 3; i += 2){
		float xDist = (pathPoints[i] - pathPoints[i + 2]) * (pathPoints[i] - pathPoints[i + 2]);
		float yDist = (pathPoints[i + 1] - pathPoints[i + 3]) * (pathPoints[i + 1] - pathPoints[i + 3]);

		lengthTotal += std::sqrt(xDist + yDist);
	};

	return lengthTotal;
};