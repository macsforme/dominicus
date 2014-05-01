// DrawSplash.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef DRAWSPLASH_H
#define DRAWSPLASH_H

// forward declarations
class GameGraphics;

// program headers
#include "graphics/GameGraphics.h"
#include "graphics/texture/Texture.h"
#include "math/MatrixMath.h"
#include "platform/OpenGLHeaders.h"

// library headers
#include <map>
#include <sstream>
#include <string>

// global variables
extern GameGraphics* gameGraphics;

class DrawSplash : public BaseDrawNode {
public:
	DrawSplash();
	~DrawSplash();

	void execute(std::map<std::string, void*> arguments);
};

#endif // DRAWSPLASH_H
