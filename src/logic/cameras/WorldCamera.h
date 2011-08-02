/*
 * WorldViewCamera.h
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

#ifndef WORLDVIEWCAMERA_H
#define WORLDVIEWCAMERA_H

// program headers
#include "core/GamePrefs.h"
#include "drawing/rendering/cameras/Camera.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "windowing/GameWindow.h"

// constants
const float birdsEyeAngle = 30.0f; // degrees from horizon
const float rotationSpeed = 30.0f; // degrees per second
const float zoomFactor = 0.25f;	// total zoom distance
const float zoomPeriod = 1.0f;	// time in seconds to complete a zoom cycle

// global variables
extern GamePrefs* gamePrefs;
extern GameWindow* gameWindow;
extern Platform* platform;

class WorldViewCamera : Camera {
private:
	float rotation;
	float zoom;

	float aspectRatio;

public:
	Matrix4 vpMatrix;

	WorldViewCamera() :
			rotation(0.0f), zoom(0.0f), aspectRatio(gameWindow->aspectRatio) { }

	void loop();
};

#endif
