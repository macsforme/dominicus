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
#include "core/GamePrefs.h"
#include "drawing/rendering/cameras/Camera.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "windowing/GameWindow.h"

// constants
const float birdsEyeAngle = 30.0f; // degrees from horizon
const float rotationSpeed = 30.0f; // degrees per second
const float zoomFactor = 0.25f;	// total zoom distance
const float zoomPeriod = 1.0f;	// time in seconds to complete a zoom cycle

// global variables
extern GamePrefs gamePrefs;
extern Platform platform;

extern GameWindow* gameWindow;

class WorldViewCamera : Camera {
private:
	float rotation;
	float zoom;

	float aspectRatio;

public:
	Matrix4 vpMatrix;

	WorldViewCamera() :
			rotation(0.0f), zoom(0.0f), aspectRatio(gameWindow->aspectRatio) { }

	void loop();
};

#endif
