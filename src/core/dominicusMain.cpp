// dominicusMain.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "core/dominicusMain.h"

// global variable declarations
GameAudio* gameAudio;
GameGraphics* gameGraphics;
GameLogic* gameLogic;
GameState* gameState;
GameSystem* gameSystem;
InputHandler* inputHandler;
Platform* platform;

// main loop modules (should only be modified by the main loop or GameLogic)
std::map<MainLoopMember*,unsigned int> mainLoopModules;

// global main loop continuation flag
bool keepDominicusAlive;

// main game function
int dominicusMain(int argc, char* argv[]) {
	// nullify
	gameAudio = NULL;
	gameGraphics = NULL;
	gameSystem = NULL;
	inputHandler = NULL;
	platform = NULL;

	// finish SDL-related initialization
	SDL_EnableUNICODE(1);

	// initialize our common objects
	platform = new Platform();
	gameSystem = new GameSystem();
	gameAudio = new GameAudio();
	gameGraphics = new GameGraphics(gameSystem->getBool("displayStartFullscreen"), true);
	platform->loadExtensions();
	inputHandler = new InputHandler();
	gameLogic = new GameLogic();

	// prepare a map of modules which are executed up to once per loop
	mainLoopModules[inputHandler] = 0;
	mainLoopModules[gameLogic] = 0;

	// main program loop
	keepDominicusAlive = true;

	while(keepDominicusAlive) {
		// if the next execution time for any module is less than now, run it,
		// and calculate the maximum possible sleep time
		unsigned int nextPlannedLoop = -1;

		for(
				std::map<MainLoopMember*, unsigned int>::iterator itr = mainLoopModules.begin();
				itr != mainLoopModules.end();
				++itr
			) {
			if(itr->second < platform->getExecMills())
				itr->second = itr->first->execute() + platform->getExecMills();

			if(itr->second < nextPlannedLoop)
				nextPlannedLoop = itr->second;
		}

		// sleep the loop if we're not already overdue for something
		if(nextPlannedLoop > platform->getExecMills())
			platform->sleepMills(nextPlannedLoop - platform->getExecMills());
	}

	// clean up objects in reverse order
	delete(gameLogic);
	delete(inputHandler);
	delete(gameGraphics);
	delete(gameAudio);
	delete(gameSystem);
	delete(platform);

	return 0;
}
