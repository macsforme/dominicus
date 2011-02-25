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
#include "core/GamePrefs.h"
#include "drawing/rendering/cameras/Camera.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "state/Ship.h"
#include "windowing/GameWindow.h"

// global variables
extern GamePrefs* gamePrefs;
extern GameWindow* gameWindow;
extern Platform* platform;
extern Ship* ship;

class BackCamera : Camera {
private:
	float rotation;
	float aspectRatio;

	Matrix4 cameraOrientation;

public:
	Matrix4 terrainVMatrix;
	Matrix4 shipVMatrix;

	Matrix4 pMatrix;

	BackCamera();

	void loop();
};

#endif
