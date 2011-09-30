// InputHandler.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "input/InputHandler.h"

InputHandler::InputHandler() {
	keyboard = new Keyboard();
	mouse = new Mouse();
}

InputHandler::~InputHandler() {
	delete(keyboard);
	delete(mouse);
}

unsigned int InputHandler::execute() {
	// poll SDL for events and forward the ones we use to the appropriate
	// input handler
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keyboard->eventStack.push_back(event);

			break;
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
			mouse->eventStack.push_back(event);

			break;
		default:
			break;
		}
	}

	// run the input handlers
	keyboard->execute();
	mouse->execute();

	// calculate and return sleep time from superclass
	unsigned int frequency = (unsigned int) gameSystem->getFloat("inputPollingFrequency");
	static const unsigned int idealSleepTime = (
			frequency  != 0 ?
			1000 / frequency : 0
		);
	return getSleepTime(idealSleepTime);
}
