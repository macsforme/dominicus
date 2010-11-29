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
GamePrefs gamePrefs;
Keyboard keyboard;
Mouse mouse;
Platform platform;
SystemInfo systemInfo;
Terrain terrain;

bool keepDominicusAlive;	// global flag to continue game

// main game function
int dominicusMain(int argc, char* argv[]) {
	// initialize our system information now that SDL_Init() has been called
	systemInfo.init();

	// main loop continuance variable
	keepDominicusAlive = true;

	// initialize stuff
	GameWindow* gameWindow = new GameWindow(gamePrefs.getBool("windowStartFullScreen") ? true : false);
	DrawingMaster* drawingMaster = new DrawingMaster(gameWindow);

	// with the graphics context initialized, check for a compatible system
	SystemInfo::check();

	// keep a timer for each module we loop over
	unsigned long int keyboardTimer = 0;
	unsigned long int mouseTimer = 0;
	unsigned long int drawingTimer = 0;

	// main program loop
	while(keepDominicusAlive && ! keyboard.getKeyState("quit")) {
		// see if we need to change the fullscreen status
		static KeyTrap fullScreenKeyTrap("toggleFullScreen");
		fullScreenKeyTrap.loop();

		if(fullScreenKeyTrap.newPress()) {
			bool currentFullScreen = gameWindow->fullScreen;

			delete(drawingMaster);
			delete(gameWindow);

			gameWindow = new GameWindow(!currentFullScreen);
			drawingMaster = new DrawingMaster(gameWindow);
		}

		// or regenerate the terrain
		static KeyTrap newTerrainKeyTrap("newTerrain");
		newTerrainKeyTrap.loop();

		if(newTerrainKeyTrap.newPress())
			terrain = Terrain();

		// If the next execution time is less than now, run it. If not, store
		// the time until due we know how long to sleep if none of our modules
		// are ready.
		unsigned long int plannedWait = -1;	// this will be set to the minimum sleep time
		unsigned long int now = platform.getExecutionTimeMicros();

		// keyboard polling
		if(keyboardTimer <= now)
			keyboardTimer = now + keyboard.loop();
		if(keyboardTimer - now < plannedWait)
			plannedWait = keyboardTimer - now;

		// mouse polling
		if(mouseTimer <= now)
			mouseTimer = now + mouse.loop();
		if(mouseTimer - now < plannedWait)
			plannedWait = mouseTimer - now;

		// renderer
		if(drawingTimer <= now)
			drawingTimer = now + drawingMaster->loop();

		if(drawingTimer - now < plannedWait)
			plannedWait = drawingTimer - now;

		// sanity check
		if(plannedWait == -1)
			ProgramLog::report(LOG_INTERNALERROR, "Loop sleep time was unable to be properly set.");

		// sleep the loop if we're not already overdue for something
		if(plannedWait > 0)
			platform.sleepMicros(plannedWait);
	}

	// clean up objects
	delete (drawingMaster);
	delete (gameWindow);

	return 0;
}
