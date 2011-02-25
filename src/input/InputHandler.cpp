/*
 *  InputHandler.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 1/2/11.
 *  Copyright 2011 Joshua Bodine. All rights reserved.
 *
 */

#include "input/InputHandler.h"

unsigned int InputHandler::execute() {
	// poll SDL for events and forward the ones we use to the appropriate
	// input handler
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keyboard.eventStack.push_back(event);

			break;
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			mouse.eventStack.push_back(event);

			break;
		default:
			break;
		}
	}

	// run the input handlers
	keyboard.loop();
	mouse.loop();

	// calculate and return sleep time from superclass
	unsigned int frequency = (unsigned int) gamePrefs->getFloat("inputPollingFrequency");
	static const unsigned int idealSleepTime = (
			frequency  != 0 ?
			1000 / frequency : 0
		);
	return getSleepTime(idealSleepTime);
}
