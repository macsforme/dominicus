/*
 *  DrawingMaster.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 7/7/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef DRAWINGMASTER_H
#define DRAWINGMASTER_H

// program headers
#include "DrawCursor.h"
#include "DrawHUD.h"
#include "GamePrefs.h"
#include "KeyTrap.h"
#include "Platform.h"
#include "ProgramLog.h"
#include "RenderingMaster.h"
#include "Screen.h"
//#include "SplashBackground.h"
#include "SystemInfo.h"

// library headers
#include <sstream>
#include <SDL/SDL.h>
#include <OpenGL/OpenGL.h>

// global variables
extern FontManager fontManager;
extern SystemInfo systemInfo;
extern GamePrefs gamePrefs;
extern Platform platform;

class DrawingMaster {
private:
	RenderingMaster* renderingMaster;
	FontManager* fontManager;

//	SplashBackground* splashBackground;
	DrawCursor* drawCursor;
	DrawHUD* hudDrawing;

	KeyTrap* fpsCapKeyTrap;
	bool capFPS;

public:
	DrawingMaster(Screen* screen);
	~DrawingMaster();

	void setViewPortSize(unsigned short int width, unsigned short int height);

	void preFrame();
	void postFrame();

	unsigned long int loop();
};

#endif
