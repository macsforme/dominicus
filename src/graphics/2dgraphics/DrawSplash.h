// DrawSplash.h
// Crucible Island

#ifndef DRAWSPLASH_H
#define DRAWSPLASH_H

#include "graphics/DrawTypes.h"
#include "graphics/2dgraphics/DrawTexture.h"
#include "math/VectorMath.h"

class DrawSplash : public DrawTexture {
public:
	DrawStackArgList instantiateArgList() { return DrawStackArgList(); };
	void deleteArgList(DrawStackArgList argList) { };

	Vector2 getSize(DrawStackArgList argList) { return Vector2(2.0f, 2.0f); }

	void execute(DrawStackArgList argList);
};

#endif // DRAWSPLASH_H
