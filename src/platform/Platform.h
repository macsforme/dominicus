/*
 * Platform.h
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

#ifndef PLATFORM_H
#define PLATFORM_H

// this should be the one and only compile-time determination system-wide
// of what architecture the program is being compiled for (generally the
// code should not have platform-specific behavior outside of Platform,
// but we accept that there must be exceptions
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

// forward declarations
class ProgramLog;

// program headers
#include "core/ProgramLog.h"

// library headers
#include <map>
#include <stdint.h>
#include <string>

// global variables
extern ProgramLog* programLog;

class Platform {
public:
	std::string dataPath;

	Platform();

	// static system console output function
	static void consoleOut(std::string output);

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

