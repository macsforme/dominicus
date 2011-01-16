/*
 *  ShipCamera.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "FirstPersonCamera.h"

void FirstPersonCamera::loop() {
	// re-initialize the matrix
	vpMatrix.identity();

	// translate terrain opposite ship's position
	translateMatrix(-ship.position.x, -ship.position.y, -ship.position.z, vpMatrix);

	// transpose the ship's orientation
	vpMatrix *= Matrix4(
			ship.orientation.m11,
			ship.orientation.m21,
			ship.orientation.m31,
			0.0f,

			ship.orientation.m12,
			ship.orientation.m22,
			ship.orientation.m32,
			0.0f,

			ship.orientation.m13,
			ship.orientation.m23,
			ship.orientation.m33,
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);

	// perspective projection
	const float fov = gamePrefs.getFloat("perspectiveFOV");
	const float nClip = gamePrefs.getFloat("perspectiveNearClip");
	const float fClip = gamePrefs.getFloat("perspectiveFarClip");

	vpMatrix *= Matrix4(
	        1.0/tan(radians(fov)), 0.0, 0.0, 0.0,
	        0.0, aspectRatio/tan(radians(fov)), 0.0, 0.0,
	        0.0, 0.0, (fClip + nClip) / (fClip - nClip), 1.0,
	        0.0, 0.0, -2.0 * fClip * nClip / (fClip - nClip), 0.0
		);
}
