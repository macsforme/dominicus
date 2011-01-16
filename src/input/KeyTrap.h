/*
 *  KeyTrap.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/5/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef KEYTRAP_H
#define KEYTRAP_H

// program headers
#include "GamePrefs.h"
#include "InputHandler.h"

// library headers
#include <string>

// global variables
extern GamePrefs gamePrefs;
extern InputHandler* inputHandler;

class KeyTrap {
private:
	std::string action;

	bool alreadyPressed;

public:
	KeyTrap(std::string action);

	void loop();

	bool newPress();
};

#endif
