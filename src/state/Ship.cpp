/*
 *  Ship.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "Ship.h"

Ship::Ship(Vector4 position) :
		position(position),
		speed(0.0f) {
	orientation.identity();
}

unsigned long int Ship::loop() {
	// get our delta T
	static float lastUpdate = platform.getExecutionTimeMicros();
	float dt = platform.getExecutionTimeMicros() - lastUpdate;
	lastUpdate += dt;

	// update the position
	position += (Vector4(0.0f, 0.0f, 1.0f, 0.0f) * ((float) dt / 1000000.0f * speed)) * orientation;

	// calculate the maximum amount of time we can sleep in order
	// to maintain our desired frequency
	static unsigned long int last = platform.getExecutionTimeMicros();
	static unsigned long int sleepMicros = 1;

	unsigned long int now = platform.getExecutionTimeMicros();
	unsigned long int idealSleepTime = (
			SHIP_UPDATESPERSECOND != 0 ?
			1000000 / SHIP_UPDATESPERSECOND : 0
		);

	// adjust the target sleep micros by the factor we are off by
	sleepMicros = (sleepMicros > 0 ? sleepMicros : 1) * ((double) idealSleepTime / (double) (now - last));

	last = now;

	return sleepMicros;

}
