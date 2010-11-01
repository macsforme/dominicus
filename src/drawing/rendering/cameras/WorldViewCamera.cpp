/*
 *  WorldViewCamera.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "WorldViewCamera.h"

void WorldViewCamera::loop() {
	// get our delta T
	static float lastUpdate = platform.getExecutionTimeMicros();
	float dt = platform.getExecutionTimeMicros() - lastUpdate;
	lastUpdate += dt;

	// do math on variables
	rotation += (float) dt / 1000000.0f * rotationSpeed;
	if(rotation > 360.0f)
		rotation -= 360.0f;
	zoom = 1.0f + zoomFactor *
			cos((float) platform.getExecutionTimeMicros() / 1000000.0f / zoomPeriod) / 2.0f;

	// re-initialize the matrix
	vpMatrix.identity();

	// camera transformations
	scaleMatrix(zoom, zoom, zoom, vpMatrix); // zoom
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(rotation), vpMatrix); // dynamic rotation
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f), vpMatrix); // static tilt up
	translateMatrix(0.0f, 0.0f, 1.15f, vpMatrix); // back away

	// perspective projection
	vpMatrix *= Matrix4(
	        1.0/tan(radians(viewAngle)), 0.0, 0.0, 0.0,
	        0.0, aspectRatio/tan(radians(viewAngle)), 0.0, 0.0,
	        0.0, 0.0, (farClip + nearClip) / (farClip - nearClip), 1.0,
	        0.0, 0.0, -2.0 * farClip * nearClip / (farClip - nearClip), 0.0
		);
}
