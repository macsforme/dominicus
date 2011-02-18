/*
 *  Ship.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "state/Ship.h"

Ship::Ship(Vector4 position) :
		position(position),
		speed(0.0f) {
	orientation.identity();
}

unsigned long int Ship::loop() {
	// get our delta T
	static float lastUpdate = (float)platform.getExecutionTimeMicros();
	float dt = platform.getExecutionTimeMicros() - lastUpdate;
	lastUpdate += dt;

	// update the position
	position += (Vector4(0.0f, 0.0f, 1.0f, 0.0f) * ((float) dt / 1000000.0f * speed)) * orientation;

	// calculate and return sleep time from superclass
	static const unsigned long int idealSleepTime = (
			gamePrefs.getInt("shipUpdateFrequency") != 0 ?
			1000000 / gamePrefs.getInt("shipUpdateFrequency") : 0
		);
	return getSleepTime(idealSleepTime);
}
