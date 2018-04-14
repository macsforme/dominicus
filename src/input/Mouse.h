// Mouse.h
// Dominicus

#ifndef MOUSE_H
#define MOUSE_H

#include <SDL/SDL.h>
#include <stdint.h>
#include <vector>

#include "math/VectorMath.h"

class MouseButtonListener {
public:
	uint8_t listeningButton;
	Vector2 ll;
	Vector2 ur;
	bool trap;

	bool buttonHit;

	MouseButtonListener(
			uint8_t listeningButton = SDL_BUTTON_LEFT,
			Vector2 ll = Vector2(-1.0f, -1.0f),
			Vector2 ur = Vector2(1.0f, 1.0f),
			bool trap = false
		) :
			listeningButton(listeningButton),
			ll(ll),
			ur(ur),
			trap(trap),
			buttonHit(false) { }

	bool wasClicked();
};

class MouseMotionListener {
public:
	bool trap;

	bool motion;

	MouseMotionListener(bool trap = false) : trap(trap), motion(false) { }

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
		) :
			ll(ll),
			ur(ur),
			trap(trap),
			isEntered(false),
			wasChange(false) { }

	bool wasChanged();
};

class Mouse {
public:
	std::vector<SDL_Event> eventStack;

	Vector2 position;
	bool isInWindow;

	std::vector<MouseButtonListener*> mouseButtonListeners;
	std::vector<MouseMotionListener*> mouseMotionListeners;
	std::vector<MouseZoneListener*> mouseZoneListeners;

	Mouse() : position(Vector2(0.0f, 0.0f)), isInWindow(false) { };

	void addListener(MouseButtonListener* listener) { mouseButtonListeners.push_back(listener); }
	void removeListener(MouseButtonListener*);

	void addListener(MouseMotionListener* listener) { mouseMotionListeners.push_back(listener); }
	void removeListener(MouseMotionListener*);

	void addListener(MouseZoneListener* listener) { mouseZoneListeners.push_back(listener); }
	void removeListener(MouseZoneListener*);

	void clearListeners();

	void execute();
};

#endif // MOUSE_H
