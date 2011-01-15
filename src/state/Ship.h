/*
 *  Ship.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef SHIP_H
#define SHIP_H

// program headers
#include "GamePrefs.h"
#include "MainLoopMember.h"
#include "MatrixMath.h"
#include "Mouse.h"
#include "Platform.h"
#include "VectorMath.h"

// global variables
extern GamePrefs gamePrefs;
extern Mouse mouse;
extern Platform platform;

class Ship : public MainLoopMember {
public:
	Vector4 position;
	Matrix4 orientation;
	float speed;

	Ship(Vector4 position);

	unsigned long int loop();
};

#endif
