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
	unsigned int width, height;
	PixelFormat format;

	void* pixelData;

	Texture();	// so our subclasses can use this class

public:
	Texture(unsigned int newWidth, unsigned int newHeight, PixelFormat newFormat);
	~Texture();

	unsigned int getWidth() { return width; }
	unsigned int getHeight() { return height; }
	PixelFormat getFormat() { return format; }

	void* getDataPointer() { return pixelData; }

	unsigned short int getRedValueAt(unsigned int column, unsigned int row);
	unsigned short int getGreenValueAt(unsigned int column, unsigned int row);
	unsigned short int getBlueValueAt(unsigned int column, unsigned int row);
	unsigned short int getAlphaValueAt(unsigned int column, unsigned int row);
	void setColorAt(unsigned int column, unsigned int row, unsigned short int red, unsigned short int green, unsigned short int blue, unsigned short int alpha);
};

#endif


