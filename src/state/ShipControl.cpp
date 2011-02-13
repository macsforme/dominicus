/*
 *  ShipControl.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "ShipControl.h"

void ShipControl::loop() {
	// determine how much further we can rotate around the +X axis
	float displacePercent = degrees(asin(ship->orientation.m32)) /
			(float) gamePrefs.getInt("shipMaxAngle");

	// get our delta T
	static float lastUpdate = (float)platform.getExecutionTimeMicros();
	float dt = platform.getExecutionTimeMicros() - lastUpdate;
	lastUpdate += dt;

	// create directional vectors for the relative +Z and +X axises
	Vector3 xVec(
			ship->orientation.m11,
			ship->orientation.m12,
			ship->orientation.m13
		);
	Vector3 yVec(
			0.0f, 1.0f, 0.0f
		);

	// do rotations
	float xMovement = 0.0f, yMovement = 0.0f;

	// mouse steering, considering dead spot in the middle
	Vector2 zeroVec(0.0f, 0.0f);
	Vector2 posVec(
			inputHandler->mouse.position.x * gameWindow->aspectRatio,
			inputHandler->mouse.position.y
		);

	if(distance(zeroVec, posVec) >
			gamePrefs.getFloat("controlSpotSize") / gamePrefs.getFloat("controlBoxSize")) {
		xMovement = posVec.x / (gamePrefs.getFloat("controlBoxSize"));
		yMovement =	-posVec.y /	(gamePrefs.getFloat("controlBoxSize"));
	}

	// keyboard steering
	if(inputHandler->keyboard.getKeyState("bankLeft")) xMovement -= 0.5f;
	if(inputHandler->keyboard.getKeyState("bankRight")) xMovement += 0.5f;
	if(inputHandler->keyboard.getKeyState("down")) yMovement += 0.5f;
	if(inputHandler->keyboard.getKeyState("up")) yMovement -= 0.5f;

	if(xMovement > 1.0f) xMovement = 1.0f;
	if(xMovement < -1.0f) xMovement = -1.0f;
	if(yMovement > 1.0f) yMovement = 1.0f;
	if(yMovement < -1.0f) yMovement = -1.0f;

	// dampen the up/down rotation at the limits
	if(
			yMovement < 0.0f && displacePercent > 0.0f ||
			yMovement > 0.0f && displacePercent < 0.0f
		)
		yMovement *= 1.0f - absolute(displacePercent);

	rotateMatrix(
			yVec,
			radians(xMovement * (float) gamePrefs.getInt("shipTurnRate") * (float) dt / 1000000.0f),
			ship->orientation
		);
	rotateMatrix(
			xVec,
			radians(yMovement * (float) gamePrefs.getInt("shipTurnRate") * (float) dt / 1000000.0f),
			ship->orientation
		);

	// ship velocity
	if(inputHandler->keyboard.getKeyState("speed+"))
		if(ship->speed < (float) gamePrefs.getInt("shipMaxAccel"))
			ship->speed += (float) gamePrefs.getInt("shipMaxAccel") * (float) dt / 1000000.0f;
	if(inputHandler->keyboard.getKeyState("speed-"))
		if(ship->speed > -((float) gamePrefs.getInt("shipMaxAccel")))
			ship->speed -= (float) gamePrefs.getInt("shipMaxAccel") * (float) dt / 1000000.0f;
}
