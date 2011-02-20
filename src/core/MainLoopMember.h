/*
 *  MainLoopMember.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 1/11/11.
 *  Copyright 2011 Joshua Bodine. All rights reserved.
 *
 */

#ifndef MAINLOOPMEMBER_H
#define MAINLOOPMEMBER_H

// program headers
#include "platform/Platform.h"

// global variables
extern Platform platform;

class MainLoopMember {
private:
	unsigned int now;
	unsigned int last;
	float sleepMills;

protected:
	unsigned int getSleepTime(unsigned int idealSleepTime);

public:
	MainLoopMember();

	// returns milliseconds to sleep
	virtual unsigned int execute() { return 0; };
};

#endif
