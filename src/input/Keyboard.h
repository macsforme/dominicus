// Keyboard.h
// Crucible Island

#ifndef KEYBOARD_H
#define KEYBOARD_H

// program headers
#include "core/GameSystem.h"

// library headers
#include <map>
#include <SDL.h>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>

// global variables
extern GameSystem* gameSystem;

class KeyListener {
public:
	std::vector<SDLKey> listeningKeys;
	bool trap;

	std::vector<SDLKey> keyHits;

	KeyListener(std::vector<SDLKey> keys, bool trap = false);

	SDLKey popKey();
};

class KeyAbsoluteListener {
public:
	SDLKey listeningKey;
	bool trap;

	bool isDown;

	KeyAbsoluteListener(SDLKey listeningKey, bool trap = false);
};

class Keyboard {
public:
	bool listenUnicode;
	std::string unicodeChars;

	std::vector<SDL_Event> eventStack;

	std::vector<KeyListener*> keyListeners;
	std::vector<KeyAbsoluteListener*> keyAbsoluteListeners;

	void addListener(KeyListener* listener);
	void removeListener(KeyListener* listener);

	void addListener(KeyAbsoluteListener* listener);
	void removeListener(KeyAbsoluteListener* listener);

	Keyboard() : listenUnicode(false) { }

	void clearListeners();

	void execute();
};

#endif // KEYBOARD_H
