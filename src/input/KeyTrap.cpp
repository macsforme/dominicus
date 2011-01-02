/*
 *  KeyTrap.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/5/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "KeyTrap.h"

KeyTrap::KeyTrap(std::string action) : action(action) {
	alreadyPressed = false;
}

void KeyTrap::loop() {
	if(! inputHandler.keyboard.getKeyState(action))
		alreadyPressed = false;
}

bool KeyTrap::newPress() {
	if(! alreadyPressed) {
		if(inputHandler.keyboard.getKeyState(action)) {
			alreadyPressed = true;

			return true;
		}
	}

	return false;
}
