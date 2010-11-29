/*
 *  Keyboard.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

// program headers
#include "GamePrefs.h"
#include "Platform.h"

// library headers
#include <map>
#include <SDL/SDL.h>
#include <sstream>
#include <string>

// global variables
extern GamePrefs gamePrefs;
extern Platform platform;

class Keyboard {
private:
	std::map<std::string, bool> keyStates;

public:
	bool getKeyState(std::string key);

	unsigned long int loop();
};

#endif
