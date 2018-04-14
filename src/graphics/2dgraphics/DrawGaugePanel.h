// DrawGaugePanel.h
// Dominicus

#ifndef DRAWGAUGEPANEL_H
#define DRAWGAUGEPANEL_H

#include "graphics/DrawTypes.h"
#include "graphics/2dgraphics/DrawContainer.h"
#include "graphics/2dgraphics/DrawProgressBar.h"
#include "graphics/2dgraphics/DrawTexture.h"
#include "math/VectorMath.h"

class DrawGaugePanel : public BaseUIElement {
private:
	DrawContainer* containerDrawer;
	DrawProgressBar* progressBarDrawer;
	DrawTexture* textureDrawer;

public:
	DrawGaugePanel(DrawContainer* containerDrawer, DrawProgressBar* progressBarDrawer, DrawTexture* textureDrawer) :
		containerDrawer(containerDrawer),
		progressBarDrawer(progressBarDrawer),
		textureDrawer(textureDrawer) { }

	DrawStackArgList instantiateArgList();
	void deleteArgList(DrawStackArgList argList);

	Vector2 getSize(DrawStackArgList argList);

	void execute(DrawStackArgList argList);
};

#endif // DRAWGAUGEPANEL_H
