/*
 * FollowCamera.cpp
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

#include "logic/cameras/FollowCamera.h"

FollowCamera::FollowCamera(Ship* ship) : ship(ship) { }

void FollowCamera::execute() {
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

	// ship speed fraction
//	float speedFraction = ship->speed / gameSystem->getFloat("shipMaxSpeed");

	// ship matrix
	shipMatrix.identity();

	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(180.0f), shipMatrix);

	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(ship->pitch), shipMatrix);
	rotateMatrix(Vector3(0.0f, 0.0f, 1.0f), -radians(ship->roll), shipMatrix);

	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f),
			radians(-gameSystem->getFloat("renderingCameraAngle")), shipMatrix);
	translateMatrix(0.0f, 0.0f, gameSystem->getFloat("renderingCameraFollowDistance"), shipMatrix);

	shipMatrix *= gameGraphics->ppMatrix;

	// terrain matrix
	terrainMatrix.identity();

	translateMatrix(-ship->position.x, -ship->position.y, -ship->position.z, terrainMatrix);

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

	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f),
			radians(-gameSystem->getFloat("renderingCameraAngle")), terrainMatrix);
	translateMatrix(0.0f, 0.0f, (float) gameSystem->getFloat("renderingCameraFollowDistance"), terrainMatrix);

	terrainMatrix *= gameGraphics->ppMatrix;
}
