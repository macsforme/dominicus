/*
 *  DrawHUDEnclosure.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/8/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef DRAWHUDENCLOSURE_H
#define DRAWHUDENCLOSURE_H

// program headers
#include "drawing/HUD/DrawHUDContainerUtility.h"
#include "drawing/HUD/HUDArrangement.h"
#include "drawing/HUD/HUDElement.h"
#include "math/VectorMath.h"
#include "windowing/GameWindow.h"

// global variables
extern GameWindow* gameWindow;

class DrawHUDEnclosure {
private:
	HUDArrangement* hudArrangement;

	DrawHUDContainerUtility* containerUtility;

	HUDElement* findPriorElementCenter(int index);

public:
	DrawHUDEnclosure(HUDArrangement* hudArrangement);

	void draw();
};

#endif
