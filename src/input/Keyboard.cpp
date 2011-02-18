/*
 *  Keyboard.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "input/Keyboard.h"

bool Keyboard::getKeyState(std::string key) {
	if(keyStates.find(key) != keyStates.end())
		return keyStates[key];
	else
		return false;
}

void Keyboard::loop() {
	// handle input events on our stack
	while(eventStack.size() > 0) {
		SDL_Event event = eventStack.front();

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

		eventStack.erase(eventStack.begin());
	}
}
