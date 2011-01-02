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
#include "Ship.h"

extern GamePrefs gamePrefs;
extern InputHandler inputHandler;

class ShipControl {
public:
	GameWindow* gameWindow;

	Ship* ship;

	ShipControl(GameWindow* gameWindow, Ship* ship);

	void loop();
};

#endif
