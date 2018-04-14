// main.cpp
// Dominicus

#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

#include "core/gameMain.h"

int main(int argc, char* argv[]) {
	// initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		// no GameSystem to log yet so just print and exit
		std::cout << "SDL could not be initialized." << std::endl;
		exit(1);
	}

	// call the main program routine
	int returnVal = gameMain(argc, argv);

	// destroy SDL
	SDL_Quit();

	// return
	return returnVal;
}
