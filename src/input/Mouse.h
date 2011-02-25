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

// program headers
#include "core/GamePrefs.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "windowing/GameWindow.h"

// library headers
#include <iostream>
#include <map>
#include <SDL/SDL.h>
#include <vector>

// global variables
extern GamePrefs* gamePrefs;
extern GameWindow* gameWindow;
extern Platform* platform;
extern ProgramLog* programLog;

class Mouse {
public:
	Vector2 position;

	std::vector<SDL_Event> eventStack;

	Mouse() : position(Vector2(0.0f, 0.0f)) { };

	void loop();
};

#endif
