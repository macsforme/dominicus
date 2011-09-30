// Platform.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "platform/Platform.h"

// platform-specific headers
#include <CoreFoundation/CoreFoundation.h>
#include <iostream>
#include <mach/mach_time.h>
#include <SDL/SDL.h>
#include <stdint.h>
#include <time.h>

Platform::Platform() {
	// determine the resource directory path
	CFBundleRef bundle = CFBundleGetMainBundle();
	CFURLRef resources = CFBundleCopyResourcesDirectoryURL(bundle);

	char detectedDataPath[PATH_MAX];
	if(! CFURLGetFileSystemRepresentation(resources, TRUE,
			(UInt8 *)detectedDataPath, PATH_MAX)) {
		// no GameSystem yet to log so just print and exit
		std::cout << "Could not get path for resource directory." << std::endl;
		exit(1);
	}
	CFRelease(resources);

	dataPath = detectedDataPath;

	// initialize the random number generator
	srand(time(NULL));
}

void Platform::consoleOut(std::string output) {
	std::cout << output;
}

unsigned int Platform::getExecMills() {
	static uint64_t beginning = mach_absolute_time();
	uint64_t now = mach_absolute_time();
	mach_timebase_info_data_t timeInfo;
	kern_return_t error = mach_timebase_info(&timeInfo);

	if(error) {
		if(gameSystem != NULL) {
			gameSystem->log(GameSystem::LOG_FATAL,
					"An error occurred when attempting to retrieve the time.");
		} else {
			std::cout << "An error occurred when attempting to retrieve the time." << std::endl;
		}
	}

	return (unsigned int) (
			(0.000001 *
					((double) timeInfo.numer / (double) timeInfo.denom) *
					(double) (now - beginning)
				)
		);
}

void Platform::sleepMills(unsigned int mills) {
	timespec delayTime;
	delayTime.tv_sec = 0;
	delayTime.tv_nsec = mills * 1000000;
	nanosleep(&delayTime, NULL);
}

void Platform::loadExtensions(){
}

