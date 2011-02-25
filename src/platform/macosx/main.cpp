/*
 *  main.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/12/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

// program headers
#include "core/dominicusMain.h"
#include "core/ProgramLog.h"

// library headers
#include <SDL/SDL.h>

int main(int argc, char* argv[]) {
	// initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		if(programLog == NULL) {	// should always be true here
			programLog = new ProgramLog();

			// this is debug only because the program will exit immediately anyway
			programLog->report(ProgramLog::LOG_DEBUG, "SDL could not be initialized.");
		}
	}

	// call the main program routine
	int returnVal = dominicusMain(argc, argv);

	// destroy SDL
	SDL_Quit();

	// return
	return returnVal;
}

