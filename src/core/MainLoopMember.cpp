/*
 * MainLoopMember.cpp
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

#include "core/MainLoopMember.h"

MainLoopMember::MainLoopMember() {
	// variable initialization
	now = platform->getExecMills();
	last = now;
	sleepMills = 1.0f;
}

unsigned int MainLoopMember::getSleepTime(unsigned int idealSleepTime) {
	now = platform->getExecMills();
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
