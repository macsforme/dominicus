// main.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

// program headers
#include "core/dominicusMain.h"

// library headers
#include <iostream>
#include <SDL/SDL.h>

int main(int argc, char* argv[]) {
	// initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		// no GameSystem to log yet so just print and exit
		std::cout << "SDL could not be initialized." << std::endl;
		exit(1);
	}

	// call the main program routine
	int returnVal = dominicusMain(argc, argv);

	// destroy SDL
	SDL_Quit();

	// return
	return returnVal;
}

