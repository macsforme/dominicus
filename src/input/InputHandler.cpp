// InputHandler.cpp
// Crucible Island

#include "input/InputHandler.h"

#include <SDL.h>

#include "core/GameSystem.h"

extern GameSystem* gameSystem;
extern bool keepProgramAlive;

InputHandler::InputHandler() : MainLoopMember((unsigned int) gameSystem->getFloat("inputPollingFrequency")) { }

unsigned int InputHandler::execute(bool unScheduled) {
	// poll SDL for events and forward the ones we use to the appropriate input handler
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			keepProgramAlive = false;

			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keyboard.eventStack.push_back(event);

			break;
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_ACTIVEEVENT:
			mouse.eventStack.push_back(event);

			break;
		default:
			break;
		}
	}

	keyboard.execute();
	mouse.execute();

	trackRunCount();

	// calculate and return sleep time from superclass
	if(unScheduled)
		return 0;
	else
		return getSleepTime();
}
