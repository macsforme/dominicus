/*
 *  GameWindow.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

// program headers
#include "GamePrefs.h"
#include "ProgramLog.h"
#include "SystemInfo.h"

// library headers
#include <SDL/SDL.h>

// global variables
extern SystemInfo systemInfo;
extern GamePrefs gamePrefs;

class GameWindow {
public:
	unsigned short int width, height;
	float aspectRatio;
	bool fullScreen;

	GameWindow(bool fullScreen);
};

#endif
