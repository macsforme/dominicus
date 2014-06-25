// DrawButton.h
// Crucible Island

#ifndef DRAWBUTTON_H
#define DRAWBUTTON_H

#include "graphics/DrawTypes.h"
#include "graphics/2dgraphics/DrawContainer.h"
#include "graphics/2dgraphics/DrawLabel.h"
#include "math/VectorMath.h"

class DrawButton : public BaseUIElement {
private:
	DrawContainer* containerDrawer;
	DrawLabel* labelDrawer;

public:
	DrawButton(DrawContainer* newContainerDrawer, DrawLabel* newLabelDrawer);

	DrawStackArgList instantiateArgList();
	void deleteArgList(DrawStackArgList argList);

	Vector2 getSize(DrawStackArgList argList);

	void execute(DrawStackArgList argList);
};

#endif // DRAWBUTTON_H
