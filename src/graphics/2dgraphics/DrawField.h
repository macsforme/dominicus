// DrawField.h
// Crucible Island

#ifndef DRAWFIELD_H
#define DRAWFIELD_H

// forward declarations
class DrawLabel;

// program headers
#include "graphics/GameGraphics.h"
#include "graphics/text/TextBlock.h"
#include "graphics/2dgraphics/DrawLabel.h"
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
		metrics			UI element metrics
		fontSize		Font point size for this label (not to be confused with metrics->size)
		fontColor		Font color
		text			std::string of text
		boxColor		Vector4 with box color
	*/

	Vector2 getSize(std::map<std::string, void*> arguments);
	void execute(std::map<std::string, void*> arguments);
};

#endif // DRAWFIELD_H
