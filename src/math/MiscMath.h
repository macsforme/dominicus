// MiscMath.h
// Crucible Island

#ifndef MISCMATH_H
#define MISCMATH_H

#include <cmath>

#include "math/ScalarMath.h"
#include "math/VectorMath.h"

inline float getAngle(Vector2 point) {
	float angle = -degrees(atan2(point.y, point.x));
	return angle < 0.0f ? angle += 360.0f : angle;

	return angle;
}

inline Vector3 getNormal(Vector3 vec0, Vector3 vec1, Vector3 vec2) {
	Vector3 v1(vec1 - vec0);
	Vector3 v2(vec2 - vec1);

	Vector3 normal = cross(v1, v2);
	normal.norm();

	return normal;
}

inline int roundToInt(float number) {
	return (number > 0.0f ? number + 0.5f : number - 0.5f);
}

#endif // MISCMATH_H
