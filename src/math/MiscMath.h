/*
 * MiscMath.h
 * Dominicus
 *
 * Copyright 2010-2011, Joshua Bodine
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef MISCMATH_H
#define MISCMATH_H

// program headers
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"

inline float getAngle(Vector2 point) {
	// FIXME can we return 0 if only point.y == 0.0f?
	// FIXME are these angles backwards rotating?
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

#endif // MISCMATH_H
