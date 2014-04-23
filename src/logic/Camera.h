// Camera.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef CAMERA_H
#define CAMERA_H

// program headers
#include "core/GameSystem.h"
#include "geometry/Mesh.h"
#include "graphics/GameGraphics.h"
#include "math/MatrixMath.h"
#include "math/MiscMath.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "state/GameState.h"

// global variables
extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;

// class definitions
class Camera {
public:
	Vector3 position;
	Matrix4 shipMatrix, terrainMatrix, towerMatrix;

	virtual void execute() = 0;
};

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

class FollowCamera : public Camera {
private:
	Ship* ship;

public:
	FollowCamera(Ship* ship);

	void execute();
};

class TestCamera : public Camera {
public:
	TestCamera() { };

	void execute();
};

#endif
