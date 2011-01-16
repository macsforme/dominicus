/*
 *  HUDArrangement.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/6/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef HUDARRANGEMENT_H
#define HUDARRANGEMENT_H

// program headers
#include "GameWindow.h"
#include "HUDElement.h"
#include "VectorMath.h"

// library headers
#include <vector>

// global variables
extern GameWindow* gameWindow;

class HUDArrangement {
public:
	enum Bearing {
		BEARING_NONE,
		BEARING_LEFT,
		BEARING_RIGHT,
		BEARING_BOTTOM,
		BEARING_TOP
	};

	struct HUDElementEntry {
		HUDElement* element;
		Bearing bearing1;
		Bearing bearing2;
	};

	std::vector<HUDElementEntry> elementList;

private:
	HUDElement* findPriorElementCenter(int index);

	Vector2 elementPadding;

public:
	HUDArrangement();

	void rearrange();
};

#endif
