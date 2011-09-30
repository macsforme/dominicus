// TextBlock.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef FONTBLOCK_H
#define FONTBLOCK_H

// program headers
#include "graphics/text/FontManager.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"

class TextBlock {
public:
	struct CharEntry {
		char character;
		float sX, eX, sY, eY;
		Vector4 color;
	};

	unsigned short int originX, originY;
	unsigned short int width, height;
	std::vector<CharEntry> entries;

	TextBlock(const char* dataString, unsigned int wrapX, unsigned int wrapY,
			FontManager* fontManager, unsigned int size);
};

#endif
