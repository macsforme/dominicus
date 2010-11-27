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
#include "DrawHUDContainerUtility.h"
#include "HUDArrangement.h"
#include "HUDElement.h"
#include "Screen.h"
#include "VectorMath.h"

class DrawHUDEnclosure {
private:
	Screen* screen;

	HUDArrangement* hudArrangement;

	DrawHUDContainerUtility* containerUtility;

	HUDElement* findPriorElementCenter(int index);

public:
	DrawHUDEnclosure(HUDArrangement* hudArrangement, Screen* screen);

	void draw();
};

#endif
