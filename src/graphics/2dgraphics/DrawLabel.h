// DrawLabel.h
// Crucible Island

#ifndef DRAWLABEL_H
#define DRAWLABEL_H

// program headers
#include "graphics/GameGraphics.h"
#include "graphics/text/FontManager.h"
#include "graphics/text/TextBlock.h"
#include "logic/UILayoutAuthority.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"

// library headers
#include <map>
#include <string>

// global variables
extern GameGraphics* gameGraphics;

class DrawLabel : public BaseUIElement {
protected:
	struct VertexEntry {
		Vector2 position;
		Vector2 texCoord;
		Vector4 color;
	};

public:
	DrawLabel();
	~DrawLabel();

	/*
		Arguments Layout
		----------------
		metrics			UI element metrics
		fontSize		Font point size for this label (not to be confused with metrics->size)
		fontColor		Font color
		text			std::string of text
		wrap			float with portion of screen (on 2.0 scale) to wrap text to
	*/

	Vector2 getSize(std::map<std::string, void*> arguments);
	void execute(std::map<std::string, void*> arguments);
};

#endif // DRAWLABEL_H
