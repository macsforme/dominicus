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
