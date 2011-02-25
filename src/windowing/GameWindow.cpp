/*
 * GameWindow.cpp
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
