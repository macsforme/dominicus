/*
 *  TestCameras.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/12/11.
 *  Copyright 2011 Joshua Bodine. All rights reserved.
 *
 */

#include "logic/cameras/TestCameras.h"

void ShipRelativeCamera::execute() {
/*
	// ship
	shipMatrix.identity();

	shipMatrix *= ship->orientation;

	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f), shipMatrix);

	translateMatrix(0.75f, 0.0f, 3.0f, shipMatrix);

	shipMatrix *= gameGraphics->ppMatrix;
shipMatrix.zero();
	// terrain
	terrainMatrix.identity();

	terrainMatrix *= ship->orientation;

//	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(90.0f), terrainMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f), terrainMatrix);

//	translateMatrix(0.0f, 0.0f, 3.0f, terrainMatrix);

	terrainMatrix *= gameGraphics->opMatrix;
//terrainMatrix.zero();
*/
}

void TerrainRelativeCamera::execute() {
	// ship
	shipMatrix.zero();

	// terrain
	terrainMatrix.identity();
//scaleMatrix(0.2f, 0.2f, 0.2f, terrainMatrix);
//	Matrix3 orientation3 = completeMatrix(ship->direction);
//	Matrix4 orientation(
//			orientation3.m11, orientation3.m12, orientation3.m13, 0.0f,
//			orientation3.m21, orientation3.m22, orientation3.m23, 0.0f,
//			orientation3.m31, orientation3.m32, orientation3.m33, 0.0f,
//			0.0f, 0.0f, 0.0f, 1.0f
//		);
//	terrainMatrix *= orientation;
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 12000) / 12000.0f * 360.0f),
			terrainMatrix);
rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-20.0f), terrainMatrix);

translateMatrix(0.0f, 0.0f, 1000.0f, terrainMatrix);
	terrainMatrix *= gameGraphics->ppMatrix;
//shipMatrix = terrainMatrix;
}
