// MainLoopMember.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef MAINLOOPMEMBER_H
#define MAINLOOPMEMBER_H

// program headers
#include "platform/Platform.h"

// global variables
extern Platform* platform;

class MainLoopMember {
private:
	unsigned int now;
	unsigned int last;
	float sleepMills;

public:
	MainLoopMember();

	// returns milliseconds to sleep
	virtual unsigned int execute() { return 0; };

protected:
	unsigned int getSleepTime(unsigned int idealSleepTime);
};

#endif
