/*
 * Platform.cpp
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

#include "platform/Platform.h"

// platform-specific headers
#include "platform/OpenGLHeaders.h"
#include <SDL/SDL.h>
#include <time.h>

extern HINSTANCE myinstance;

Platform::Platform() {
	// determine the resource directory path
	dataPath = "./data/";

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
			dataPath = appName;
		else
		{
			strcat(appName,"\\..");
			d = appName;
			d += "\\data\\";
			if (GetFileAttributes(d.c_str()) != INVALID_FILE_ATTRIBUTES)
				dataPath = appName;
		}
	} else {
		programLog->report(ProgramLog::LOG_FATAL, "Could not get path for resource directory.");
	}

	// initialize the random number generator
	srand(SDL_GetTicks());
}

void Platform::consoleOut(std::string output) {
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
	Sleep(mills);
}

void Platform::loadExtensions(){
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		programLog.report(ProgramLog::LOG_FATAL, "Unable to initialize GLEW.");
	}
}
