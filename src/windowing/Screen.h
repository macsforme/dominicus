/*
 *  Screen.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef SCREEN_H
#define SCREEN_H

// program headers
#include "GamePrefs.h"
#include "ProgramLog.h"
#include "SystemInfo.h"

// library headers
#include <map>
#include <OpenGL/OpenGL.h>
#include <SDL/SDL.h>

// global variables
extern SystemInfo systemInfo;
extern GamePrefs gamePrefs;

class Screen {
public:
	unsigned short int width, height;
	float viewRatio;
	bool fullScreen;

	Screen(bool fullScreen);
};

#endif
