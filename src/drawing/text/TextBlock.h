/*
 *  TextBlock.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/22/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef FONTBLOCK_H
#define FONTBLOCK_H

// program headers
#include "FontManager.h"
#include "ScalarMath.h"

class TextBlock {
public:
	struct CharEntry {
		char character;
		float sX, eX, sY, eY;
	};

	unsigned int width, height;
	std::vector<CharEntry> entries;

	TextBlock(const char* dataString, unsigned int wrapX, unsigned int wrapY,
			FontManager* fontManager, unsigned int size);
};

#endif
