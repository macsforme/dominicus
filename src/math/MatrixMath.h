/*
 *  MatrixMath.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 8/23/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef MATRIXMATH_H
#define MATRIXMATH_H

// program headers
#include "VectorMath.h"

// library headers
#include <math.h>

class Matrix4 {
public:
	// public data
	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;

	// constructors
	Matrix4() { }
	Matrix4(const Matrix4& mat) :
			m11(mat.m11), m12(mat.m12), m13(mat.m13), m14(mat.m14),
			m21(mat.m21), m22(mat.m22), m23(mat.m23), m24(mat.m24),
			m31(mat.m31), m32(mat.m32), m33(mat.m33), m34(mat.m34),
			m41(mat.m41), m42(mat.m42), m43(mat.m43), m44(mat.m44) { }
	Matrix4(
			float n11, float n12, float n13, float n14,
			float n21, float n22, float n23, float n24,
			float n31, float n32, float n33, float n34,
			float n41, float n42, float n43, float n44
		) :
			m11(n11), m12(n12), m13(n13), m14(n14),
			m21(n21), m22(n22), m23(n23), m24(n24),
			m31(n31), m32(n32), m33(n33), m34(n34),
			m41(n41), m42(n42), m43(n43), m44(n44) { }

	// assignment and comparison operators
	Matrix4& operator = (const Matrix4& mat) {
			m11 = mat.m11; m12 = mat.m12; m13 = mat.m13; m14 = mat.m14;
			m21 = mat.m21; m22 = mat.m22; m23 = mat.m23; m24 = mat.m24;
			m31 = mat.m31; m32 = mat.m32; m33 = mat.m33; m34 = mat.m34;
			m41 = mat.m41; m42 = mat.m42; m43 = mat.m43; m44 = mat.m44;

			return *this;
	}
	bool operator == (const Matrix4& mat) const {
		return
			m11 == mat.m11 && m12 == mat.m12 && m13 == mat.m13 && m14 == mat.m14 &&
			m21 == mat.m21 && m22 == mat.m22 && m23 == mat.m23 && m24 == mat.m24 &&
			m31 == mat.m31 && m32 == mat.m32 && m33 == mat.m33 && m34 == mat.m34 &&
			m41 == mat.m41 && m42 == mat.m42 && m43 == mat.m43 && m44 == mat.m44;
	}
	bool operator != (const Matrix4& mat) const {
		return
			m11 != mat.m11 || m12 != mat.m12 || m13 != mat.m13 || m14 != mat.m14 ||
			m21 != mat.m21 || m22 != mat.m22 || m23 != mat.m23 || m24 != mat.m24 ||
			m31 != mat.m31 || m32 != mat.m32 || m33 != mat.m33 || m34 != mat.m34 ||
			m41 != mat.m41 || m42 != mat.m42 || m43 != mat.m43 || m44 != mat.m44;
	}

	// arithmetic operators
	Matrix4 operator - () const {
		return Matrix4(
				-m11, -m12, -m13, -m14,
				-m21, -m22, -m23, -m24,
				-m31, -m32, -m33, -m34,
				-m41, -m42, -m43, -m44
			);
	}
	Matrix4 operator * (float scal) const {
		return Matrix4(
				m11 * scal, m12 * scal, m13 * scal, m14 * scal,
				m21 * scal, m22 * scal, m23 * scal, m24 * scal,
				m31 * scal, m32 * scal, m33 * scal, m34 * scal,
				m41 * scal, m42 * scal, m43 * scal, m44 * scal
			);
	}
	Matrix4& operator *= (float scal) {
		m11 *= scal; m12 *= scal; m13 *= scal; m14 *= scal;
		m21 *= scal; m22 *= scal; m23 *= scal; m24 *= scal;
		m31 *= scal; m32 *= scal; m33 *= scal; m34 *= scal;
		m41 *= scal; m42 *= scal; m43 *= scal; m44 *= scal;

		return *this;
	}
	Matrix4 operator * (const Matrix4& mat) const {
		return Matrix4(
				m11 * mat.m11 + m12 * mat.m21 + m13 * mat.m31 + m14 * mat.m41,
				m11 * mat.m12 + m12 * mat.m22 + m13 * mat.m32 + m14 * mat.m42,
				m11 * mat.m13 + m12 * mat.m23 + m13 * mat.m33 + m14 * mat.m43,
				m11 * mat.m14 + m12 * mat.m24 + m13 * mat.m34 + m14 * mat.m44,

				m21 * mat.m11 + m22 * mat.m21 + m23 * mat.m31 + m24 * mat.m41,
				m21 * mat.m12 + m22 * mat.m22 + m23 * mat.m32 + m24 * mat.m42,
				m21 * mat.m13 + m22 * mat.m23 + m23 * mat.m33 + m24 * mat.m43,
				m21 * mat.m14 + m22 * mat.m24 + m23 * mat.m34 + m24 * mat.m44,

				m31 * mat.m11 + m32 * mat.m21 + m33 * mat.m31 + m34 * mat.m41,
				m31 * mat.m12 + m32 * mat.m22 + m33 * mat.m32 + m34 * mat.m42,
				m31 * mat.m13 + m32 * mat.m23 + m33 * mat.m33 + m34 * mat.m43,
				m31 * mat.m14 + m32 * mat.m24 + m33 * mat.m34 + m34 * mat.m44,

				m41 * mat.m11 + m42 * mat.m21 + m43 * mat.m31 + m44 * mat.m41,
				m41 * mat.m12 + m42 * mat.m22 + m43 * mat.m32 + m44 * mat.m42,
				m41 * mat.m13 + m42 * mat.m23 + m43 * mat.m33 + m44 * mat.m43,
				m41 * mat.m14 + m42 * mat.m24 + m43 * mat.m34 + m44 * mat.m44
			);
	}

	Matrix4& operator *= (const Matrix4& mat) {
		Matrix4 old(*this);

		m11 = old.m11 * mat.m11 + old.m12 * mat.m21 + old.m13 * mat.m31 + old.m14 * mat.m41;
		m12 = old.m11 * mat.m12 + old.m12 * mat.m22 + old.m13 * mat.m32 + old.m14 * mat.m42;
		m13 = old.m11 * mat.m13 + old.m12 * mat.m23 + old.m13 * mat.m33 + old.m14 * mat.m43;
		m14 = old.m11 * mat.m14 + old.m12 * mat.m24 + old.m13 * mat.m34 + old.m14 * mat.m44;

		m21 = old.m21 * mat.m11 + old.m22 * mat.m21 + old.m23 * mat.m31 + old.m24 * mat.m41;
		m22 = old.m21 * mat.m12 + old.m22 * mat.m22 + old.m23 * mat.m32 + old.m24 * mat.m42;
		m23 = old.m21 * mat.m13 + old.m22 * mat.m23 + old.m23 * mat.m33 + old.m24 * mat.m43;
		m24 = old.m21 * mat.m14 + old.m22 * mat.m24 + old.m23 * mat.m34 + old.m24 * mat.m44;

		m31 = old.m31 * mat.m11 + old.m32 * mat.m21 + old.m33 * mat.m31 + old.m34 * mat.m41;
		m32 = old.m31 * mat.m12 + old.m32 * mat.m22 + old.m33 * mat.m32 + old.m34 * mat.m42;
		m33 = old.m31 * mat.m13 + old.m32 * mat.m23 + old.m33 * mat.m33 + old.m34 * mat.m43;
		m34 = old.m31 * mat.m14 + old.m32 * mat.m24 + old.m33 * mat.m34 + old.m34 * mat.m44;

		m41 = old.m41 * mat.m11 + old.m42 * mat.m21 + old.m43 * mat.m31 + old.m44 * mat.m41;
		m42 = old.m41 * mat.m12 + old.m42 * mat.m22 + old.m43 * mat.m32 + old.m44 * mat.m42;
		m43 = old.m41 * mat.m13 + old.m42 * mat.m23 + old.m43 * mat.m33 + old.m44 * mat.m43;
		m44 = old.m41 * mat.m14 + old.m42 * mat.m24 + old.m43 * mat.m34 + old.m44 * mat.m44;

		return *this;
	}

	// matrix operation functions
	Matrix4 transpose() const {
		return Matrix4(
				m11, m21, m31, m41,
				m12, m22, m32, m42,
				m13, m23, m33, m43,
				m14, m24, m34, m44
			);
	}

	// manipulation functions
	void identity() {
		m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
		m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = 0.0f;
		m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = 0.0f;
		m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
	}
	void zero() {
		m11 = 0.0f; m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
		m21 = 0.0f; m22 = 0.0f; m23 = 0.0f; m24 = 0.0f;
		m31 = 0.0f; m32 = 0.0f; m33 = 0.0f; m34 = 0.0f;
		m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 0.0f;
	}
};

// non-member matrix-related arithmetic functions
inline Vector4 operator * (const Vector4 vec, const Matrix4& mat) {
	return Vector4(
			vec.x * mat.m11 + vec.y * mat.m21 + vec.z * mat.m31 + vec.w * mat.m41,
			vec.x * mat.m21 + vec.y * mat.m22 + vec.z * mat.m32 + vec.w * mat.m42,
			vec.x * mat.m31 + vec.y * mat.m23 + vec.z * mat.m33 + vec.w * mat.m43,
			vec.x * mat.m41 + vec.y * mat.m24 + vec.z * mat.m34 + vec.w * mat.m44
		);
}

// non-member matrix manipulation functions
inline void scaleMatrix(float x, float y, float z, Matrix4& mat) {
	mat *= Matrix4(
			x, 0.0f, 0.0f, 0.0f,
			0.0f, y, 0.0f, 0.0f,
			0.0f, 0.0f, z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
}
inline void rotateMatrix(const Vector3 axis, float angle, Matrix4& mat) {
	mat *= Matrix4(
			axis.x * axis.x * (1.0f - cos(angle)) + cos(angle),
			axis.x * axis.y * (1.0f - cos(angle)) + axis.z * sin(angle),
			axis.x * axis.z * (1.0f - cos(angle)) - axis.y * sin(angle),
			0.0f,

			axis.x * axis.y * (1.0f - cos(angle)) - axis.z * sin(angle),
			axis.y * axis.y * (1.0f - cos(angle)) + cos(angle),
			axis.y * axis.z * (1.0f - cos(angle)) + axis.x * sin(angle),
			0.0f,

			axis.x * axis.z * (1.0f - cos(angle)) + axis.y * sin(angle),
			axis.y * axis.z * (1.0f - cos(angle)) - axis.x * sin(angle),
			axis.x * axis.x * (1.0f - cos(angle)) + cos(angle),
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);
}
inline void translateMatrix(float x, float y, float z, Matrix4& mat) {
	mat *= Matrix4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x, y, z, 1.0f
		);
}

#endif
