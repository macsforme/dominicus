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
#include <CoreFoundation/CoreFoundation.h>
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
			(UInt8 *)detectedDataPath, PATH_MAX))
		programLog->report(ProgramLog::LOG_FATAL, "Could not get path for resource directory.");
	CFRelease(resources);

	dataPath = detectedDataPath;

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
	static uint64_t beginning = mach_absolute_time();
	uint64_t now = mach_absolute_time();
	mach_timebase_info_data_t timeInfo;
	kern_return_t error = mach_timebase_info(&timeInfo);

	if(error)
		programLog->report(ProgramLog::LOG_FATAL,
				"An error occurred when attempting to retrieve the time.");

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

