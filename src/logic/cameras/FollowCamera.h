// FollowCamera.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef FOLLOWCAMERA_H
#define FOLLOWCAMERA_H

// forward declarations
class Ship;

// program headers
#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "logic/cameras/Camera.h"
#include "math/MatrixMath.h"
#include "math/MiscMath.h"
#include "math/VectorMath.h"
#include "state/Ship.h"

// global variables
extern GameGraphics* gameGraphics;
extern GameSystem* gameSystem;

// class definition
class FollowCamera : public Camera {
private:
	Ship* ship;

public:
	FollowCamera(Ship* ship);

	void execute();
};

#endif // FOLLOWCAMERA_H
