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
#include "math/MatrixMath.h"
#include "math/MiscMath.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "state/GameState.h"

// global variables
extern GameState* gameState;
extern GameSystem* gameSystem;

// class definitions
class Camera {
public:
	Vector3 position;
	Matrix4 mvMatrix, lightMatrix;

	virtual void execute() = 0;
};

class OrbitCamera : public Camera {
public:
	void execute();
};

class PresentationCamera : public Camera {
public:
	void execute();
};

class RoamingCamera : public Camera {
public:
	Vector3 position;
	float rotationX, rotationY;

	RoamingCamera() : position(Vector3(0.0f, 5.0f, -2000.0f)), rotationX(0.0f), rotationY(0.0f) {};

	void execute();
};

class TowerCamera: public Camera {
public:
	void execute();
};

#endif
