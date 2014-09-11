// DrawMissileIndicators.h
// Crucible Island

#ifndef DRAWMISSILEINDICATORS_H
#define DRAWMISSILEINDICATORS_H

#include <map>
#include <string>

#include "graphics/DrawTypes.h"
#include "graphics/2dgraphics/DrawRoundedTriangle.h"
#include "math/VectorMath.h"

class DrawMissileIndicators : public BaseUIElement {
private:
	DrawRoundedTriangle* roundedTriangleDrawer;

public:
	DrawMissileIndicators(DrawRoundedTriangle* newRoundedTriangleDrawer);
	~DrawMissileIndicators();

	DrawStackArgList instantiateArgList();
	void deleteArgList(DrawStackArgList argList);

	Vector2 getSize(DrawStackArgList argList) { return Vector2(2.0f, 2.0f); }

	void execute(DrawStackArgList argList);
};

#endif // DRAWMISSILEINDICATORS_H
