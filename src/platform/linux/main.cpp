/*
 * main.cpp
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

// program headers
#include "core/dominicusMain.h"
#include "core/GameSystem.h"

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

