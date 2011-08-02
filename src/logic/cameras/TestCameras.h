/*
 *  TestCameras.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/12/11.
 *  Copyright 2011 Joshua Bodine. All rights reserved.
 *
 */

#ifndef TESTCAMERAS_H
#define TESTCAMERAS_H

// program headers
#include "geometry/Mesh.h"
#include "graphics/GameGraphics.h"
#include "logic/cameras/Camera.h"
#include "math/MatrixMath.h"
#include "math/MiscMath.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "state/GameState.h"
#include "state/Ship.h"

// global variables
extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern Platform* platform;

// class definitions
class ShipRelativeCamera : public Camera {
private:
	Ship* ship;

public:
	ShipRelativeCamera(Ship* ship) : ship(ship) { }

	void execute();
};

class TerrainRelativeCamera : public Camera {
private:
	Ship* ship;

public:
	TerrainRelativeCamera(Ship* ship) : ship(ship) { }

	void execute();
};

#endif // TESTCAMERAS_H
