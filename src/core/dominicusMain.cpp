/*
 * dominicusMain.cpp
 * Dominicus
 *
 * Copyright 2010-2011, Joshua Bodine
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
