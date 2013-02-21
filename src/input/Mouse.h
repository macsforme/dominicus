// Mouse.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef MOUSE_H
#define MOUSE_H

// program headers
#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "math/VectorMath.h"

// library headers
#include <iostream>
#include <map>
#include <SDL.h>
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
