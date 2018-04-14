// TextBlock.h
// Dominicus

#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include <vector>

#include "graphics/text/FontManager.h"
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

#endif // TEXTBLOCK_H
