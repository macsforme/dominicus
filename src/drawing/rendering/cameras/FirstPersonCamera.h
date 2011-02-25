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
