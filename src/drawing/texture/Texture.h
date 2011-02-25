/*
 * Texture.h
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

#ifndef TEXTURE_H
#define TEXTURE_H

// program headers
#include "core/ProgramLog.h"

// library headers
#include <sstream>
#include <stdint.h>

// global variables
extern ProgramLog* programLog;

class Texture {
public:
	enum PixelFormat {
			FORMAT_RGB,
			FORMAT_RGBA
		};

protected:
	uint32_t width, height;
	PixelFormat format;

	void* pixelData;

	Texture();	// so our subclasses can use this class

public:
	Texture(uint32_t newWidth, uint32_t newHeight, PixelFormat newFormat);
	~Texture();

	uint32_t getWidth() { return width; }
	uint32_t getHeight() { return height; }
	PixelFormat getFormat() { return format; }

	void* getDataPointer() { return pixelData; }

	uint8_t getRedValueAt(uint32_t column, uint32_t row);
	uint8_t getGreenValueAt(uint32_t column, uint32_t row);
	uint8_t getBlueValueAt(uint32_t column, uint32_t row);
	uint8_t getAlphaValueAt(uint32_t column, uint32_t row);
	void setColorAt(uint32_t column, uint32_t row, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
};

#endif
