// MiscMath.h
// Crucible Island

#ifndef MISCMATH_H
#define MISCMATH_H

// program headers
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"

inline float getAngle(Vector2 point) {
	if(point.x == 0.0f && point.y == 0.0f)
		// avoid division by zero
		return 0.0f;

	Vector2 zeroAngle(1.0f, 0.0f);
	point.norm();
	float angle = degrees(acos(dot(zeroAngle, point)));
	if(point.y > 0.0f)
		angle = 360.0f - angle;

	return angle;
}

inline Vector3 getNormal(Vector3 vec0, Vector3 vec1, Vector3 vec2) {
	Vector3 v1(vec1 - vec0);
	Vector3 v2(vec2 - vec1);

	Vector3 normal = cross(v1, v2);
	normal.norm();

	return normal;
}

/*
inline Matrix4 getXVectorTransformationMatrix(Vector4 vector) {
	Matrix4 transformationMatrix;
	transformationMatrix.identity();

	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(getAngle(Vector2(vector.x, vector.z))), transformationMatrix);	// x
	vector = vector * transformationMatrix;
	rotateMatrix(Vector3(0.0f, 0.0f, -1.0f), radians(getAngle(Vector2(vector.x, vector.y))), transformationMatrix);	// y

	return transformationMatrix;
}
*/

inline bool fEq(float a, float b) {
	return (absolute(a - b) < 0.001 ? true : false);
}

inline Matrix3 completeMatrix(Vector3 vector) {
	return Matrix3(
			vector.x,
			vector.y,
			vector.z,

			fEq(absolute(vector.y), 1.0f) ? -vector.y : -vector.y * vector.x / sqrt(1.0f - vector.y * vector.y),
			sqrt(vector.x * vector.x + vector.z * vector.z),
			fEq(absolute(vector.y), 1.0f) ? 0.0f : -vector.y * vector.z / sqrt(1.0f - vector.y * vector.y),

			fEq(absolute(vector.y), 1.0f) ? 0.0f : -vector.z / sqrt(vector.x * vector.x + vector.z * vector.z),
			0.0f,
			fEq(absolute(vector.y), 1.0f) ? 1.0f : vector.x / sqrt(vector.x * vector.x + vector.z * vector.z)
		);
}

inline int roundToInt(float number) {
	return (number > 0.0f ? number + 0.5f : number - 0.5f);
}

#endif // MISCMATH_H
