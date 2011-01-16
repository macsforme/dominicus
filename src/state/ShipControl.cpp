/*
 *  ShipControl.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "ShipControl.h"

ShipControl::ShipControl(Ship* ship) : ship(ship) {
	;
}

void ShipControl::loop() {
	// get our delta T
	static float lastUpdate = platform.getExecutionTimeMicros();
	float dt = platform.getExecutionTimeMicros() - lastUpdate;
	lastUpdate += dt;

	// create directional vectors for the relative +Z and +X axises
	Vector3 zVec(
			ship->orientation.m31,
			ship->orientation.m32,
			ship->orientation.m33
		);
	Vector3 xVec(
			ship->orientation.m11,
			ship->orientation.m12,
			ship->orientation.m13
		);


	// do rotations
	float xMovement =
			-inputHandler->mouse.position.x * gameWindow->aspectRatio /
			(gamePrefs.getFloat("controlBoxSize"));
	float yMovement =
			-inputHandler->mouse.position.y /
			(gamePrefs.getFloat("controlBoxSize"));

	// we have a dead spot in the middle
	Vector2 zeroVec(0.0f, 0.0f);
	Vector2 posVec(xMovement, yMovement);

	if(distance(zeroVec, posVec) >
			gamePrefs.getFloat("controlSpotSize") / gamePrefs.getFloat("controlBoxSize")) {
		if(xMovement > 1.0f) xMovement = 1.0f;
		if(xMovement < -1.0f) xMovement = -1.0f;
		if(yMovement > 1.0f) yMovement = 1.0f;
		if(yMovement < -1.0f) yMovement = -1.0f;

		rotateMatrix(
				zVec,
				radians(xMovement * (float) gamePrefs.getInt("shipTurnRate") * (float) dt / 1000000.0f),
				ship->orientation
			);
		rotateMatrix(
				xVec,
				radians(yMovement * (float) gamePrefs.getInt("shipTurnRate") * (float) dt / 1000000.0f),
				ship->orientation
			);
	}

	if(inputHandler->keyboard.getKeyState("bankLeft"))
		rotateMatrix(zVec, radians(0.5f * (float) gamePrefs.getInt("shipTurnRate") *
				(float) dt / 1000000.0f), ship->orientation);
	if(inputHandler->keyboard.getKeyState("bankRight"))
		rotateMatrix(zVec, radians(-0.5f * (float) gamePrefs.getInt("shipTurnRate") *
				(float) dt / 1000000.0f), ship->orientation);
	if(inputHandler->keyboard.getKeyState("down"))
		rotateMatrix(xVec, radians(0.5f * (float) gamePrefs.getInt("shipTurnRate") *
				(float) dt / 1000000.0f), ship->orientation);
	if(inputHandler->keyboard.getKeyState("up"))
		rotateMatrix(xVec, radians(-0.5f * (float) gamePrefs.getInt("shipTurnRate") *
				(float) dt / 1000000.0f), ship->orientation);

	if(inputHandler->keyboard.getKeyState("speed+"))
		if(ship->speed < (float) gamePrefs.getInt("shipMaxAccel"))
			ship->speed += (float) gamePrefs.getInt("shipMaxAccel") * (float) dt / 1000000.0f;
	if(inputHandler->keyboard.getKeyState("speed-"))
		if(ship->speed > -((float) gamePrefs.getInt("shipMaxAccel")))
			ship->speed -= (float) gamePrefs.getInt("shipMaxAccel") * (float) dt / 1000000.0f;
}
