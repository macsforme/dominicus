// InputHandler.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

// forward declarations
class Keyboard;
class Mouse;

// program headers
#include "core/GameSystem.h"
#include "core/MainLoopMember.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

// library headers
#include <SDL.h>

// global variables
extern GameSystem* gameSystem;

class InputHandler : public MainLoopMember {
public:
	Keyboard* keyboard;
	Mouse* mouse;

	InputHandler();
	~InputHandler();

	unsigned int execute();
};

#endif
