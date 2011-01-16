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
#include "DrawHUDContainerUtility.h"
#include "FontManager.h"
#include "GamePrefs.h"
#include "GameWindow.h"
#include "HUDArrangement.h"
#include "HUDElement.h"
#include "OpenGLHeaders.h"
#include "Platform.h"
#include "ProgramLog.h"
#include "ShaderTools.h"
#include "TextBlock.h"
#include "VectorMath.h"

// library headers
#include <string>
#include <vector>

// global variables
extern Platform platform;
extern GamePrefs gamePrefs;

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
	GLint positionAttrib, texCoordAttrib;

	// uniforms
	GLint mvpMatrixUniform, textureUniform;

public:
	DrawConsole(HUDArrangement* hudArrangement, FontManager* fontManager);

	void draw();
};

#endif
