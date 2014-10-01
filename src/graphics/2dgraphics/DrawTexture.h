// DrawTexture.h
// Crucible Island

#ifndef DRAWTEXTURE_H
#define DRAWTEXTURE_H

#include "graphics/DrawTypes.h"
#include "math/VectorMath.h"

class DrawTexture : public BaseUIElement {
public:
	DrawTexture();
	~DrawTexture();

	DrawStackArgList instantiateArgList();
	void deleteArgList(DrawStackArgList argList);

	Vector2 getSize(DrawStackArgList argList);

	void execute(DrawStackArgList argList);
};

#endif // DRAWTEXTURE_H
