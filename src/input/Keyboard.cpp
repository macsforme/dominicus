/*
 *  Keyboard.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "Keyboard.h"

unsigned long int Keyboard::loop() {
	// poll SDL for keyboard events
	SDL_Event event;
	std::map<std::string,bool> keyEvents;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				bool pressed = (event.type == SDL_KEYDOWN ? true : false);

				// see if it's bound to a function, and if so, if we use it
				std::string binding = gamePrefs.getBinding(event.key.keysym.sym);

				if(binding == "quit") {
					if(pressed) {
						// quit key
						keepDominicusAlive = false;
					}
				} else if(binding == "toggleFullScreen") {
					if(pressed) {
						bool currentFullScreen = (*screen)->fullScreen;

						delete(*screen);

						*screen = new Screen(!currentFullScreen);
					}
				}
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


