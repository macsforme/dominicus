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
	Screen* screen;
	Vector2 elementPadding;

	HUDArrangement hudArrangement;

	HUDElement* consoleHUDElement;
	HUDElement* rearViewMirrorHUDElement;
	HUDElement* playerInfoHUDElement;
	HUDElement* radarElement;

	HUDElement* findPriorElementCenter(int index);

public:
	DrawHUD(Screen* screen);
	
	void draw();
};

#endif
