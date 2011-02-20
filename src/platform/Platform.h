/*
 *  Platform.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef PLATFORM_H
#define PLATFORM_H

// this should be the one and only compile-time determination system-wide
// of what architecture the program is being compiled for
#if defined(__APPLE__) && defined(__MACH__)
	#define PROGRAM_ARCH_MACOSX
	#define PROGRAM_ARCH_STR "MacOSX"
#elif defined(_WIN32)
	#define PROGRAM_ARCH_WINNT
	#define PROGRAM_ARCH_STR "WinNT"
#elif defined(linux) || defined(__linux)
	#define PROGRAM_ARCH_LINUX
	#define PROGRAM_ARCH_STR "Linux"
#else
	#define PROGRAM_ARCH_UNKNOWN
	#define PROGRAM_ARCH_STR "Unknown"
#endif

// program headers
#include "core/ProgramLog.h"

// library headers
#include <map>
#include <stdint.h>
#include <string>

// global variables
extern ProgramLog programLog;

class Platform {
public:
	std::string dataPath;

	Platform();

	void init();

	// input functions
	void hideCursor();
	void warpCursor(unsigned int x, unsigned int y);

	// time functions
	unsigned int getExecMills();
	void sleepMills(unsigned int mills);

	// GL context function
	void loadExtensions();
};

#endif

