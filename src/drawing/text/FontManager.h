/*
 * FontManager.h
 * Dominicus
 *
 * Copyright 2010-2011, Joshua Bodine
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef FONTMANAGER_H
#define FONTMANAGER_H

// program headers
#include "core/GamePrefs.h"
#include "core/ProgramLog.h"
#include "drawing/texture/Texture.h"
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
extern GamePrefs* gamePrefs;
extern ProgramLog* programLog;

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
	void rebuildFontTextureCache(unsigned int size);

public:
	// object management
	FontManager();
	~FontManager();

	// glyph management
	void buildChar(const char character, unsigned int size);
	bool isCharCached(const char character, unsigned int size);
	std::vector<char> getCachedCharsList(unsigned int size);
	void populateCommonChars(unsigned int size);
};

#endif
