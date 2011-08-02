/*
 * WorldViewCamera.cpp
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

#include "drawing/rendering/cameras/WorldViewCamera.h"

void WorldViewCamera::loop() {
	// get our delta T
	static float lastUpdate = (float)platform->getExecMills();
	float dt = (float) platform->getExecMills() - lastUpdate;
	lastUpdate += dt;

	// do math on variables
	rotation += (float) dt / 1000.0f * rotationSpeed;
	if(rotation > 360.0f)
		rotation -= 360.0f;
	zoom = 1.0f + zoomFactor *
			cos((float) (platform->getExecMills() % 1000) / 1000.0f / zoomPeriod) / 2.0f;

	// re-initialize the matrix
	vpMatrix.identity();

	// do transformations
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(rotation), vpMatrix); // dynamic rotation
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-birdsEyeAngle), vpMatrix); // static tilt up
	translateMatrix(0.0f, 0.0f, 750.0f, vpMatrix); // z translation
	scaleMatrix(zoom, zoom, zoom, vpMatrix); // dynamic zoom

	// perspective projection
	const float fov = gamePrefs->getFloat("perspectiveFOV");
	const float nClip = gamePrefs->getFloat("perspectiveNearClip");
	const float fClip = gamePrefs->getFloat("perspectiveFarClip");

	vpMatrix *= Matrix4(
	        1.0f/tan(radians(fov)), 0.0, 0.0, 0.0,
	        0.0, aspectRatio/tan(radians(fov)), 0.0, 0.0,
	        0.0, 0.0, (fClip + nClip) / (fClip - nClip), 1.0f,
	        0.0, 0.0, -2.0f * fClip * nClip / (fClip - nClip), 0.0
		);
}
