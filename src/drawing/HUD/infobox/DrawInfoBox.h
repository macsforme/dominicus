/*
 *  DrawInfoBox.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/26/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef DRAWINFOBOX_H
#define DRAWINFOBOX_H

// program headers
#include "core/GamePrefs.h"
#include "drawing/HUD/DrawHUDContainerUtility.h"
#include "drawing/text/FontManager.h"
#include "drawing/HUD/HUDArrangement.h"
#include "drawing/HUD/HUDElement.h"
#include "drawing/ShaderTools.h"
#include "drawing/text/TextBlock.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "state/Ship.h"
#include "windowing/GameWindow.h"

// library headers
#include <sstream>
#include <string>
#include <vector>

// global variables
extern GamePrefs gamePrefs;
extern Platform platform;
extern Ship ship;

extern GameWindow* gameWindow;

class DrawInfoBox {
private:
	FontManager* fontManager;
	HUDElement* myElement;
	TextBlock* textBlock;
	DrawHUDContainerUtility* containerUtility;

	// program/shader names
	GLuint program, vertexShader, fragmentShader;

	// attributes
	GLint positionAttrib, texCoordAttrib, colorAttrib;

	// uniforms
	GLint mvpMatrixUniform, textureUniform, useTextureUniform, useLightingUniform;

	// fps tracking
	unsigned long int lastCheck;
	unsigned int frameCount;
	unsigned int currentFPS;

	// execution time tracking
	float currentExecTime;

public:
	DrawInfoBox(HUDArrangement* hudArrangement, FontManager* fontManager);

	void draw();
};

#endif
