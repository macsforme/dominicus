// DrawField.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef DRAWFIELD_H
#define DRAWFIELD_H

// forward declarations
class DrawLabel;

// program headers
#include "graphics/GameGraphics.h"
#include "graphics/text/TextBlock.h"
#include "graphics/2dgraphics/ui/BaseUIElement.h"
#include "graphics/2dgraphics/ui/components/DrawLabel.h"
#include "logic/UILayoutAuthority.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"

// library headers
#include <string>

// global variables
extern GameGraphics* gameGraphics;

class DrawField : public BaseUIElement {
private:
	DrawLabel* labelDrawer;

public:
	DrawField();
	~DrawField();

	/*
		Arguments Layout
		----------------
		mvpMatrix		Orthogonal projection matrix for shader
		metrics			UI element metrics
		fontSize		Font point size for this label (not to be confused with metrics->size)
		fontColor		Font color
		text			std::string of text
	*/

	Vector2 getSize(std::map<std::string, void*> arguments);
	void execute(std::map<std::string, void*> arguments);
};

#endif // DRAWFIELD_H
