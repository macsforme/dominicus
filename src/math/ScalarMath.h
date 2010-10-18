/*
 *  ScalarMath.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 8/23/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef SCALARMATH_H
#define SCALARMATH_H

// library headers
#include <math.h>

// constants
#define PI 3.14159265

// common functions
inline float radians(float deg) {
	return (deg * PI / 180);
}

inline float degrees(float rad) {
	return (rad * 180 / PI);
}

inline float absolute(float num) {
	return (num < 0.0f ? -num : num);
}

#endif


