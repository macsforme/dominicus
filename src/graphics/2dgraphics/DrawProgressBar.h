// DrawProgressBar.h
// Crucible Island

#ifndef DRAWPROGRESSBAR_H
#define DRAWPROGRESSBAR_H

#include <map>
#include <string>

#include "graphics/DrawTypes.h"
#include "math/VectorMath.h"

class DrawProgressBar : public BaseUIElement {
public:
	DrawProgressBar();
	~DrawProgressBar();

	DrawStackArgList instantiateArgList();
	void deleteArgList(DrawStackArgList argList);

	Vector2 getSize(DrawStackArgList argList) { return *((Vector2*) argList["size"]); }

	void execute(DrawStackArgList argList);
};

#endif // DRAWPROGRESSBAR_H
