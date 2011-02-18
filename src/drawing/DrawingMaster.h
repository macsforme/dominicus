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
#include "core/GamePrefs.h"
#include "core/MainLoopMember.h"
#include "core/ProgramLog.h"
#include "core/SystemInfo.h"
#include "drawing/controlbox/DrawControlBox.h"
#include "drawing/cursor/DrawCursor.h"
#include "drawing/HUD/DrawHUD.h"
#include "drawing/rendering/RenderingMaster.h"
#include "input/KeyTrap.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "windowing/GameWindow.h"

// library headers
#include <sstream>
#include <SDL/SDL.h>

// global variables
extern FontManager fontManager;
extern GamePrefs gamePrefs;
extern SystemInfo systemInfo;
extern Platform platform;

extern GameWindow* gameWindow;

class DrawingMaster : public MainLoopMember {
private:
	FontManager* fontManager;

	DrawControlBox* drawControlBox;
	DrawCursor* drawCursor;
	DrawHUD* hudDrawing;

	KeyTrap* fpsCapKeyTrap;
	bool capFPS;

public:
	RenderingMaster* renderingMaster;

	DrawingMaster();
	~DrawingMaster();

	void setViewPortSize(unsigned short int width, unsigned short int height);
	void preFrame();
	void postFrame();

	unsigned long int loop();
};

#endif
