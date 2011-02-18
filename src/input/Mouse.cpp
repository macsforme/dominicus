/*
 *  Mouse.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/5/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "input/Mouse.h"

void Mouse::loop() {
	// handle input events on our stack
	while(eventStack.size() > 0) {
		SDL_Event event = eventStack.front();

		switch(event.type) {
		case SDL_MOUSEMOTION:
			position.x = (float) event.motion.x / (float) gameWindow->width * 2.0f - 1.0f;
			position.y = (float) -event.motion.y / (float) gameWindow->height * 2.0f + 1.0f;

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
