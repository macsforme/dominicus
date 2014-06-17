// FontManager.h
// Crucible Island

#ifndef FONTMANAGER_H
#define FONTMANAGER_H

// program headers
#include "core/GameSystem.h"
#include "graphics/texture/Texture.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"

// library headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

// global variables
extern Platform* platform;
extern GameSystem* gameSystem;

class FontManager {
public:
	struct FontData {
		// pixel dimensions
		unsigned int width, height;

		// metrics
		int adjustX, adjustY;
		unsigned int advanceX;

		// texture coordinates for texture cache
		float sX, eX;
		float sY, eY;

		// individual font bitmap image
		Texture* bitmap;
	};

	// map of [character][size] font data
	std::map< char, std::map<unsigned int, FontData> > fontData;
	std::map<unsigned int, unsigned int> lineHeights;

	// OpenGL texture IDs for font caches of each size
	std::map<unsigned int, GLuint> textureIDs;

private:
	// internal freetype objects
	FT_Library library;
	FT_Face fontFace;

	// texture representations of OpenGL font caches
	std::map<unsigned int, Texture*> fontTextureCaches;

	// internal functions
	float positiveNormalize(unsigned int coordinate, unsigned int scale);

public:
	// object management
	FontManager();
	~FontManager();

	// glyph management
	void rebuildFontTextureCache(unsigned int size);
	void buildChar(const char character, unsigned int size, bool rebuildCache = true);
	bool isCharCached(const char character, unsigned int size);
	std::vector<char> getCachedCharsList(unsigned int size);
	void populateCommonChars(unsigned int size);
};

#endif // FONTMANAGER_H
