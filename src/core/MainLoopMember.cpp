// MainLoopMember.cpp
// Crucible Island

#include "core/MainLoopMember.h"

MainLoopMember::MainLoopMember(unsigned int requestedFrequency) {
	last = 0;
	targetSleepMills = (int) (requestedFrequency != 0 ? 1000 / requestedFrequency : 0);
	workingSleepMills = targetSleepMills;
	lastRunCountCheck = 0;
	runsCounter = 0;
	runRate = 0;
}

unsigned int MainLoopMember::getSleepTime() {
	// calculate the required sleep time
	unsigned int now = platform->getExecMills();
	int timeDiff = (int) (now - last);

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
