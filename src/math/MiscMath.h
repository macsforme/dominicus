/*
 *  MiscMath.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 1/16/11.
 *  Copyright 2011 Joshua Bodine. All rights reserved.
 *
 */

// program headers
#include "VectorMath.h"

inline float getAngle(Vector2 point) {
	if(point.x == 0.0f && point.y == 0.0f)
		// avoid division by zero
		return 0.0f;

	Vector2 zeroAngle(1.0f, 0.0f);
	point.norm();
	float angle = degrees(acos(dot(zeroAngle, point)));
	if(point.y < 0.0f)
		angle = 360.0f - angle;

	return angle;
}
