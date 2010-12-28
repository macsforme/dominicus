/*
 *  ShipCamera.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "ShipCamera.h"

void FirstPersonCamera::loop() {
	// re-initialize the matrix
	vpMatrix.identity();

	// SHIP RENDERING VERSION
/*
	// rotate the ship
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(180.0f), vpMatrix);

	// rotate opposite ship's orientation
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
*/
	// TERRAIN RENDERING VERSION

	// translate terrain opposite ship's position
	translateMatrix(-ship.position.x, -ship.position.y, -ship.position.z, vpMatrix);

	// roll it again against the z+ vector
	rotateMatrix(
//			Vector3(0.0f, 0.0f, 1.0f),
			Vector3(ship.orientation.m31, ship.orientation.m32, ship.orientation.m33),
			ship.roll,
			vpMatrix
		);


	Matrix4 rotationMatrix;
	rotationMatrix.identity();

	// rotate opposite ship's orientation
	rotationMatrix *= Matrix4(
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
	rotateMatrix(
//			Vector3(ship.orientation.m31, ship.orientation.m32, ship.orientation.m33),
			Vector3(0.0f, 0.0f, 1.0f),
			-ship.roll,
			rotationMatrix
		);
	
	// reverse the horizon angle
//	rotationMatrix.m12 = -rotationMatrix.m12;
//	rotationMatrix.m13 = -rotationMatrix.m13;

//	rotationMatrix.m21 = -rotationMatrix.m21;
//	rotationMatrix.m23 = -rotationMatrix.m23;

//	rotationMatrix.m31 = -rotationMatrix.m31;
//	rotationMatrix.m32 = -rotationMatrix.m32;

	vpMatrix *= rotationMatrix;

	// perspective projection
	vpMatrix *= Matrix4(
	        1.0/tan(radians(SHIPCAMERA_VIEWANGLE)), 0.0, 0.0, 0.0,
	        0.0, aspectRatio/tan(radians(SHIPCAMERA_VIEWANGLE)), 0.0, 0.0,
	        0.0, 0.0, (SHIPCAMERA_FARCLIP + SHIPCAMERA_NEARCLIP) / (SHIPCAMERA_FARCLIP - SHIPCAMERA_NEARCLIP), 1.0,
	        0.0, 0.0, -2.0 * SHIPCAMERA_FARCLIP * SHIPCAMERA_NEARCLIP / (SHIPCAMERA_FARCLIP - SHIPCAMERA_NEARCLIP), 0.0
		);

}
