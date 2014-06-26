// DrawStrikeEffect.h
// crucible island

#ifndef DRAWSTRIKEEFFECT_H
#define DRAWSTRIKEEFFECT_H

#include "graphics/DrawTypes.h"
#include "graphics/2dgraphics/DrawGrayOut.h"
#include "math/VectorMath.h"

class DrawStrikeEffect : public DrawGrayOut {
public:
	DrawStackArgList instantiateArgList() { return DrawStackArgList(); }
	void deleteArgList(DrawStackArgList argList) { }

	void execute(DrawStackArgList argList);
};

#endif // DRAWSTRIKEEFFECT_H
