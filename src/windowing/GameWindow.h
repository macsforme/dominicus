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
#include "core/GamePrefs.h"
#include "core/ProgramLog.h"
#include "core/SystemInfo.h"
#include "platform/Platform.h"

// library headers
#include <SDL/SDL.h>

// global variables
extern GamePrefs* gamePrefs;
extern Platform* platform;
extern ProgramLog* programLog;
extern SystemInfo* systemInfo;

class GameWindow {
public:
	unsigned short int width, height;
	float aspectRatio;
	bool fullScreen;

	GameWindow(bool fullScreen);
};

#endif
