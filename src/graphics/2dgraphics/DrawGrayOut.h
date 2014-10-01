// DrawGrayOut.h
// Crucible Island

#ifndef DRAWGRAYOUT_H
#define DRAWGRAYOUT_H

#include "graphics/DrawTypes.h"
#include "math/VectorMath.h"

class DrawGrayOut : public BaseUIElement {
public:
	DrawGrayOut();
	~DrawGrayOut();

	DrawStackArgList instantiateArgList();
	void deleteArgList(DrawStackArgList argList);

	Vector2 getSize(DrawStackArgList argList) { return Vector2(2.0f, 2.0f); }

	void execute(DrawStackArgList argList);
};

#endif // DRAWGRAYOUT_H
