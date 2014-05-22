// InputHandler.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "input/InputHandler.h"

InputHandler::InputHandler() : MainLoopMember((unsigned int) gameSystem->getFloat("inputPollingFrequency")) {
	keyboard = new Keyboard();
	mouse = new Mouse();
}

InputHandler::~InputHandler() {
	delete(keyboard);
	delete(mouse);
}

unsigned int InputHandler::execute(bool unScheduled) {
	// poll SDL for events and forward the ones we use to the appropriate
	// input handler
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			keepDominicusAlive = false;
			
			break;
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

	// track runcount
	trackRunCount();

	// calculate and return sleep time from superclass
	if(unScheduled)
		return 0;
	else
		return getSleepTime();
}
