// gameMain.cpp
// Crucible Island

#include "core/gameMain.h"

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
	platform->loadExtensions();
	inputHandler = new InputHandler();
	gameLogic = new GameLogic();

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
				MainLoopMember* thisMember = itr->first;

				if(itr->first == gameGraphics) {
					// always run gameState and gameLogic before gameState
					if(gameState != NULL)
						gameState->execute(true);
					gameLogic->execute(true);

					// gameLogic can recreate gameGraphics mid-loop, so don't execute a null iterator
					if(mainLoopModules.find(thisMember) == mainLoopModules.end())
						break;
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
	delete(gameLogic);
	delete(inputHandler);
	delete(gameGraphics);
	delete(gameAudio);
	delete(gameSystem);
	delete(platform);

	return 0;
}
