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
#include "CoreFoundation/CoreFoundation.h"
#include <mach/mach_time.h>
#include <SDL/SDL.h>
#include <time.h>

Platform::Platform() {
	// determine the resource directory path
	CFBundleRef bundle = CFBundleGetMainBundle();
	CFURLRef resources = CFBundleCopyResourcesDirectoryURL(bundle);

	char detectedDataPath[PATH_MAX];
	if(! CFURLGetFileSystemRepresentation(resources, TRUE,
			(UInt8 *)detectedDataPath, PATH_MAX))
		ProgramLog::report(LOG_FATAL, "Could not get path for resource directory.");
	CFRelease(resources);

	dataPath = detectedDataPath;

	// initialize the random number generator
	srand(time(NULL));
}

void Platform::hideCursor() {
	SDL_ShowCursor(SDL_DISABLE);
}

void Platform::warpCursor(unsigned int x, unsigned int y) {
	SDL_WarpMouse(x, y);
}

unsigned int Platform::getExecutionTimeMicros() {
	static unsigned long int beginning = mach_absolute_time();
	unsigned long int now = mach_absolute_time();
	mach_timebase_info_data_t timeInfo;
	kern_return_t error = mach_timebase_info(&timeInfo);

	if(error)
		ProgramLog::report(LOG_FATAL, "An error occurred when attempting to retrieve the time.");

	return (0.001 * (double) timeInfo.numer / (double) timeInfo.denom) *
			(double) (now - beginning);
}

void Platform::sleepMicros(unsigned int micros) {
	timespec delayTime;
	delayTime.tv_sec = 0;
	delayTime.tv_nsec = micros * 1000;
	nanosleep(&delayTime, NULL);
}
