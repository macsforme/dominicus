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

// definitions
#define SHIP_UPDATESPERSECOND 60
#define SHIP_MOVEPERSEC 3.0f
#define SHIP_TURNPERSEC 90.0f

// program headers
#include "MainLoopMember.h"
#include "MatrixMath.h"
#include "Mouse.h"
#include "Platform.h"
#include "VectorMath.h"

// global variables
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
