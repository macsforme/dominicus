// VectorMath.h
// Crucible Island

#ifndef VECTORMATH_H
#define VECTORMATH_H

// program headers
#include "math/ScalarMath.h"

// library headers
#include <math.h>

class Vector2 {
public:
	// public data
	float x, y;

	// constructors
	Vector2() { }
	Vector2(const Vector2& vec) : x(vec.x), y(vec.y) { }
	Vector2(float newX, float newY) : x(newX), y(newY) { }

	// assignment and comparison operators
	Vector2& operator = (const Vector2& vec) { x = vec.x; y = vec.y; return *this; }
	bool operator == (const Vector2& vec) const { return x == vec.x && y == vec.y; }
	bool operator != (const Vector2& vec) const { return x != vec.x || y != vec.y; }

	// arithmetic operators
	Vector2 operator - () const { return Vector2(-x, -y); }
	Vector2 operator + (const Vector2& vec) const { return Vector2(x + vec.x, y + vec.y); }
	Vector2& operator += (const Vector2& vec) { x += vec.x; y += vec.y; return *this; }
	Vector2 operator - (const Vector2& vec) const { return Vector2(x - vec.x, y - vec.y); }
	Vector2& operator -= (const Vector2& vec) { x -= vec.x; y -= vec.y; return *this; }
	Vector2 operator * (float scal) const { return Vector2(x * scal, y * scal); }
	Vector2& operator *= (float scal) { x *= scal; y *= scal; return *this; }
	Vector2 operator / (float scal) const { return Vector2(x / scal, y / scal); }
	Vector2& operator /= (float scal) { x /= scal; y /= scal; return *this; }

	// manipulation functions
	void zero() { x = 0.0f; y = 0.0f; }
	void norm() {
		float mag = sqrt(x * x + y * y);

		if(mag > 0.0f) {
			x /= mag;
			y /= mag;
		}
	}
};

// non-member vector-related functions
inline Vector2 operator * (float scal, const Vector2& vec) { return Vector2(scal * vec.x, scal * vec.y); }
inline float dot(Vector2 vec1, Vector2 vec2) { return vec1.x * vec2.x + vec1.y * vec2.y; }
inline Vector2 cross(Vector2& vec1, Vector2& vec2) {
	return Vector2(
			vec1.y * vec2.x - vec1.x * vec2.y,
			vec1.x * vec2.y - vec1.y * vec2.x
		);
}
inline float distance(Vector2 vec1, Vector2 vec2) {
	float distX = vec1.x - vec2.x;
	float distY = vec1.y - vec2.y;

	return sqrt(distX * distX + distY * distY);
}
inline float mag(Vector2 vec) {
	Vector2 midPoint(0.0f, 0.0f);

	return absolute(distance(midPoint, vec));
}

class Vector3 {
public:
	// public data
	float x, y, z;

	// constructors
	Vector3() { }
	Vector3(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z) { }
	Vector3(float newX, float newY, float newZ) : x(newX), y(newY), z(newZ) { }

	// assignment and comparison operators
	Vector3& operator = (const Vector3& vec) { x = vec.x; y = vec.y; z = vec.z; return *this; }
	bool operator == (const Vector3& vec) const { return x == vec.x && y == vec.y && z == vec.z; }
	bool operator != (const Vector3& vec) const { return x != vec.x || y != vec.y || z != vec.z; }

	// arithmetic operators
	Vector3 operator - () const { return Vector3(-x, -y, -z); }
	Vector3 operator + (const Vector3& vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
	Vector3& operator += (const Vector3& vec) { x += vec.x; y += vec.y; z += vec.z; return *this; }
	Vector3 operator - (const Vector3& vec) const { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
	Vector3& operator -= (const Vector3& vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }
	Vector3 operator * (float scal) const { return Vector3(x * scal, y * scal, z * scal); }
	Vector3& operator *= (float scal) { x *= scal; y *= scal; z *= scal; return *this; }
	Vector3 operator / (float scal) const { return Vector3(x / scal, y / scal, z / scal); }
	Vector3& operator /= (float scal) { x /= scal; y /= scal; z /= scal; return *this; }

	// manipulation functions
	void zero() { x = 0.0f; y = 0.0f; z = 0.0f; }
	void norm() {
		float mag = sqrt(x * x + y * y + z * z);

		if(mag > 0.0f) {
			x /= mag;
			y /= mag;
			z /= mag;
		}
	}
};

// non-member vector-related functions
inline Vector3 operator * (float scal, const Vector3& vec) { return Vector3(scal * vec.x, scal * vec.y, scal * vec.z); }
inline float dot(Vector3 vec1, Vector3 vec2) { return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z; }
inline Vector3 cross(Vector3& vec1, Vector3& vec2) {
	return Vector3(
			vec1.y * vec2.z - vec1.z * vec2.y,
			vec1.z * vec2.x - vec1.x * vec2.z,
			vec1.x * vec2.y - vec1.y * vec2.x
		);
}
inline float distance(Vector3 vec1, Vector3 vec2) {
	float distX = vec1.x - vec2.x;
	float distY = vec1.y - vec2.y;
	float distZ = vec1.z - vec2.z;

	return sqrt(distX * distX + distY * distY + distZ * distZ);
}
inline float mag(Vector3 vec) {
	Vector3 midPoint(0.0f, 0.0f, 0.0f);

	return absolute(distance(midPoint, vec));
}
inline Vector3 reflect(Vector3 vec, Vector3 normal) {
	return 2.0f * dot(normal, vec) * normal - vec;
}

class Vector4 {
public:
	// public data
	float x, y, z, w;

	// constructors
	Vector4() { }
	Vector4(const Vector4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) { }
	Vector4(float newX, float newY, float newZ, float newW) : x(newX), y(newY), z(newZ), w(newW) { }

	// assignment and comparison operators
	Vector4& operator = (const Vector4& vec) { x = vec.x; y = vec.y; z = vec.z; w = vec.w; return *this; }
	bool operator == (const Vector4& vec) const { return x == vec.x && y == vec.y && z == vec.z && w == vec.w; }
	bool operator != (const Vector4& vec) const { return x != vec.x || y != vec.y || z != vec.z || w != vec.w; }

	// arithmetic operators
	Vector4 operator - () const { return Vector4(-x, -y, -z, -w); }
	Vector4 operator + (const Vector4& vec) const { return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	Vector4& operator += (const Vector4& vec) { x += vec.x; y += vec.y; z += vec.z; w += vec.w; return *this; }
	Vector4 operator - (const Vector4& vec) const { return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	Vector4& operator -= (const Vector4& vec) { x -= vec.x; y -= vec.y; z -= vec.z; w -= vec.w; return *this; }
	Vector4 operator * (float scal) const { return Vector4(x * scal, y * scal, z * scal, w * scal); }
	Vector4& operator *= (float scal) { x *= scal; y *= scal; z *= scal; w *= scal; return *this; }
	Vector4 operator / (float scal) const { return Vector4(x / scal, y / scal, z / scal, w / scal); }
	Vector4& operator /= (float scal) { x /= scal; y /= scal; z /= scal; w *= scal; return *this; }

	// manipulation functions
	void zero() { x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f; }
	void norm() {
		float mag = sqrt(x * x + y * y + z * z + w * w);

		if(mag > 0.0f) {
			x /= mag;
			y /= mag;
			z /= mag;
			w /= mag;
		}
	}
};

// non-member vector-related functions
inline Vector4 operator * (float scal, const Vector4& vec) { return Vector4(scal * vec.x, scal * vec.y, scal * vec.z, scal * vec.w); }
inline float dot(Vector4 vec1, Vector4 vec2) { return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w; }
inline Vector4 cross(Vector4& vec1, Vector4& vec2) {
	return Vector4(
			vec1.y * vec2.z - vec1.z * vec2.y,
			vec1.z * vec2.w - vec1.w * vec2.z,
			vec1.w * vec2.x - vec1.x * vec2.w,
			vec1.x * vec2.y - vec1.y * vec2.x
		);
}
inline float distance(Vector4 vec1, Vector4 vec2) {
	float distX = vec1.x - vec2.x;
	float distY = vec1.y - vec2.y;
	float distZ = vec1.z - vec2.z;
	float distW = vec1.w - vec2.w;

	return sqrt(distX * distX + distY * distY + distZ * distZ + distW * distW);
}
inline float mag(Vector4 vec) {
	Vector4 midPoint(0.0f, 0.0f, 0.0f, 0.0f);

	return absolute(distance(midPoint, vec));
}

#endif // VECTORMATH_H
