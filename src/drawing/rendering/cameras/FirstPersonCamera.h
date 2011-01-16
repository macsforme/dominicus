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
#include "GamePrefs.h"
#include "GameWindow.h"
#include "VectorMath.h"

// global variables
extern GamePrefs gamePrefs;
extern Platform platform;
extern Ship ship;

extern GameWindow* gameWindow;

class FirstPersonCamera : Camera {
private:
	float aspectRatio;

public:
	Matrix4 vpMatrix;

	FirstPersonCamera() :
			aspectRatio(gameWindow->aspectRatio) { }

	void loop();
};

#endif
