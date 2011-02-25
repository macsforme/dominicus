/*
 *  ShipControl.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef SHIPCONTROL_H
#define SHIPCONTROL_H

// program headers
#include "core/GamePrefs.h"
#include "input/InputHandler.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "state/Ship.h"
#include "windowing/GameWindow.h"

// forward declarations
class Ship;

// global variables
extern GamePrefs* gamePrefs;
extern GameWindow* gameWindow;
extern InputHandler* inputHandler;

class ShipControl {
public:
	class Ship* ship;

	ShipControl(Ship* ship) : ship(ship) { }

	void loop();
};

#endif
