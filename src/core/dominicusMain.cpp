/*
 *  dominicusMain.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "core/dominicusMain.h"

// global variable instantiations
GamePrefs gamePrefs;
Platform platform;
Ship ship(Vector4(0.0f, 2.0f, -500.0f, 0.0f));
SystemInfo systemInfo;
Terrain terrain;

InputHandler* inputHandler = NULL;
GameWindow* gameWindow = NULL;
DrawingMaster* drawingMaster = NULL;

bool keepDominicusAlive;	// global flag to continue game

// main game function
int dominicusMain(int argc, char* argv[]) {
	// let the platform init what it needs to after the main is called since the constructor happens before the main

	platform.init();

	// initialize our system information now that SDL_Init() has been called
	systemInfo.init();

	// main loop continuance variable
	keepDominicusAlive = true;

	// initialize stuff
	gameWindow = new GameWindow(gamePrefs.getBool("windowStartFullScreen") ? true : false);

	// let the platform load any openGl extensions (or extension managers) it needs
	platform.loadExtensions();

	inputHandler = new InputHandler();
	drawingMaster = new DrawingMaster();
	ShipControl shipControl(&ship);

	// with the graphics context initialized, check for a compatible system
	systemInfo.check();

	// prepare a map of modules to be executed up to once per loop
	std::map<MainLoopMember*,unsigned int> mainLoopModules;

	mainLoopModules[inputHandler] = 0;
	mainLoopModules[drawingMaster] = 0;
	mainLoopModules[&ship] = 0;
	mainLoopModules[new MainLoopMember()] = 0;

	// main program loop
	while(keepDominicusAlive && ! inputHandler->keyboard.getKeyState("quit")) {
		// see if we need to change the fullscreen status
		static KeyTrap fullScreenKeyTrap("toggleFullScreen");
		fullScreenKeyTrap.loop();

		if(fullScreenKeyTrap.newPress()) {
			bool currentFullScreen = gameWindow->fullScreen;

			mainLoopModules.erase(mainLoopModules.find(drawingMaster));

			delete(drawingMaster);
			delete(gameWindow);

			gameWindow = new GameWindow(!currentFullScreen);
			drawingMaster = new DrawingMaster();

			mainLoopModules[drawingMaster] = 0;
		}

		// or regenerate the terrain
		static KeyTrap newTerrainKeyTrap("newTerrain");
		newTerrainKeyTrap.loop();

		if(newTerrainKeyTrap.newPress()) {
			terrain = Terrain();

			drawingMaster->renderingMaster->terrainRenderer.reloadGeometry();
		}

		// if the next execution time for any module is less than now, run it
		unsigned int nextPlannedLoop = -1;	// this will be set to the minimum sleep time
		unsigned int now;

		for(
				std::map<MainLoopMember*, unsigned int>::iterator itr = mainLoopModules.begin();
				itr != mainLoopModules.end();
				++itr
			) {
			now = platform.getExecMills();

			if(itr->second < now)
				itr->second = now + (itr->first)->execute();

			if(itr->second < nextPlannedLoop)
				nextPlannedLoop = itr->second;
		}

		// sleep the loop if we're not already overdue for something
		if(nextPlannedLoop > now)
			platform.sleepMills(nextPlannedLoop - now);
	}

	// clean up objects
	delete (drawingMaster);
	delete (gameWindow);

	return 0;
}
