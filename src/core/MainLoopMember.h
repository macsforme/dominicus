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
#include "Platform.h"

// global variables
extern Platform platform;

class MainLoopMember {
private:
	unsigned long int now;
	unsigned long int last;
	float sleepMicros;

public:
	MainLoopMember();

	unsigned long int getSleepTime(unsigned long int idealSleepTime);
};

#endif
