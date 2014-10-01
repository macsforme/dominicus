// gameMain.cpp
// Crucible Island

#include "core/gameMain.h"

#include <cstdlib>
#include <map>
#include <SDL.h>

#include "audio/GameAudio.h"
#include "core/GameSystem.h"
#include "core/MainLoopMember.h"
#include "graphics/DrawingMaster.h"
#include "graphics/GameGraphics.h"
#include "input/InputHandler.h"
#include "logic/GameLogic.h"
#include "platform/Platform.h"
#include "state/GameState.h"

// global variable declarations
DrawingMaster* drawingMaster;
GameAudio* gameAudio;
GameGraphics* gameGraphics;
GameLogic* gameLogic;
GameState* gameState;
GameSystem* gameSystem;
InputHandler* inputHandler;
Platform* platform;

// main loop modules (should only be modified by the main loop or GameLogic)
std::map<MainLoopMember*, unsigned int> mainLoopModules;

// global main loop continuation flag
bool keepProgramAlive;

// main game function
int gameMain(int argc, char* argv[]) {
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
	drawingMaster = new DrawingMaster();
	inputHandler = new InputHandler();
	gameLogic = new GameLogic();

//FIXME delete this when schemes is incorporated directly into gameLogic
gameLogic->reScheme();
drawingMaster->execute();

	// prepare a map of modules which are executed up to once per loop
	mainLoopModules[inputHandler] = 0;
	mainLoopModules[gameLogic] = 0;

	// main program loop
	keepProgramAlive = true;

	while(keepProgramAlive) {
		// if the next execution time for any module is less than now, run it,
		// and calculate the maximum possible sleep time
		unsigned int nextPlannedLoop = -1;

		for(
				std::map<MainLoopMember*, unsigned int>::iterator itr = mainLoopModules.begin();
				itr != mainLoopModules.end();
				++itr
			) {
			if(itr->second < platform->getExecMills()) {
				if(itr->first == drawingMaster) {
					// always run gameState and gameLogic before drawingMaster
					if(gameState != NULL)
						gameState->execute(true);
					gameLogic->execute(true);

					// if logic re-created drawingmaster, don't execute the old, invalid object
					if(mainLoopModules.find(itr->first) == mainLoopModules.end()) {
						itr = mainLoopModules.begin();

						continue;
					}
				}

				itr->second = itr->first->execute() + platform->getExecMills();
			}

			if(itr->second < nextPlannedLoop)
				nextPlannedLoop = itr->second;
		}

		// sleep the loop if we're not already overdue for something
		if(nextPlannedLoop > platform->getExecMills())
			platform->sleepMills(nextPlannedLoop - platform->getExecMills());
	}

	// clean up objects in reverse order
	delete gameLogic;
	delete inputHandler;
	delete drawingMaster;
	delete gameGraphics;
	delete gameAudio;
	delete gameSystem;
	delete platform;

	return 0;
}
