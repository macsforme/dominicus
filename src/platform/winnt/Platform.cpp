/*
 *  Platform.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "Platform.h"

// platform-dependent includes
#include <SDL/SDL.h>
#include "OpenGLHeaders.h"

#include <time.h>

Platform::Platform() {
	// determine the resource directory path
	dataPath = ".";

	// initialize the random number generator
	srand(SDL_GetTicks());
}

const char* Platform::getArchitecture() {
	return "WinNT";
}

void Platform::hideCursor() {
	SDL_ShowCursor(SDL_DISABLE);
}

void Platform::warpCursor(unsigned int x, unsigned int y) {
	SDL_WarpMouse(x, y);
}

unsigned int Platform::getExecutionTimeMicros() {
	return SDL_GetTicks();
}

void Platform::sleepMicros(unsigned int micros) {
	Sleep(micros);
}

void Platform::loadExtensions(){
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  // do something?
	}
}
