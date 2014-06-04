// DrawRadar.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef DRAWRADAR_H
#define DRAWRADAR_H

// forward declarations
class DrawSpot;

// program headers
#include "graphics/GameGraphics.h"
#include "graphics/2dgraphics/DrawContainer.h"
#include "graphics/2dgraphics/DrawSpot.h"
#include "graphics/texture/Texture.h"
#include "logic/UILayoutAuthority.h"
#include "math/MatrixMath.h"
#include "math/MiscMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "state/GameState.h"

// library headers
#include <map>
#include <string>
#include <vector>

// global variables
extern GameGraphics* gameGraphics;
extern GameState* gameState;

class DrawRadar : public BaseUIElement {
private:
	std::vector<Missile> missileCache;
	float lastRotation;

	GLuint radarTextureID;
	GLuint progressionTextureID;

	DrawContainer* containerDrawer;
	DrawSpot* spotDrawer;

public:
	DrawRadar();
	~DrawRadar();

	void reloadGraphics();

	Vector2 getSize(std::map<std::string, void*> arguments);
	void execute(std::map<std::string, void*> arguments);
};

#endif // DRAWRADAR_H
