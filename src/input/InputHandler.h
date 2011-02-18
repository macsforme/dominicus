/*
 *  InputHandler.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 1/2/11.
 *  Copyright 2011 Joshua Bodine. All rights reserved.
 *
 */

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

// program headers
#include "core/MainLoopMember.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

// library headers
#include <SDL/SDL.h>

class InputHandler : public MainLoopMember {
public:
	Keyboard keyboard;
	Mouse mouse;

	unsigned long int processEvents();
};

#endif
