/*
 * Mouse.h
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

#ifndef MOUSE_H
#define MOUSE_H

// program headers
#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "math/VectorMath.h"

// library headers
#include <iostream>
#include <map>
#include <SDL/SDL.h>
#include <stdint.h>
#include <vector>

// global variables
extern GameGraphics* gameGraphics;
extern GameSystem* gameSystem;

class MouseButtonListener {
public:
	Uint8 listeningButton;
	Vector2 ll;
	Vector2 ur;
	bool trap;

	bool buttonHit;

	MouseButtonListener(
			Uint8 button = SDL_BUTTON_LEFT,
			Vector2 ll = Vector2(-1.0f, -1.0f),
			Vector2 ur = Vector2(1.0f, 1.0f),
			bool trap = false
		);

	bool wasClicked();
};

class MouseMotionListener {
public:
	bool trap;

	bool motion;

	MouseMotionListener(bool trap = false);

	bool wasMoved();
};

class MouseZoneListener {
public:
	Vector2 ll;
	Vector2 ur;
	bool trap;

	bool isEntered;

	bool wasChange;

	MouseZoneListener(
			Vector2 ll = Vector2(-1.0f, -1.0f),
			Vector2 ur = Vector2(1.0f, 1.0f),
			bool trap = false
		);

	bool wasChanged();
};

class Mouse {
public:
	std::vector<SDL_Event> eventStack;

	Vector2 position;

	std::vector<MouseButtonListener*> mouseButtonListeners;
	std::vector<MouseMotionListener*> mouseMotionListeners;
	std::vector<MouseZoneListener*> mouseZoneListeners;

	Mouse() : position(Vector2(0.0f, 0.0f)) { };

	void addListener(MouseButtonListener*);
	void removeListener(MouseButtonListener*);

	void addListener(MouseMotionListener*);
	void removeListener(MouseMotionListener*);

	void addListener(MouseZoneListener*);
	void removeListener(MouseZoneListener*);

	void clearListeners();

	void execute();
};

#endif
