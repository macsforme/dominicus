/*
 *  WorldViewCamera.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "OrthoCamera.h"

void OrthoCamera::loop() {
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
	scaleMatrix(0.75, 0.75, 0.75, vpMatrix);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(rotation), vpMatrix); // dynamic rotation
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-orthoBirdsEyeAngle), vpMatrix); // static tilt up
	scaleMatrix(aspectRatio, 1.0f, 1.0f, vpMatrix); // aspect ratio
}
