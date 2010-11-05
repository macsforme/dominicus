/*
 *  Keyboard.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "Keyboard.h"

bool Keyboard::getKeyState(std::string key) {
	if(keyStates.find(key) != keyStates.end())
		return keyStates[key];
	else
		return false;
}

unsigned long int Keyboard::loop() {
	// poll SDL for keyboard events
	SDL_Event event;
	std::map<std::string,bool> keyEvents;

	SDL_PumpEvents();

	while(SDL_PeepEvents(NULL, 1, SDL_PEEKEVENT, SDL_KEYEVENTMASK)) {
		SDL_PollEvent(&event);

		switch(event.type) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keyStates[gamePrefs.getBinding(event.key.keysym.sym)] =
					(event.type == SDL_KEYDOWN ? true : false);

			break;
		default:
			ProgramLog::report(LOG_DEBUG, "Received unhandled keyboard event.");

			break;

		}
	}

	// calculate the maximum amount of time we can sleep in order
	// to maintain our desired frequency
	static unsigned long int last = platform.getExecutionTimeMicros();
	static unsigned long int sleepMicros = 1;

	unsigned long int now = platform.getExecutionTimeMicros();
	unsigned long int idealSleepTime = (
			gamePrefs.getInt("keyboardPollFrequency") != 0 ?
			1000000 / gamePrefs.getInt("keyboardPollFrequency") : 0
		);

	// adjust the target sleep micros by the factor we are off by
	sleepMicros = (sleepMicros > 0 ? sleepMicros : 1) * ((double) idealSleepTime / (double) (now - last));

	last = now;

	return sleepMicros;
}
