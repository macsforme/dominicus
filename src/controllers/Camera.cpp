/*
 *  Camera.cpp
 *  ModelViewer
 *
 *  Created by Joshua Bodine on 8/23/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "Camera.h"

Camera::Camera() {
	// camera setup
	currentRotate.identity();
	currentScale.identity();

	zoomInKey = false;
	zoomOutKey = false;
	angleXPlusKey = false;
	angleXMinusKey = false;
	angleYPlusKey = false;
	angleYMinusKey = false;
}

void Camera::update() {
	if(zoomInKey) {
		scaleMatrix(
				1.0f + ZOOM_FACTOR / (float) LOOP_FREQUENCY,
				1.0f + ZOOM_FACTOR / (float) LOOP_FREQUENCY,
				1.0f + ZOOM_FACTOR / (float) LOOP_FREQUENCY,
				currentScale
			);
	}
	if(zoomOutKey) {
		scaleMatrix(
				1.0f - ZOOM_FACTOR / (float) LOOP_FREQUENCY,
				1.0f - ZOOM_FACTOR / (float) LOOP_FREQUENCY,
				1.0f - ZOOM_FACTOR / (float) LOOP_FREQUENCY,
				currentScale
			);
	}

	if(angleXPlusKey) {
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), ANGLE_FACTOR / (float) LOOP_FREQUENCY, currentRotate);
	}
	if(angleXMinusKey) {
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), -ANGLE_FACTOR / (float) LOOP_FREQUENCY, currentRotate);
	}
	if(angleYPlusKey) {
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), ANGLE_FACTOR / (float) LOOP_FREQUENCY, currentRotate);
	}
	if(angleYMinusKey) {
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), -ANGLE_FACTOR / (float) LOOP_FREQUENCY, currentRotate);
	}
	if(angleZPlusKey) {
		rotateMatrix(Vector3(0.0f, 0.0f, 1.0f), ANGLE_FACTOR / (float) LOOP_FREQUENCY, currentRotate);
	}
	if(angleZMinusKey) {
		rotateMatrix(Vector3(0.0f, 0.0f, 1.0f), -ANGLE_FACTOR / (float) LOOP_FREQUENCY, currentRotate);
	}
}


