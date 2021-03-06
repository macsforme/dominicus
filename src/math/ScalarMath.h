// ScalarMath.h
// Dominicus

#ifndef SCALARMATH_H
#define SCALARMATH_H

#include <cmath>
#include <math.h>

#define PI 3.14159265

inline float radians(float deg) {
	return (float)(deg * PI / 180);
}

inline float degrees(float rad) {
	return (float)(rad * 180 / PI);
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
	return (float) ((int) (num * pow(10.0f, (float) places))) / pow(10.0f, (float) places);
}

#endif // SCALARMATH_H
