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

inline float maximum(float num1, float num2) {
	return (num1 > num2 ? num1 : num2);
}

inline float minimum(float num1, float num2) {
	return (num1 < num2 ? num1 : num2);
}

inline float round(float num, unsigned short int places) {
	return((float) ((int) (num * pow(10.0f, (float) places))) / pow(10.0f, (float) places));
}

#ifdef WIN32
inline double log2(double n)
{
	return log( n ) / log( 2.0 );  
}
#endif
#endif
