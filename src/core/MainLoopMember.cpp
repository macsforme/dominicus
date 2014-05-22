// MainLoopMember.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "core/MainLoopMember.h"

MainLoopMember::MainLoopMember(unsigned int requestedSleepMills) {
	last = 0;
	targetSleepMills = (int) (requestedSleepMills != 0 ? 1000 / requestedSleepMills : 0);
	workingSleepMills = targetSleepMills;
	lastRunCountCheck = 0;
	runsCounter = 0;
	runRate = 0;
}

unsigned int MainLoopMember::getSleepTime() {
	// calculate the required sleep time
	unsigned int now = platform->getExecMills();
	int timeDiff = (int) (now - (unsigned int) last);

	int adjustment = (timeDiff - targetSleepMills) / 2;
	if(adjustment > workingSleepMills)
		workingSleepMills = 0;
	else
		workingSleepMills = workingSleepMills - adjustment;

	last = now;

	return workingSleepMills;
}

void MainLoopMember::trackRunCount() {
	// track run count per second
	unsigned int now = platform->getExecMills();

	++runsCounter;
	if(now > lastRunCountCheck + 1000) {
		runRate = runsCounter;

		runsCounter = 0;
		lastRunCountCheck = now;
	}
}
