// DrawControlBox.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef DRAWCONTROLBOX_H
#define DRAWCONTROLBOX_H

// forward declarations
class GameGraphics;
class GameSystem;

// program headers
#include "core/GameSystem.h"
#include "graphics/BaseDrawNode.h"
#include "graphics/GameGraphics.h"
#include "graphics/texture/Texture.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"

// library headers
#include <map>
#include <sstream>
#include <string>

// global variables
extern GameGraphics* gameGraphics;
extern GameSystem* gameSystem;

class DrawControlBox : public BaseDrawNode {
public:
	DrawControlBox();
	~DrawControlBox();

	void execute(std::map<std::string, void*> arguments);
};

#endif // DRAWCONTROLBOX_H
