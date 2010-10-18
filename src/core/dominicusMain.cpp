/*
 *  dominicusMain.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "dominicusMain.h"

// global variable instantiations
SystemInfo systemInfo;
GamePrefs gamePrefs;
Platform platform;
bool keepDominicusAlive;	// global flag to continue game

// main game function
int dominicusMain(int argc, char* argv[]) {
	// initialize our system information now that SDL_Init() has been called
	systemInfo.init();

	// main loop continuance variable
	keepDominicusAlive = true;

	// initialize stuff
	Screen* screen = new Screen(gamePrefs.getBool("windowStartFullScreen") ? true : false);
	Keyboard* keyboard = new Keyboard(&screen);
	RenderingMaster* renderingMaster = new RenderingMaster();

	// with the graphics context initialized, check for a compatible system
	SystemInfo::check();

	// keep a timer for each module we loop over
	unsigned long int keyboardTimer = 0;
	unsigned long int renderingTimer = 0;

	// main program loop
	while(keepDominicusAlive) {
		// If the next execution time is less than now, run it. If not, store
		// the time until due we know how long to sleep if none of our modules
		// are ready.

		unsigned long int plannedWait = -1;	// this will be set to the minimum sleep time
		unsigned long int now = platform.getExecutionTimeMicros();

		// keyboard
		if(keyboardTimer <= now)
			keyboardTimer = now + keyboard->loop();
		if(keyboardTimer - now < plannedWait)
			plannedWait = keyboardTimer - now;

		// renderer
		if(renderingTimer <= now)
			renderingTimer = now + renderingMaster->loop();
		if(renderingTimer - now < plannedWait)
			plannedWait = renderingTimer - now;

		// sanity check
		if(plannedWait == -1)
			ProgramLog::report(LOG_INTERNALERROR, "Loop sleep time was unable to be properly set.");

		// sleep the loop if we're not already overdue for something
		if(plannedWait > 0)
			platform.sleepMicros(plannedWait);
	}

	// clean up objects
	delete (renderingMaster);
	delete (screen);
	delete (keyboard);

	return 0;
}


