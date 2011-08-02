/*
 * Keyboard.h
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

#ifndef KEYBOARD_H
#define KEYBOARD_H

// program headers
#include "GameSystem.h"

// library headers
#include <map>
#include <SDL/SDL.h>
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

#endif
