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
#include "core/ProgramLog.h"
#include "drawing/texture/Texture.h"

// library headers
#include <cstring>
#include <fstream>
#include <stdint.h>
#include <sstream>
#include <string>

class BMPImage : public Texture {
public:
	BMPImage(std::string filename);
};

#endif
