// DrawRoundedTriangle.h
// Crucible Island

#ifndef DRAWROUNDEDTRIANGLE_H
#define DRAWROUNDEDTRIANGLE_H

#include "graphics/DrawTypes.h"
#include "graphics/2dgraphics/DrawContainer.h"

class DrawRoundedTriangle : public DrawContainer {
public:
	DrawStackArgList instantiateArgList();
	void deleteArgList(DrawStackArgList argList);

	void execute(DrawStackArgList argList);
};

#endif // DRAWROUNDEDTRIANGLE_H
