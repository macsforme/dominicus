/*
 *  WorldViewCamera.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "drawing/rendering/cameras/WorldViewCamera.h"

void WorldViewCamera::loop() {
	// get our delta T
	static float lastUpdate = (float)platform.getExecMills();
	float dt = (float)platform.getExecMills() - lastUpdate;
	lastUpdate += dt;

	// do math on variables
	rotation += (float) dt / 1000.0f * rotationSpeed;
	if(rotation > 360.0f)
		rotation -= 360.0f;
	zoom = 1.0f + zoomFactor *
			cos((float) (platform.getExecMills() % 1000) / 1000.0f / zoomPeriod) / 2.0f;

	// re-initialize the matrix
	vpMatrix.identity();

	// do transformations
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(rotation), vpMatrix); // dynamic rotation
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-birdsEyeAngle), vpMatrix); // static tilt up
	translateMatrix(0.0f, 0.0f, 750.0f, vpMatrix); // z translation
	scaleMatrix(zoom, zoom, zoom, vpMatrix); // dynamic zoom

	// perspective projection
	const float fov = gamePrefs.getFloat("perspectiveFOV");
	const float nClip = gamePrefs.getFloat("perspectiveNearClip");
	const float fClip = gamePrefs.getFloat("perspectiveFarClip");

	vpMatrix *= Matrix4(
	        1.0f/tan(radians(fov)), 0.0, 0.0, 0.0,
	        0.0, aspectRatio/tan(radians(fov)), 0.0, 0.0,
	        0.0, 0.0, (fClip + nClip) / (fClip - nClip), 1.0f,
	        0.0, 0.0, -2.0f * fClip * nClip / (fClip - nClip), 0.0
		);
}
