// DrawSpot.h
// Crucible Island

#ifndef DRAWSPOT_H
#define DRAWSPOT_H

// program headers
#include "graphics/GameGraphics.h"
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"

// library headers
#include <map>
#include <string.h>
#include <vector>

// global variables
extern GameGraphics* gameGraphics;
extern Platform* platform;

class DrawSpot : public BaseDrawNode {
protected:
	struct VertexEntry {
		Vector2 position;
		Vector2 primCoord;
		Vector2 curveOriginCoord;
		bool highlight;
		bool concave;
	};

	// utility drawing functions
	void drawCurve(std::vector<VertexEntry>* quadVertices, Vector2 position, Vector2 size, float rotation,
			bool highlight = false, bool concave = false);

public:
	DrawSpot();

	/*
		Arguments Layout
		----------------
		size			width/height
		position		position
		softEdge		thickness of antialiasing of border in pixels
		insideColor		color of inside of container
		outsideColor	color of outside of container
	*/

	void execute(std::map<std::string, void*> newDrawData);
};

#endif // DRAWSPOT_H
