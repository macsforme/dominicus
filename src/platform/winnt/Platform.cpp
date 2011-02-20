/*
 *  Platform.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "platform/Platform.h"

// platform-specific headers
#include "platform/OpenGLHeaders.h"
#include <SDL/SDL.h>
#include <time.h>

extern HINSTANCE myinstance;

Platform::Platform() {
	// determine the resource directory path
	dataPath = "./data/";

	// initialize the random number generator
	srand(SDL_GetTicks());
}

void Platform::init()
{
	if (myinstance)
	{
		char appName[MAX_PATH];
		GetModuleFileName(myinstance,appName,MAX_PATH);
		char* p = strrchr(appName,'\\');
		if (p)
			*p = 0;
		std::string d = appName;
		d += "\\data\\";
		if (GetFileAttributes(d.c_str()) != INVALID_FILE_ATTRIBUTES)
			dataPath = d;
		else
		{
			d = appName;
			d += "\\..\\data\\";
			if (GetFileAttributes(d.c_str()) != INVALID_FILE_ATTRIBUTES)
				dataPath = d;
		}
	}
}

void Platform::hideCursor() {
	SDL_ShowCursor(SDL_DISABLE);
}

void Platform::warpCursor(unsigned int x, unsigned int y) {
	SDL_WarpMouse(x, y);
}

unsigned int Platform::getExecMills() {
	return SDL_GetTicks();
}

void Platform::sleepMills(unsigned int mills) {
	Sleep(micros);
}

void Platform::loadExtensions(){
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  // do something?
	}
}
