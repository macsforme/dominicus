/*
 * Ship.cpp
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

#include "state/Ship.h"

Ship::Ship(Vector4 position) :
		position(position),
		speed(0.0f),
		shipControl(new ShipControl(this)) {
	orientation.identity();
}

unsigned int Ship::execute() {
	// run our controller
	shipControl->loop();

	// get our delta T
	static float lastUpdate = (float) platform->getExecMills();
	float dt = platform->getExecMills() - lastUpdate;
	lastUpdate += dt;

	// update the position
	position += (Vector4(0.0f, 0.0f, 1.0f, 0.0f) * ((float) dt / 1000.0f * speed)) * orientation;

	// calculate and return sleep time from superclass
	unsigned int frequency = (unsigned int) gamePrefs->getFloat("shipUpdateFrequency");
	static const unsigned int idealSleepTime = (
			frequency != 0 ?
			1000 / frequency : 0
		);
	return getSleepTime(idealSleepTime);
}
