// InputHandler.h
// Dominicus

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "core/MainLoopMember.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

class InputHandler : public MainLoopMember {
public:
	Keyboard keyboard;
	Mouse mouse;

	InputHandler();

	unsigned int execute(bool unScheduled = false);
};

#endif // INPUTHANDLER_H
