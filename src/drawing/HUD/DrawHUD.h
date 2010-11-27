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
#include "DrawConsole.h"
#include "DrawInfoBox.h"
#include "DrawHUDEnclosure.h"
#include "FontManager.h"
#include "HUDArrangement.h"
#include "HUDElement.h"
#include "Platform.h"
#include "Screen.h"
#include "VectorMath.h"

// library headers
#include <vector>

// global variables
extern Platform platform;

class DrawHUD {
private:
	HUDArrangement* hudArrangement;
	DrawHUDEnclosure* drawHUDEnclosure;

	// HUD element drawing handlers
	DrawConsole* consoleDrawer;
	DrawInfoBox* infoBoxDrawer;

public:
	DrawHUD(Screen* screen, FontManager* fontManager);
	~DrawHUD();

	void draw();
};

#endif
