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
#include "HUDArrangement.h"
#include "HUDElement.h"
#include "Platform.h"
#include "ProgramLog.h"
#include "Screen.h"
#include "ShaderTools.h"
#include "TextBlock.h"
#include "VectorMath.h"

// library headers
#include <OpenGL/OpenGL.h>
#include <string>
#include <vector>

// global variables
extern Platform platform;
extern GamePrefs gamePrefs;

class DrawConsole {
private:
	Screen* screen;
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
	DrawConsole(HUDArrangement* hudArrangement, Screen* screen, FontManager* fontManager);

	void draw();
};

#endif
