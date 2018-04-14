// DrawField.h
// Dominicus

#ifndef DRAWFIELD_H
#define DRAWFIELD_H

#include "graphics/DrawTypes.h"
#include "graphics/2dgraphics/DrawLabel.h"
#include "math/VectorMath.h"

class DrawField : public BaseUIElement {
private:
	DrawLabel* labelDrawer;

public:
	DrawField(DrawLabel* labelDrawer);
	~DrawField();

	DrawStackArgList instantiateArgList();
	void deleteArgList(DrawStackArgList argList);

	Vector2 getSize(DrawStackArgList argList);

	void execute(DrawStackArgList argList);
};

#endif // DRAWFIELD_H
