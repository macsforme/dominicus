// DrawStrikeEffect.h
// Crucible Island

#ifndef DRAWSTRIKEEFFECT_H
#define DRAWSTRIKEEFFECT_H

#include "graphics/DrawTypes.h"
#include "graphics/2dgraphics/DrawGrayOut.h"

class DrawStrikeEffect : public DrawGrayOut {
public:
	DrawStackArgList instantiateArgList() { return DrawStackArgList(); }
	void deleteArgList(DrawStackArgList argList) { }

	void execute(DrawStackArgList argList);
};

#endif // DRAWSTRIKEEFFECT_H
