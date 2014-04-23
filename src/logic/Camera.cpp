// Camera.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "logic/Camera.h"

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

void FollowCamera::execute() {
/*
	// update camera position
	float xzAngle = getAngle(Vector2(ship->direction.x, ship->direction.z)) - 90.0f;

	Matrix3 positionMatrix;
	positionMatrix.identity();
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(-xzAngle), positionMatrix);
	rotateMatrix(
			Vector3(positionMatrix.m11, positionMatrix.m12, positionMatrix.m13),
			radians(gameSystem->getFloat("renderingCameraAngle")),
			positionMatrix
		);
	Vector3 newPosition(0.0f, 0.0f, -gameSystem->getFloat("renderingCameraFollowDistance"));
	position = newPosition * positionMatrix + ship->position;

	// generate the actual ship orientation matrix from the direction
	Matrix3 shipOrientation = completeMatrix(ship->direction);
	shipOrientation = Matrix3(
			-shipOrientation.m31, shipOrientation.m32, -shipOrientation.m33,
			shipOrientation.m21, shipOrientation.m22, shipOrientation.m23,
			shipOrientation.m11, shipOrientation.m12, shipOrientation.m13
		);
*/
	// ship speed fraction
//	float speedFraction = ship->speed / gameSystem->getFloat("shipMaxSpeed");

	// ship matrix
	shipMatrix.identity();

	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(180.0f), shipMatrix);
/*
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(ship->pitch), shipMatrix);
	rotateMatrix(Vector3(0.0f, 0.0f, 1.0f), -radians(ship->roll), shipMatrix);
*/
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f),
			radians(-gameSystem->getFloat("renderingCameraAngle")), shipMatrix);
	translateMatrix(0.0f, 0.0f, gameSystem->getFloat("renderingCameraFollowDistance"), shipMatrix);

	shipMatrix *= gameGraphics->ppMatrix;

	// terrain matrix
	terrainMatrix.identity();

	translateMatrix(-ship->position.x, -ship->position.y, -ship->position.z, terrainMatrix);
/*
	Vector3 zVec(shipOrientation.m31, 0.0f, shipOrientation.m33);
	zVec.norm();

	terrainMatrix *= Matrix4(
			zVec.z,
			0.0f,
			zVec.x,
			0.0f,

			0.0f,
			1.0f,
			0.0f,
			0.0f,

			-zVec.x,
			0.0f,
			zVec.z,
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);
*/
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f),
			radians(-gameSystem->getFloat("renderingCameraAngle")), terrainMatrix);
	translateMatrix(0.0f, 0.0f, (float) gameSystem->getFloat("renderingCameraFollowDistance"), terrainMatrix);

	terrainMatrix *= gameGraphics->ppMatrix;
}

void TestCamera::execute() {
	if(gameState == NULL)
		return;

	// overhead view
	terrainMatrix.identity();
//	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 32000) / 32000.0f * 360.0f), terrainMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-90.0f), terrainMatrix);
	translateMatrix(0.0f, 0.0f, gameSystem->getFloat("islandMaximumWidth") * 3.0f, terrainMatrix);

	towerMatrix.identity();
	translateMatrix(gameState->fortress.position.x, gameState->fortress.position.y, gameState->fortress.position.z, towerMatrix);
//	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 32000) / 32000.0f * 360.0f), towerMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-90.0f), towerMatrix);
	translateMatrix(0.0f, 0.0f, gameSystem->getFloat("islandMaximumWidth") * 3.0f, towerMatrix);

	shipMatrix.identity();
//	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(90.0f), shipMatrix);
//	translateMatrix(gameSystem->getFloat("islandMaximumWidth") * 0.5f, 0.0f, 0.0f, shipMatrix);
//	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 16000) / 16000.0f * 360.0f), shipMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-90.0f), shipMatrix);
	translateMatrix(0.0f, 0.0f, gameSystem->getFloat("islandMaximumWidth") * 3.0f, shipMatrix);

return;

	// overall tilted view
	terrainMatrix.identity();
//	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 32000) / 32000.0f * 360.0f), terrainMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f), terrainMatrix);
	translateMatrix(0.0f, 0.0f, gameSystem->getFloat("islandMaximumWidth"), terrainMatrix);

	towerMatrix.identity();
	translateMatrix(gameState->fortress.position.x, gameState->fortress.position.y, gameState->fortress.position.z, towerMatrix);
//	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 32000) / 32000.0f * 360.0f), towerMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f), towerMatrix);
	translateMatrix(0.0f, 0.0f, gameSystem->getFloat("islandMaximumWidth"), towerMatrix);

	shipMatrix.identity();
//	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(90.0f), shipMatrix);
//	translateMatrix(gameSystem->getFloat("islandMaximumWidth") * 0.5f, 0.0f, 0.0f, shipMatrix);
//	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 16000) / 16000.0f * 360.0f), shipMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f), shipMatrix);
	translateMatrix(0.0f, 0.0f, gameSystem->getFloat("islandMaximumWidth"), shipMatrix);

return;

	// zoomed in pulsing tower view
	terrainMatrix.identity();
	translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, terrainMatrix);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 16000) / 16000.0f * 360.0f), terrainMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f * (sin(PI + (float) (platform->getExecMills() % 16000) / 16000.0f * 2.0f * PI) * 0.5f + 0.5f )), terrainMatrix);
	translateMatrix(0.0f, 0.0f, 25.0f + (sin(-PI + (float) (platform->getExecMills() % 16000) / 16000.0f * 2.0f * PI) * 0.5f + 0.5f ) * 200.0f, terrainMatrix);

	towerMatrix.identity();
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 16000) / 16000.0f * 360.0f), towerMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f * (sin(PI + (float) (platform->getExecMills() % 16000) / 16000.0f * 2.0f * PI) * 0.5f + 0.5f )), towerMatrix);
	translateMatrix(0.0f, 0.0f, 25.0f + (sin(-PI + (float) (platform->getExecMills() % 16000) / 16000.0f * 2.0f * PI) * 0.5f + 0.5f ) * 200.0f, towerMatrix);

	shipMatrix.identity();
	translateMatrix(0.0f, -gameState->fortress.position.y, 0.0f, shipMatrix);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(90.0f), shipMatrix);
	translateMatrix(gameSystem->getFloat("islandMaximumWidth"), 0.0f, 0.0f, shipMatrix);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 8000) / 8000.0f * 360.0f), shipMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f * (sin(PI + (float) (platform->getExecMills() % 16000) / 16000.0f * 2.0f * PI) * 0.5f + 0.5f )), shipMatrix);
	translateMatrix(0.0f, 0.0f, 25.0f + (sin(-PI + (float) (platform->getExecMills() % 16000) / 16000.0f * 2.0f * PI) * 0.5f + 0.5f ) * 200.0f, shipMatrix);
}
