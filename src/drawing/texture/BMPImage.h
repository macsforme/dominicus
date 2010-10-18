/*
 *  BMPImage.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/30/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef BMPIMAGE_H
#define BMPIMAGE_H

// program headers
#include "ProgramLog.h"
#include "Texture.h"

// library headers
#include <cstring>
#include <fstream>
#include <stdint.h>
#include <sstream>

class BMPImage : public Texture {
public:
	BMPImage(const char* filename);
};

#endif


