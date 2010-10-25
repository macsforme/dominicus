/*
 *  Camera.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 8/23/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef CAMERA_H
#define CAMERA_H

// program headers
#include "MatrixMath.h"
#include "Platform.h"
#include "Preferences.h"

// constants
#define ANGLE_FACTOR 1.0f
#define ZOOM_FACTOR 1.0f

class Camera {
public:
	Matrix4 currentRotate;
	Matrix4 currentScale;

	Camera();

	bool zoomInKey, zoomOutKey;
	bool angleXPlusKey, angleXMinusKey;
	bool angleYPlusKey, angleYMinusKey;
	bool angleZPlusKey, angleZMinusKey;

	void update();
};

#endif
