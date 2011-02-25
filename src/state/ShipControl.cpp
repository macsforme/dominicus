/*
 * ShipControl.cpp
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

#include "state/ShipControl.h"

void ShipControl::loop() {
	// determine how much further we can rotate around the +X axis
	float displacePercent = degrees(asin(ship->orientation.m32)) /
			gamePrefs->getFloat("shipMaxAngle");

	// get our delta T
	static float lastUpdate = (float) platform->getExecMills();
	float dt = platform->getExecMills() - lastUpdate;
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
			gamePrefs->getFloat("controlSpotSize") / gamePrefs->getFloat("controlBoxSize")) {
		xMovement = posVec.x / (gamePrefs->getFloat("controlBoxSize"));
		yMovement =	-posVec.y /	(gamePrefs->getFloat("controlBoxSize"));
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
			(yMovement < 0.0f && displacePercent > 0.0f) ||
			(yMovement > 0.0f && displacePercent < 0.0f)
		)
		yMovement *= 1.0f - absolute(displacePercent);

	rotateMatrix(
			yVec,
			radians(xMovement * gamePrefs->getFloat("shipTurnRate") * (float) dt / 1000.0f),
			ship->orientation
		);
	rotateMatrix(
			xVec,
			radians(yMovement * gamePrefs->getFloat("shipTurnRate") * (float) dt / 1000.0f),
			ship->orientation
		);

	// ship velocity
	if(inputHandler->keyboard.getKeyState("speed+"))
		if(ship->speed < gamePrefs->getFloat("shipMaxAccel"))
			ship->speed += gamePrefs->getFloat("shipMaxAccel") * (float) dt / 1000.0f;
	if(inputHandler->keyboard.getKeyState("speed-"))
		if(ship->speed > -gamePrefs->getFloat("shipMaxAccel"))
			ship->speed -= gamePrefs->getFloat("shipMaxAccel") * (float) dt / 1000.0f;
}
