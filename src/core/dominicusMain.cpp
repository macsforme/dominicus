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
	gameGraphics = NULL;
	gameSystem = NULL;
	inputHandler = NULL;
	platform = NULL;

	// finish SDL-related initialization
	SDL_EnableUNICODE(1);

	// initialize our common objects
	platform = new Platform();
	gameSystem = new GameSystem();

	gameGraphics = new GameGraphics(gameSystem->getBool("displayStartFullScreen"), true);
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
	delete(gameSystem);
	delete(platform);

	return 0;
}
