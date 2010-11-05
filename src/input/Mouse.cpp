/*
 *  Mouse.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/5/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "Mouse.h"

unsigned long int Mouse::loop() {
	// poll SDL for mouse events
	SDL_Event event;
	std::map<std::string,bool> keyEvents;

	SDL_PumpEvents();
	
	while(SDL_PeepEvents(NULL, 1, SDL_PEEKEVENT, SDL_MOUSEEVENTMASK)) {
		SDL_PollEvent(&event);

		switch(event.type) {
		case SDL_MOUSEMOTION:
			positionX = event.motion.x;
			positionY = event.motion.y;

			break;
		case SDL_MOUSEBUTTONDOWN:
			break;
		case SDL_MOUSEBUTTONUP:
			break;
		default:
			ProgramLog::report(LOG_DEBUG, "Received unhandled mouse event.");

			break;

		}
	}

	// calculate the maximum amount of time we can sleep in order
	// to maintain our desired frequency
	static unsigned long int last = platform.getExecutionTimeMicros();
	static unsigned long int sleepMicros = 1;

	unsigned long int now = platform.getExecutionTimeMicros();
	unsigned long int idealSleepTime = (
			gamePrefs.getInt("mousePollFrequency") != 0 ?
			1000000 / gamePrefs.getInt("mousePollFrequency") : 0
		);

	// adjust the target sleep micros by the factor we are off by
	sleepMicros = (sleepMicros > 0 ? sleepMicros : 1) * ((double) idealSleepTime / (double) (now - last));

	last = now;

	return sleepMicros;
}