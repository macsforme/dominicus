/*
 *  Platform.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "Platform.h"

// platform-specific headers
#include <SDL/SDL.h>
#include <sys/time.h>

Platform::Platform() {
	// determine the resource directory path
	dataPath = ".";

	// initialize the random number generator
	srand(time(NULL));
}

void Platform::init(){
}

const char* Platform::getArchitecture() {
	return "Linux";
}

void Platform::hideCursor() {
	SDL_ShowCursor(SDL_DISABLE);
}

void Platform::warpCursor(unsigned int x, unsigned int y) {
	SDL_WarpMouse(x, y);
}

unsigned int Platform::getExecutionTimeMicros() {
	timeval tv;
	if(gettimeofday(&tv, NULL) == -1)
		ProgramLog::report(LOG_FATAL, "An error occurred when attempting to retrieve the time.");

	static unsigned long int beginning = tv.tv_sec * 1000000 + tv.tv_usec;
	unsigned long int now = tv.tv_sec * 1000000 + tv.tv_usec;

	return now - beginning;
}

void Platform::sleepMicros(unsigned int micros) {
	timespec delayTime;
	delayTime.tv_sec = 0;
	delayTime.tv_nsec = micros * 1000;
	nanosleep(&delayTime, NULL);
}

void Platform::loadExtensions(){
}
