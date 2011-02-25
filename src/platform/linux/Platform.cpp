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
#include <SDL/SDL.h>
#include <sys/time.h>

Platform::Platform() {
	// determine the resource directory path
	dataPath = ".";

	// initialize the random number generator
	srand(time(NULL));
}

void Platform::consoleOut(std::string output) {
	std::cout << output;
}

void Platform::hideCursor() {
	SDL_ShowCursor(SDL_DISABLE);
}

void Platform::warpCursor(unsigned int x, unsigned int y) {
	SDL_WarpMouse(x, y);
}

unsigned int Platform::getExecMills() {
	timeval tv;
	if(gettimeofday(&tv, NULL) == -1)
		programLog.report(ProgramLog::LOG_FATAL, "An error occurred when attempting to retrieve the time.");

	static unsigned int beginning = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	unsigned int now = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	return now - beginning;
}

void Platform::sleepMills(unsigned int mills) {
	timespec delayTime;
	delayTime.tv_sec = 0;
	delayTime.tv_nsec = mills * 1000000;
	nanosleep(&delayTime, NULL);
}

void Platform::loadExtensions(){
}
