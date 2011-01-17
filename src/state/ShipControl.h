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
#include "GamePrefs.h"
#include "GameWindow.h"
#include "InputHandler.h"
#include "ScalarMath.h"
#include "Ship.h"
#include "VectorMath.h"

// global variables
extern GamePrefs gamePrefs;
extern InputHandler* inputHandler;

extern GameWindow* gameWindow;

class ShipControl {
public:
	Ship* ship;

	ShipControl(Ship* ship) : ship(ship) { }

	void loop();
};

#endif
