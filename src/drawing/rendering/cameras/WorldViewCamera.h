/*
 *  WorldViewCamera.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef WORLDVIEWCAMERA_H
#define WORLDVIEWCAMERA_H

// program headers
#include "Camera.h"
#include "MatrixMath.h"
#include "Platform.h"
#include "GameWindow.h"
#include "VectorMath.h"

// constants
const float birdsEyeAngle = 30.0f; // degrees from horizon
const float rotationSpeed = 30.0f; // degrees per second
const float zoomFactor = 0.25f;	// total zoom distance
const float zoomPeriod = 1.0f;	// time in seconds to complete a zoom cycle

#define WVC_NEARCLIP 1.0f
#define WVC_FARCLIP 1500.0f
#define WVC_VIEWANGLE 30.0f

// global variables
extern Platform platform;

class WorldViewCamera : Camera {
private:
	float rotation;
	float zoom;

	float aspectRatio;

public:
	Matrix4 vpMatrix;

	WorldViewCamera(GameWindow* gameWindow) :
			aspectRatio(gameWindow->aspectRatio), rotation(0.0f), zoom(0.0f) { }

	void loop();
};

#endif
