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
#include "core/GamePrefs.h"
#include "core/MainLoopMember.h"
#include "input/Mouse.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"

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
