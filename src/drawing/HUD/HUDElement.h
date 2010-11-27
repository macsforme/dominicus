/*
 *  HUDElement.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/6/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef HUDELEMENT_H
#define HUDELEMENT_H

// program headers
#include "VectorMath.h"

class HUDElement {
public:
	Vector2 size, position;

	HUDElement(Vector2 size, Vector2 position = Vector2(0.0f, 0.0f)) :
			size(size), position(position) { }
};

#endif
