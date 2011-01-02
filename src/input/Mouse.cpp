/*
 *  Mouse.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/5/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "Mouse.h"

void Mouse::loop() {
	// handle input events on our stack
	while(eventStack.size() > 0) {
		SDL_Event event = eventStack.front();

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

		eventStack.erase(eventStack.begin());
	}
}
