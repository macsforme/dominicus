// DrawButton.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef DRAWBUTTON_H
#define DRAWBUTTON_H

// forward declarations
class DrawContainer;
class DrawLabel;

// program headers
#include "graphics/GameGraphics.h"
#include "graphics/2dgraphics/DrawContainer.h"
#include "graphics/2dgraphics/DrawLabel.h"
#include "logic/UILayoutAuthority.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"

// global variables
extern GameGraphics* gameGraphics;

class DrawButton : public BaseUIElement {
private:
	DrawContainer* containerDrawer;
	DrawLabel* labelDrawer;

public:
	DrawButton();
	~DrawButton();

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

#endif // DRAWBUTTON_H
