/*
 *  MainLoopMember.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 1/11/11.
 *  Copyright 2011 Joshua Bodine. All rights reserved.
 *
 */

#include "core/MainLoopMember.h"

MainLoopMember::MainLoopMember() {
	// variable initialization
	now = platform.getExecMills();
	last = now;
	sleepMills = 1.0f;
}

unsigned int MainLoopMember::getSleepTime(unsigned int idealSleepTime) {
	now = platform.getExecMills();
	unsigned int timeDiff = (now - last);

	// adjust the target sleep micros by the factor we are off by
	if(timeDiff == 0) {
		return (unsigned int) sleepMills;
	} else if(timeDiff < idealSleepTime / 2) {
		// avoid over-compensating if we hit way earlier than expected
		sleepMills *= 1.5f;
	} else if (timeDiff > idealSleepTime * 2) {
		// same thing if we hit much later than expected
		sleepMills /= 1.5f;
	} else {
		// perform minute adjustments
		sleepMills *= ((float) idealSleepTime / (float) (now - last));
	}

	last = now;

	return (unsigned int) sleepMills;
}
