/****************************************************************************
File name: MathTools.hpp

Description: Various mathematical and geometric functions.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/

#ifndef MATH_TOOLS_HPP
#define MATH_TOOLS_HPP

#include <cmath>
#include <vector>

namespace Math{

	// Float equality tolerance.
	const float EQ_TOLERANCE = 0.0001f;

	// Mathematical constants.
	const float PI = 3.14159265f;
	const float DEG_TO_RAD = Math::PI / 180.0f;
	const float RAD_TO_DEG = 1 / DEG_TO_RAD;

	// Outcodes for line clipping.
	const int INSIDE_OUTCODE = 0;
	const int LEFT_OUTCODE = 1;
	const int RIGHT_OUTCODE = 2;
	const int UP_OUTCODE = 4;
	const int DOWN_OUTCODE = 8;
};

// Compares equality of floats within a tolerance.
bool fEq(float f1, float f2, float tolerance = Math::EQ_TOLERANCE);

// Get the angle between two points, in either radians or degrees.
float getDirectionRad(float startX, float startY, float endX, float endY);
float getDirectionDeg(float startX, float startY, float endX, float endY);

// Gets the angle between a given point and the coordinate system origin.
float getAngleAroundOrigin(float x, float y);


// Return type for line clipping.
struct ClipResult{

	ClipResult(float x0 = 0, float y0 = 0, float x1 = 0, float y1 = 0, bool remove = true) :
		x0(x0), y0(y0), x1(x1), y1(y1), remove(remove)
	{};

	float x0, y0; //Start position.
	float x1, y1; //End position.

	bool remove; //Fully outside of clipping area?
};

// Clip the line to within a window.
ClipResult clipLine(float x0, float y0, float x1, float y1,
					float xMin, float yMin, float xMax, float yMax);

// Simple, quick rounding function.
// TODO: Fix? Rounds the wrong way for negative values.
template <typename T>
T round(T val, float precision){
	return std::floor(val * (1 / precision) + 0.5f) * precision;
}

#endif
