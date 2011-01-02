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
InputHandler inputHandler;
Platform platform;
Ship ship(Vector4(0.0f, 2.0f, -50.0f, 0.0f));
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
	ShipControl shipControl(gameWindow, &ship);

	// with the graphics context initialized, check for a compatible system
	SystemInfo::check();

	// main program loop
	while(keepDominicusAlive && ! inputHandler.keyboard.getKeyState("quit")) {
		// see if we need to change the fullscreen status
		static KeyTrap fullScreenKeyTrap("toggleFullScreen");
		fullScreenKeyTrap.loop();

		if(fullScreenKeyTrap.newPress()) {
			bool currentFullScreen = gameWindow->fullScreen;

			delete(drawingMaster);
			delete(gameWindow);

			gameWindow = new GameWindow(!currentFullScreen);
			drawingMaster = new DrawingMaster(gameWindow);

			shipControl.gameWindow = gameWindow;
		}

		// or regenerate the terrain
		static KeyTrap newTerrainKeyTrap("newTerrain");
		newTerrainKeyTrap.loop();

		if(newTerrainKeyTrap.newPress())
			terrain = Terrain();

		// keep a timer for each module we loop over
		static unsigned long int inputTimer = 0;
		static unsigned long int drawingTimer = 0;
		static unsigned long int shipTimer = 0;

		// If the next execution time is less than now, run it. If not, store
		// the time until due we know how long to sleep if none of our modules
		// are ready.
		unsigned long int plannedWait = -1;	// this will be set to the minimum sleep time
		unsigned long int now = platform.getExecutionTimeMicros();

		// input polling
		if(inputTimer <= now)
			inputTimer = now + inputHandler.processEvents();
		if(inputTimer - now < plannedWait)
			plannedWait = inputTimer - now;

		// renderer
		if(drawingTimer <= now)
			drawingTimer = now + drawingMaster->loop();

		if(drawingTimer - now < plannedWait)
			plannedWait = drawingTimer - now;

		// ship
		shipControl.loop();

		if(shipTimer <= now)
			shipTimer = now + ship.loop();

		if(shipTimer - now < plannedWait)
			plannedWait = shipTimer - now;

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
