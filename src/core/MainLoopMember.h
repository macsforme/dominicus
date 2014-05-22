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
protected:
	unsigned int last;
	int targetSleepMills;
	int workingSleepMills;

	unsigned int lastRunCountCheck;
	unsigned int runsCounter;

	// called by inherited classes to calculate milliseconds needed to sleep
	unsigned int getSleepTime();

	MainLoopMember(unsigned int requestedSleepMills);

public:
	unsigned int runRate;

	// returns milliseconds to sleep
	virtual unsigned int execute(bool unScheduled = false) = 0;

	// runcount checking
	void trackRunCount();
};

#endif
