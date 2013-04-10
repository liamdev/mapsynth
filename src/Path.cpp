#include "Path.hpp"

Path::Path(const std::vector<float>& pathPoints, float size) :
	pathPoints(pathPoints),
	size(size),
	pathExport(true)
{
	//recalculateVertices();
};

void Path::recalculateVertices(){
	vertices.clear();

	float startAngle = getDirectionRad(pathPoints[0], pathPoints[1], pathPoints[2], pathPoints[3]);
	float prevXOffset = (size / 2) * cos(startAngle);
	float prevYOffset = (size / 2) * sin(startAngle);

	for(std::size_t i = 0; i < pathPoints.size() - 2; i += 2){
		float angle = getDirectionRad(pathPoints[i], pathPoints[i + 1], pathPoints[i + 2], pathPoints[i + 3]);
		float xOffset = (size / 2) * cos(angle);
		float yOffset = (size / 2) * sin(angle);

		addVertex<float>(vertices, pathPoints[i] - prevXOffset, pathPoints[i + 1] - prevYOffset, 0.0f);
		addVertex<float>(vertices, pathPoints[i] + prevXOffset, pathPoints[i + 1] + prevYOffset, 0.0f);
		addVertex<float>(vertices, pathPoints[i + 2] - xOffset, pathPoints[i + 3] - yOffset, 0.0f);

		addVertex<float>(vertices, pathPoints[i] + prevXOffset, pathPoints[i + 1] + prevYOffset, 0.0f);
		addVertex<float>(vertices, pathPoints[i + 2] + xOffset, pathPoints[i + 3] + yOffset, 0.0f);
		addVertex<float>(vertices, pathPoints[i + 2] - xOffset, pathPoints[i + 3] - yOffset, 0.0f);

		prevXOffset = xOffset;
		prevYOffset = yOffset;
	}
};