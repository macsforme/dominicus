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
#include "GamePrefs.h"
#include "GameWindow.h"
#include "MatrixMath.h"
#include "Platform.h"
#include "Ship.h"
#include "VectorMath.h"

// global variables
extern GamePrefs gamePrefs;
extern Platform platform;
extern Ship ship;

extern GameWindow* gameWindow;

class BackCamera : Camera {
private:
	float rotation;
	float aspectRatio;

	Matrix4 cameraOrientation;

public:
	Matrix4 terrainVPMatrix;
	Matrix4 shipVPMatrix;

	BackCamera();

	void loop();
};

#endif
