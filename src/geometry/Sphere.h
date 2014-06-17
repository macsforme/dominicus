// Sphere.h
// Crucible Island

#ifndef SPHERE_H
#define SPHERE_H

// program headers
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "geometry/Mesh.h"

inline Mesh makeSphere(size_t sections) {
	Mesh sphere;

	if(sections < 4)
		return sphere;

	// top
	sphere.addVertex(Vector3(0.0f, 1.0f, 0.0f));

	Matrix3 latitudeMatrix; latitudeMatrix.identity();
	rotateMatrix(Vector3(0.0f, 0.0f, -1.0f), radians(360.0f / (float) sections), latitudeMatrix);

	for(size_t i = 0; i < sections; ++i) {
		Matrix3 longitudeMatrix = latitudeMatrix;
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) i * 360.0f / (float) sections), longitudeMatrix);

		sphere.addVertex(Vector3(0.0f, 1.0f, 0.0f) * longitudeMatrix);
		if(i > 0)
			sphere.addFace(0, i, i + 1);
	}

	sphere.addFace(sections, 1, 0);

	// middle
	for(size_t i = 0; i < sections / 2 - 2; ++i) {
		latitudeMatrix.identity();
		rotateMatrix(Vector3(0.0f, 0.0f, -1.0f), radians((i + 2) * 360.0f / (float) sections), latitudeMatrix);

		for(size_t p = 0; p < sections; ++p) {
			Matrix3 longitudeMatrix = latitudeMatrix;
			rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) p * 360.0f / (float) sections), longitudeMatrix);

			sphere.addVertex(Vector3(0.0f, 1.0f, 0.0f) * longitudeMatrix);
			if(p > 0) {
				sphere.addFace(
						1 + i * sections + p - 1,
						1 + (i + 1) * sections + p - 1,
						1 + (i + 1) * sections + p
					);
				sphere.addFace(
						1 + (i + 1) * sections + p,
						1 + i * sections + p,
						1 + i * sections + p - 1
					);
			}
		}

		sphere.addFace(
				1 + i * sections + sections - 1,
				1 + (i + 1) * sections + sections - 1,
				1 + (i + 1) * sections
			);
		sphere.addFace(
				1 + (i + 1) * sections,
				1 + i * sections,
				1 + i * sections + sections - 1
			);
	}

	// bottom
	sphere.addVertex(Vector3(0.0f, -1.0f, 0.0f));

	for(size_t i = 0; i < sections; ++i)
		sphere.addFace(
				1 + (sections / 2 - 2 + 1) * sections,
				1 + (sections / 2 - 2 + 1) * sections - sections + (i < sections - 1 ? i + 1 : 0),
				1 + (sections / 2 - 2 + 1) * sections - sections + i
			);

	sphere.autoNormal();

	return sphere;
}

#endif // SPHERE_H
