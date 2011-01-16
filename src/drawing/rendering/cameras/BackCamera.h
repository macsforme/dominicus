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
#define BC_BACKOFFDIST 5.0f
#define BC_WATCHANGLE -20.0f

#define BC_NEARCLIP 1.0f
#define BC_FARCLIP 1500.0f
#define BC_VIEWANGLE 30.0f

// global variables
extern Platform platform;
extern Ship ship;

extern GameWindow* gameWindow;

class BackCamera : Camera {
private:
	float rotation;

	float aspectRatio;

public:
	Matrix4 terrainVPMatrix;
	Matrix4 shipVPMatrix;

	BackCamera() : rotation(0.0f), aspectRatio(gameWindow->aspectRatio) { }

	void loop();
};

#endif
