// InputHandler.h
// Crucible Island

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

	unsigned int execute(bool unScheduled = false);
};

#endif // INPUTHANDLER_H
