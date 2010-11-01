/*
 *  Texture.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 6/9/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef TEXTURE_H
#define TEXTURE_H

// program headers
#include "ProgramLog.h"

// library headers
#include <sstream>

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
