/*
 * Mouse.cpp
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

#include "input/Mouse.h"

MouseButtonListener::MouseButtonListener(Uint8 button, Vector2 ll, Vector2 ur, bool trap) :
		listeningButton(button), ll(ll), ur(ur), trap(trap), buttonHit(false) { }

bool MouseButtonListener::wasClicked() {
	if(! buttonHit)
		return false;

	buttonHit = false;

	return true;
}

MouseMotionListener::MouseMotionListener(bool trap) : trap(trap), motion(false) { }

bool MouseMotionListener::wasMoved() {
	if(! motion)
		return false;

	motion = false;

	return true;
}

MouseZoneListener::MouseZoneListener(Vector2 ll, Vector2 ur, bool trap) :
		ll(ll), ur(ur), trap(trap), isEntered(false), wasChange(false) { }

bool MouseZoneListener::wasChanged() {
	if(! wasChange)
		return false;

	wasChange = false;

	return true;
}

void Mouse::addListener(MouseButtonListener* listener) {
	mouseButtonListeners.push_back(listener);
}

void Mouse::removeListener(MouseButtonListener* listener) {
	for(int i = (int) mouseButtonListeners.size() - 1; i >= 0; --i) {
		if(mouseButtonListeners[i] == listener) {
			mouseButtonListeners.erase(mouseButtonListeners.begin() + i);

			break;
		}
	}
}

void Mouse::addListener(MouseMotionListener* listener) {
	mouseMotionListeners.push_back(listener);
}

void Mouse::removeListener(MouseMotionListener* listener) {
	for(int i = (int) mouseMotionListeners.size() - 1; i >= 0; --i) {
		if(mouseMotionListeners[i] == listener) {
			mouseMotionListeners.erase(mouseMotionListeners.begin() + i);

			break;
		}
	}
}

void Mouse::addListener(MouseZoneListener* listener) {
	mouseZoneListeners.push_back(listener);
}

void Mouse::removeListener(MouseZoneListener* listener) {
	for(int i = (int) mouseZoneListeners.size() - 1; i >= 0; --i) {
		if(mouseZoneListeners[i] == listener) {
			mouseZoneListeners.erase(mouseZoneListeners.begin() + i);

			break;
		}
	}
}

void Mouse::clearListeners() {
	// clear all listeners
	mouseButtonListeners.clear();
	mouseMotionListeners.clear();
	mouseZoneListeners.clear();
}

void Mouse::execute() {
	// handle input events on our stack
	while(eventStack.size() > 0) {
		SDL_Event event = eventStack.front();

		switch(event.type) {
		case SDL_MOUSEMOTION:
			position.x = (float) event.motion.x / (float) (gameGraphics->resolutionX - 1) * 2.0f - 1.0f;
			position.y = (float) -event.motion.y / (float) (gameGraphics->resolutionY - 1) * 2.0f + 1.0f;

			// traverse the motion listener stack
			for(
					std::vector<MouseMotionListener*>::reverse_iterator listenerItr = mouseMotionListeners.rbegin();
					listenerItr != mouseMotionListeners.rend();
					++listenerItr
				) {
				(*listenerItr)->motion = true;

				if((*listenerItr)->trap)
					break;
			}

			// traverse the zone listener stack
			for(
					std::vector<MouseZoneListener*>::reverse_iterator listenerItr = mouseZoneListeners.rbegin();
					listenerItr != mouseZoneListeners.rend();
					++listenerItr
				) {
				if(
						(*listenerItr)->isEntered &&
						(position.x < (*listenerItr)->ll.x || position.y < (*listenerItr)->ll.y ||
							position.x > (*listenerItr)->ur.x || position.y > (*listenerItr)->ur.y)
					) {
					(*listenerItr)->isEntered = false;
					(*listenerItr)->wasChange = true;
				} else if(
						! (*listenerItr)->isEntered &&
						position.x >= (*listenerItr)->ll.x && position.y >= (*listenerItr)->ll.y &&
						position.x <= (*listenerItr)->ur.x && position.y <= (*listenerItr)->ur.y
					) {
					(*listenerItr)->isEntered = true;
					(*listenerItr)->wasChange = true;
				}

				if((*listenerItr)->trap)
					break;
			}

			break;
		case SDL_MOUSEBUTTONDOWN:
			// traverse the listener stack
			for(
					std::vector<MouseButtonListener*>::reverse_iterator listenerItr = mouseButtonListeners.rbegin();
					listenerItr != mouseButtonListeners.rend();
					++listenerItr
				) {
				if(
						(*listenerItr)->listeningButton == event.button.button &&
						position.x >= (*listenerItr)->ll.x && position.y >= (*listenerItr)->ll.y &&
						position.x <= (*listenerItr)->ur.x && position.y <= (*listenerItr)->ur.y
					) {
					(*listenerItr)->buttonHit = true;

					if((*listenerItr)->trap)
						break;
				}
			}

			break;
		}

		eventStack.erase(eventStack.begin());
	}
}
