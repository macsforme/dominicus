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
#include "GameWindow.h"
#include "Platform.h"
#include "VectorMath.h"

// library headers
#include <iostream>
#include <map>
#include <SDL/SDL.h>
#include <vector>

// global variables
extern GamePrefs gamePrefs;
extern GameWindow* gameWindow;
extern Platform platform;

class Mouse {
public:
	Vector2 position;

	std::vector<SDL_Event> eventStack;

	Mouse() : position(Vector2(0.0f, 0.0f)) { };

	void loop();
};

#endif
