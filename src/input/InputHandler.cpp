/*
 *  InputHandler.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 1/2/11.
 *  Copyright 2011 Joshua Bodine. All rights reserved.
 *
 */

#include "InputHandler.h"

unsigned long int InputHandler::processEvents() {
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

	// calculate the maximum amount of time we can sleep in order
	// to maintain our desired frequency
	static unsigned long int last = platform.getExecutionTimeMicros();
	static unsigned long int sleepMicros = 1;

	unsigned long int now = platform.getExecutionTimeMicros();
	unsigned long int idealSleepTime = (
			gamePrefs.getInt("inputPollingFrequency") != 0 ?
			1000000 / gamePrefs.getInt("inputPollingFrequency") : 0
		);

	// adjust the target sleep micros by the factor we are off by
	sleepMicros = (sleepMicros > 0 ? sleepMicros : 1) * ((double) idealSleepTime / (double) (now - last));

	last = now;

	return sleepMicros;

}
