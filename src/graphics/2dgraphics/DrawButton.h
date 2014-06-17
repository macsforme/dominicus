// DrawButton.h
// Crucible Island

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
		padding			width/height of curved edges in pixels
		border			thickness of border in pixels
		softEdge		thickness of antialiasing of border in pixels
		insideColor		color of inside of container
		borderColor		color of border of container
		outsideCOlor	color of outside of container
		fontSize		Font point size for this label (not to be confused with metrics->size)
		fontColor		Font color
		text			std::string of text
	*/

	Vector2 getSize(std::map<std::string, void*> arguments);
	void execute(std::map<std::string, void*> arguments);
};

#endif // DRAWBUTTON_H
