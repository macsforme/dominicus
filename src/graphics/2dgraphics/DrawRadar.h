// DrawRadar.h
// Crucible Island

#ifndef DRAWRADAR_H
#define DRAWRADAR_H

#include <map>
#include <string>
#include <vector>

#include "graphics/DrawTypes.h"
#include "graphics/2dgraphics/DrawContainer.h"
#include "graphics/2dgraphics/DrawCircle.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "state/GameState.h"

class DrawRadar : public BaseUIElement {
private:
	std::vector<Missile> missileCache;
	float lastRotation;

	GLuint radarTextureID;
	GLuint progressionTextureID;

	DrawContainer* containerDrawer;
	DrawCircle* circleDrawer;

public:
	DrawRadar(DrawContainer* newContainerDrawer, DrawCircle* newCircleDrawer);
	~DrawRadar();

	void reloadState();

	DrawStackArgList instantiateArgList();
	void deleteArgList(DrawStackArgList argList);

	Vector2 getSize(DrawStackArgList argList);

	void execute(DrawStackArgList argList);
};

#endif // DRAWRADAR_H
