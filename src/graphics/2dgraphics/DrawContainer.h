// DrawContainer.h
// Dominicus

#ifndef DRAWCONTAINER_H
#define DRAWCONTAINER_H

#include <vector>

#include "graphics/DrawTypes.h"
#include "math/VectorMath.h"

class DrawContainer : public BaseUIElement {
protected:
	struct VertexEntry {
		Vector2 position;
		Vector2 primCoord;
		Vector2 curveOriginCoord;
		bool highlight;
		bool concave;
	};

	void drawCurve(std::vector<VertexEntry>* quadVertices, Vector2 position, Vector2 size, float rotation,
			bool highlight = false, bool concave = false);
	void drawBorder(std::vector<VertexEntry>* quadVertices, Vector2 position, Vector2 size, float rotation,
			bool highlight = false);
	void drawFiller(std::vector<VertexEntry>* quadVertices, Vector2 position, Vector2 size,
			bool highlight = false);

public:
	DrawContainer();
	~DrawContainer();

	DrawStackArgList instantiateArgList();
	void deleteArgList(DrawStackArgList argList);

	Vector2 getSize(DrawStackArgList argList) { return *((Vector2*) argList["size"]); }

	void execute(DrawStackArgList argList);
};

#endif // DRAWCONTAINER_H
