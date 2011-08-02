/*
 * Keyboard.cpp
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

#include "input/Keyboard.h"

KeyListener::KeyListener(std::vector<SDLKey> keys, bool trap) :
		listeningKeys(keys), trap(trap) { }

SDLKey KeyListener::popKey() {
	if(keyHits.size() == 0)
		return SDLK_UNKNOWN;

	SDLKey toReturn = keyHits[0];
	keyHits.erase(keyHits.begin());

	return toReturn;
}

KeyAbsoluteListener::KeyAbsoluteListener(SDLKey listeningKey, bool trap) :
		listeningKey(listeningKey), trap(trap), isDown(false) { }

void Keyboard::addListener(KeyListener* listener) {
	keyListeners.push_back(listener);
}

void Keyboard::removeListener(KeyListener* listener) {
	for(int i = (int) keyListeners.size() - 1; i >= 0; --i) {
		if(keyListeners[i] == listener) {
			keyListeners.erase(keyListeners.begin() + i);

			break;
		}
	}
}

void Keyboard::addListener(KeyAbsoluteListener* listener) {
	keyAbsoluteListeners.push_back(listener);
}

void Keyboard::removeListener(KeyAbsoluteListener* listener) {
	for(int i = (int) keyAbsoluteListeners.size() - 1; i >= 0; --i) {
		if(keyAbsoluteListeners[i] == listener) {
			keyAbsoluteListeners.erase(keyAbsoluteListeners.begin() + i);

			break;
		}
	}
}

void Keyboard::clearListeners() {
	// clear all listeners
	keyListeners.clear();
}

void Keyboard::execute() {
	// handle input events on our stack
	while(eventStack.size() > 0) {
		SDL_Event event = eventStack.front();

		switch(event.type) {
		case SDL_KEYDOWN:
		{
			// if we're listening for Unicode input text, and it's a usable character, record it
			if(listenUnicode && event.key.keysym.unicode >= 0x20 && event.key.keysym.unicode <= 0x7E) {
				char thisString[] = { (char) event.key.keysym.unicode, '\0' };
				unicodeChars += thisString;
			}

			// traverse the listener stacks
			bool checkStandardListeners = true;	// only not if the key is trapped by absolute listeners

			for(
					std::vector<KeyAbsoluteListener*>::reverse_iterator listenerItr = keyAbsoluteListeners.rbegin();
					listenerItr != keyAbsoluteListeners.rend();
					++listenerItr
				) {
				if((*listenerItr)->listeningKey == event.key.keysym.sym) {
					(*listenerItr)->isDown = true;

					if((*listenerItr)->trap) {
						checkStandardListeners = false;

						break;
					}
				}
			}

			if(! checkStandardListeners)
				break;

			for(
					std::vector<KeyListener*>::reverse_iterator listenerItr = keyListeners.rbegin();
					listenerItr != keyListeners.rend();
					++listenerItr
				) {
				bool trapped = false;

				for(
						std::vector<SDLKey>::iterator keyItr = (*listenerItr)->listeningKeys.begin();
						keyItr != (*listenerItr)->listeningKeys.end();
						++keyItr
					) {
					if(*keyItr == event.key.keysym.sym) {
						(*listenerItr)->keyHits.push_back(event.key.keysym.sym);

						if((*listenerItr)->trap)
							trapped = true;

						break;
					}
				}

				if(trapped)
					break;
			}

			break;
		}
		case SDL_KEYUP:
			// traverse the absolute listener stack only
			for(
					std::vector<KeyAbsoluteListener*>::reverse_iterator listenerItr = keyAbsoluteListeners.rbegin();
					listenerItr != keyAbsoluteListeners.rend();
					++listenerItr
				) {
				if((*listenerItr)->listeningKey == event.key.keysym.sym) {
					(*listenerItr)->isDown = false;

					if((*listenerItr)->trap)
						break;
				}
			}

			break;
		}

		eventStack.erase(eventStack.begin());
	}
}
