/*
 *  DrawConsole.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/5/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef DRAWCONSOLE_H
#define DRAWCONSOLE_H

// program headers
#include "core/GamePrefs.h"
#include "core/ProgramLog.h"
#include "drawing/HUD/DrawHUDContainerUtility.h"
#include "drawing/HUD/HUDArrangement.h"
#include "drawing/HUD/HUDElement.h"
#include "drawing/ShaderTools.h"
#include "drawing/text/FontManager.h"
#include "drawing/text/TextBlock.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "math/VectorMath.h"
#include "windowing/GameWindow.h"

// library headers
#include <string>
#include <vector>

// global variables
extern Platform platform;
extern GamePrefs gamePrefs;
extern ProgramLog programLog;

extern GameWindow* gameWindow;

class DrawConsole {
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

public:
	DrawConsole(HUDArrangement* hudArrangement, FontManager* fontManager);

	void draw();
};

#endif
