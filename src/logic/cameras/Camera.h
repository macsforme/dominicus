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
#include "math/MatrixMath.h"

// class definition
class Camera {
public:
	Vector3 position;
	Matrix4 shipMatrix, terrainMatrix;

	virtual void execute() = 0;
};

#endif
