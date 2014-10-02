// Keyboard.h
// Crucible Island

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL.h>
#include <string>
#include <vector>

class KeyListener {
public:
	std::vector<SDLKey> listeningKeys;
	bool trap;

	std::vector<SDLKey> keyHits;

	KeyListener(std::vector<SDLKey> keys, bool trap = false) : listeningKeys(keys), trap(trap) { }

	SDLKey popKey();
};

class KeyAbsoluteListener {
public:
	SDLKey listeningKey;
	bool trap;

	bool isDown;

	KeyAbsoluteListener(SDLKey listeningKey, bool trap = false) : listeningKey(listeningKey), trap(trap), isDown(false) { }
};

class Keyboard {
public:
	bool listenUnicode;
	std::string unicodeChars;

	std::vector<SDL_Event> eventStack;

	std::vector<KeyListener*> keyListeners;
	std::vector<KeyAbsoluteListener*> keyAbsoluteListeners;

	Keyboard() : listenUnicode(false) { }

	void addListener(KeyListener* listener) { keyListeners.push_back(listener); }
	void removeListener(KeyListener* listener);

	void addListener(KeyAbsoluteListener* listener) { keyAbsoluteListeners.push_back(listener); }
	void removeListener(KeyAbsoluteListener* listener);

	void clearListeners() { keyListeners.clear(); keyAbsoluteListeners.clear(); }

	void execute();
};

#endif // KEYBOARD_H
