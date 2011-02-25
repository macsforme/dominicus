/*
 *  DrawHUD.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/7/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef DRAWHUD_H
#define DRAWHUD_H

// program headers
#include "drawing/HUD/console/DrawConsole.h"
#include "drawing/HUD/infobox/DrawInfoBox.h"
#include "drawing/HUD/DrawHUDEnclosure.h"
#include "drawing/HUD/HUDArrangement.h"
#include "drawing/HUD/HUDElement.h"
#include "drawing/text/FontManager.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "windowing/GameWindow.h"

// library headers
#include <vector>

// global variables
extern GameWindow* gameWindow;
extern Platform* platform;

class DrawHUD {
private:
	HUDArrangement* hudArrangement;
	DrawHUDEnclosure* drawHUDEnclosure;

	// HUD element drawing handlers
	DrawConsole* consoleDrawer;
	DrawInfoBox* infoBoxDrawer;

public:
	DrawHUD(FontManager* fontManager);
	~DrawHUD();

	void draw();
};

#endif
