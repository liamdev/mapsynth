#include "MathTools.hpp"

// Equality within a tolerance.
bool fEq(float f1, float f2, float tolerance){
	return fabs(f1 - f2) < tolerance;
};

float getAngleAroundOrigin(float x, float y){
	//Prevent divide-by-zero errors.
	if(fEq(y, 0.0f)){
		if(x > 0)
			return Math::PI / 2;
		else if(x < 0)
			return -Math::PI / 2;
		else
			return 0;
	}

	return -atan(x / y) + ((y < 0) ? 0 : Math::PI);
};

float getDirectionRad(float startX, float startY, float endX, float endY){
	//Get relative direction of the points.
	float x = endX - startX;
	float y = endY - startY;

	//Angle of point 2 relative to point 1 is equivalent to the angle between their difference and the origin.
	return getAngleAroundOrigin(x, y);
};

float getDirectionDeg(float startX, float startY, float endX, float endY){
	return getDirectionRad(startX, startY, endX, endY) * Math::DEG_TO_RAD;
};

// Find which area in/around the clipping window contains the point.
int getOutCode(float x, float y, float xMin, float yMin, float xMax, float yMax){
	int code = 0;

	if(x < xMin)
		code |= Math::LEFT_OUTCODE;
	else if(x > xMax)
		code |= Math::RIGHT_OUTCODE;

	if(y < yMin)
		code |= Math::UP_OUTCODE;
	else if(y > yMax)
		code |= Math::DOWN_OUTCODE;

	return code;
};

//Implementation of the Cohen-Sutherland clipping algorithm.
ClipResult clipLine(float x0, float y0, float x1, float y1,
					float xMin, float yMin, float xMax, float yMax){
	ClipResult result;

	int startCode = getOutCode(x0, y0, xMin, yMin, xMax, yMax);
	int endCode   = getOutCode(x1, y1, xMin, yMin, xMax, yMax);

	//Line is entirely within the clipping window.
	if((startCode | endCode) == 0){
		return ClipResult(x0, y0, x1, y1, false);
	}

	//Line does not cross the clipping window, remove it.
	if(startCode & endCode)
		return ClipResult(x0, y0, x1, y1, true);

	int clipCode = startCode ? startCode : endCode;
	float xClip = 0;
	float yClip = 0;

	if(clipCode & Math::UP_OUTCODE){ // Get x intersection with window's top border.
		xClip = x0 + ((yMin - y0) / (y1 - y0)) * (x1 - x0);
		yClip = yMin;
	} else if(clipCode & Math::DOWN_OUTCODE){ // Get x intersection with window's bottom border.
		xClip = x0 + ((yMax - y0) / (y1 - y0)) * (x1 - x0);
		yClip = yMax;
	} else if(clipCode & Math::LEFT_OUTCODE){ // Get y intersection with window's left border.
		xClip = xMin;
		yClip = y0 + ((xMin - x0) / (x1 - x0)) * (y1 - y0);
	} else if(clipCode & Math::RIGHT_OUTCODE){ // Get y intersection with window's right border.
		xClip = xMax;
		yClip = y0 + ((xMax - x0) / (x1 - x0)) * (y1 - y0);
	}

	// Update coordinates of the result.
	if(clipCode == startCode){
		x0 = xClip;
		y0 = yClip;
	} else {
		x1 = xClip;
		y1 = yClip;
	}

	// Recursive, but maximum of two calls (if in a corner region), so little overhead.
	return clipLine(x0, y0, x1, y1, xMin, yMin, xMax, yMax);
};

