/*
 *  ShipControl.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "ShipControl.h"

ShipControl::ShipControl(GameWindow* gameWindow, Ship* ship) :
		gameWindow(gameWindow), ship(ship) {
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
			-((float) mouse.positionX / (float) gameWindow->width - 0.5f) *
			 gameWindow->aspectRatio / (gamePrefs.getFloat("controlBoxSize") / 2);
	float yMovement =
			((float) mouse.positionY / (float) gameWindow->height - 0.5f) /
			(gamePrefs.getFloat("controlBoxSize") / 2);

	if(xMovement > 1.0f) xMovement = 1.0f;
	if(xMovement < -1.0f) xMovement = -1.0f;
	if(yMovement > 1.0f) yMovement = 1.0f;
	if(yMovement < -1.0f) yMovement = -1.0f;

	rotateMatrix(
			zVec,
			radians(xMovement * SHIP_TURNPERSEC * (float) dt / 1000000.0f),
			ship->orientation
		);
	rotateMatrix(
			xVec,
			radians(yMovement * SHIP_TURNPERSEC * (float) dt / 1000000.0f),
			ship->orientation
		);

	if(keyboard.getKeyState("bankLeft"))
		rotateMatrix(zVec, radians(0.5f * SHIP_TURNPERSEC * (float) dt / 1000000.0f), ship->orientation);
	if(keyboard.getKeyState("bankRight"))
		rotateMatrix(zVec, radians(-0.5f * SHIP_TURNPERSEC * (float) dt / 1000000.0f), ship->orientation);
	if(keyboard.getKeyState("down"))
		rotateMatrix(xVec, radians(0.5f * SHIP_TURNPERSEC * (float) dt / 1000000.0f), ship->orientation);
	if(keyboard.getKeyState("up"))
		rotateMatrix(xVec, radians(-0.5f * SHIP_TURNPERSEC * (float) dt / 1000000.0f), ship->orientation);

	if(keyboard.getKeyState("speed+"))
		if(ship->speed < SHIP_MOVEPERSEC)
			ship->speed += SHIP_MOVEPERSEC * (float) dt / 1000000.0f;
	if(keyboard.getKeyState("speed-"))
		if(ship->speed > -SHIP_MOVEPERSEC)
			ship->speed -= SHIP_MOVEPERSEC * (float) dt / 1000000.0f;
}
