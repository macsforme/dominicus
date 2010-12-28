/*
 *  BackCamera.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef BACKCAMERA_H
#define BACKCAMERA_H

// program headers
#include "Camera.h"
#include "GameWindow.h"
#include "MatrixMath.h"
#include "Platform.h"
#include "Ship.h"
#include "VectorMath.h"

// constants
const float orthoBirdsEyeAngle = 45.0f; // degrees from horizon
const float orthoRotationSpeed = 30.0f; // degrees per second

// global variables
extern Platform platform;
extern Ship ship;

class BackCamera : Camera {
private:
	float rotation;

	float aspectRatio;

public:
	Matrix4 vpMatrix;

	BackCamera(GameWindow* gameWindow) :
			aspectRatio(gameWindow->aspectRatio), rotation(0.0f) { }

	void loop();
};

#endif
