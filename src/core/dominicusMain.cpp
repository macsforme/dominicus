/*
 *  dominicusMain.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "core/dominicusMain.h"

// global variable declarations
InputHandler* inputHandler = NULL;
DrawingMaster* drawingMaster = NULL;
GamePrefs* gamePrefs = NULL;
GameWindow* gameWindow = NULL;
Platform* platform = NULL;
ProgramLog* programLog = NULL;
SystemInfo* systemInfo = NULL;

Ship* ship = NULL;
Terrain* terrain = NULL;

bool keepDominicusAlive;	// global flag to continue game

// main game function
int dominicusMain(int argc, char* argv[]) {
	// initialize our core system now that SDL_Init() has been called
	programLog = new ProgramLog();
	platform = new Platform();
	systemInfo = new SystemInfo();
	gamePrefs = new GamePrefs();

	// initialize graphics, platform extensions, and then check system compatibility
	gameWindow = new GameWindow(gamePrefs->getBool("windowStartFullScreen") ? true : false);
	platform->loadExtensions();
	systemInfo->check();

	// initialize state
	terrain = new Terrain();
	ship = new Ship(Vector4(0.0f, 2.0f, -500.0f, 0.0f));
	ShipControl shipControl(ship);

	// initialize input and prepare drawing
	inputHandler = new InputHandler();
	drawingMaster = new DrawingMaster();

	// prepare a map of modules to be executed up to once per loop
	std::map<MainLoopMember*,unsigned int> mainLoopModules;

	mainLoopModules[inputHandler] = 0;
	mainLoopModules[drawingMaster] = 0;
	mainLoopModules[ship] = 0;

	// main program loop
	keepDominicusAlive = true;

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
			terrain = new Terrain();

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
			now = platform->getExecMills();

			if(itr->second < now)
				itr->second = now + (itr->first)->execute();

			if(itr->second < nextPlannedLoop)
				nextPlannedLoop = itr->second;
		}

		// sleep the loop if we're not already overdue for something
		if(nextPlannedLoop > now)
			platform->sleepMills(nextPlannedLoop - now);
	}

	// clean up objects in reverse order
	delete(drawingMaster);
	delete(inputHandler);
	delete(gameWindow);
	delete(systemInfo);
	delete(platform);
	delete(programLog);

	return 0;
}
