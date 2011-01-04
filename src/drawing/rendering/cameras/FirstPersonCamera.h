/*
 *  FirstPersonCamera.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef FIRSTPERSONCAMERA_H
#define FIRSTPERSONCAMERA_H

// program headers
#include "Camera.h"
#include "MatrixMath.h"
#include "Platform.h"
#include "Ship.h"
#include "GameWindow.h"
#include "VectorMath.h"

// definitions
#define FPC_NEARCLIP 1.0f
#define FPC_FARCLIP 150.0f
#define FPC_VIEWANGLE 30.0f

// global variables
extern Platform platform;
extern Ship ship;

class FirstPersonCamera : Camera {
private:
	float aspectRatio;

public:
	Matrix4 vpMatrix;

	FirstPersonCamera(GameWindow* gameWindow) :
			aspectRatio(gameWindow->aspectRatio) { }

	void loop();
};

#endif
