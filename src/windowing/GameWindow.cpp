/*
 *  GameWindow.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "windowing/GameWindow.h"

GameWindow::GameWindow(bool fullScreen) : fullScreen(fullScreen) {
	// initialize our viewing screen
	width = (fullScreen ? systemInfo->screenWidth : (int) gamePrefs->getFloat("windowWidth"));
	height = (fullScreen ? systemInfo->screenHeight : (int) gamePrefs->getFloat("windowHeight"));
	aspectRatio = (float) width / (float) height;

	uint32_t flags = SDL_OPENGL | SDL_DOUBLEBUF | SDL_HWSURFACE | (fullScreen ? SDL_FULLSCREEN : 0);

	if(! SDL_VideoModeOK(
			width,
			height,
			(int) gamePrefs->getFloat("windowColorDepth"),
			flags
		))
		programLog->report(ProgramLog::LOG_FATAL,
				"SDL cannot initialize a window with the specified settings.");

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, (int) gamePrefs->getFloat("windowColorDepth"));
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Surface* surface = SDL_SetVideoMode(
			width,
			height,
			(int) gamePrefs->getFloat("windowColorDepth"),
			flags
		);

	if(surface == NULL)
		programLog->report(ProgramLog::LOG_FATAL,
				"SDL could not initialize an OpenGL-ready window.");

	if(! fullScreen)
		SDL_WM_SetCaption("dominicus", NULL);

	platform->hideCursor();
	platform->warpCursor(width / 2, height / 2);
}
