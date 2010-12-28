/*
 *  Mouse.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/5/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef MOUSE_H
#define MOUSE_H

#include "Mouse.h"

// program headers
#include "GamePrefs.h"
#include "Platform.h"

// library headers
#include <iostream>
#include <map>
#include <SDL/SDL.h>

// global variables
extern GamePrefs gamePrefs;
extern Platform platform;

class Mouse {
public:
	unsigned short int positionX, positionY;

	Mouse() : positionX(0), positionY(0) { };

	unsigned long int loop();
};

#endif
