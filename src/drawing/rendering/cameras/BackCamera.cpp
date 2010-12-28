/*
 *  WorldViewCamera.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "BackCamera.h"

void BackCamera::loop() {
	// get our delta T
	static float lastUpdate = platform.getExecutionTimeMicros();
	float dt = platform.getExecutionTimeMicros() - lastUpdate;
	lastUpdate += dt;

	// do math on variables
	rotation += (float) dt / 1000000.0f * orthoRotationSpeed;
	if(rotation > 360.0f)
		rotation -= 360.0f;

	// re-initialize the matrix
	vpMatrix.identity();

	// camera transformations
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(180.0f), vpMatrix);
	vpMatrix *= Matrix4(
			ship.orientation.m11,
			ship.orientation.m12,
			ship.orientation.m13,
			0.0f,

			ship.orientation.m21,
			ship.orientation.m22,
			ship.orientation.m23,
			0.0f,

			ship.orientation.m31,
			ship.orientation.m32,
			ship.orientation.m33,
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);

	// un-roll it across the directional vector
//	rotateMatrix(
//			Vector3(ship.orientation.m31, ship.orientation.m32, ship.orientation.m33),
//			-ship.roll,
//			rotationMatrix
//		);

	// reverse the horizon angle
//	rotationMatrix.m12 = -rotationMatrix.m12;
//	rotationMatrix.m13 = -rotationMatrix.m13;

//	rotationMatrix.m21 = -rotationMatrix.m21;
//	rotationMatrix.m23 = -rotationMatrix.m23;

//	rotationMatrix.m31 = -rotationMatrix.m31;
//	rotationMatrix.m32 = -rotationMatrix.m32;

	// roll it again against the z+ vector
//	rotateMatrix(
//			Vector3(ship.orientation.m31, ship.orientation.m32, ship.orientation.m33),
//			ship.roll,
//			vpMatrix
//		);
}
