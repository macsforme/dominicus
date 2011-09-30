// DrawTexture.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef DRAWTEXTURE_H
#define DRAWTEXTURE_H

// program headers
#include "graphics/GameGraphics.h"
#include "graphics/texture/Texture.h"
#include "graphics/2dgraphics/ui/BaseUIElement.h"
#include "logic/UILayoutAuthority.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"

// library headers
#include <map>
#include <string>

// global variables
extern GameGraphics* gameGraphics;

class DrawTexture : public BaseUIElement {
public:
	DrawTexture();
	~DrawTexture();

	/*
		Arguments Layout
		----------------
		metrics			UI element metrics
		texture			texture filename
	*/

	Vector2 getSize(std::map<std::string, void*> arguments);
	void execute(std::map<std::string, void*> arguments);
};

#endif // DRAWTEXTURE_H
