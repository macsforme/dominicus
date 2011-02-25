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
#include "core/GamePrefs.h"
#include "platform/Platform.h"

// library headers
#include <map>
#include <SDL/SDL.h>
#include <sstream>
#include <string>
#include <vector>

// global variables
extern GamePrefs* gamePrefs;
extern Platform* platform;
extern ProgramLog* programLog;

class Keyboard {
private:
	std::map<std::string, bool> keyStates;

public:
	std::vector<SDL_Event> eventStack;

	bool getKeyState(std::string key);

	void loop();
};

#endif
