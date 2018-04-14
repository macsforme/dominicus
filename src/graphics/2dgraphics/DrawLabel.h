// DrawLabel.h
// Dominicus

#ifndef DRAWLABEL_H
#define DRAWLABEL_H

#include "graphics/DrawTypes.h"
#include "math/VectorMath.h"

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

	DrawStackArgList instantiateArgList();
	void deleteArgList(DrawStackArgList argList);

	Vector2 getSize(DrawStackArgList argList);

	void execute(DrawStackArgList argList);
};

#endif // DRAWLABEL_H
