// DrawCircle.h
// Crucible Island

#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include "graphics/DrawTypes.h"
#include "graphics/2dgraphics/DrawContainer.h"

class DrawCircle : public DrawContainer {
public:
	DrawStackArgList instantiateArgList();
	void deleteArgList(DrawStackArgList argList);

	void execute(DrawStackArgList argList);
};

#endif // DRAWCIRCLE_H
