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
#include "DrawHUDContainerUtility.h"
#include "FontManager.h"
#include "GamePrefs.h"
#include "HUDArrangement.h"
#include "HUDElement.h"
#include "OpenGLHeaders.h"
#include "Platform.h"
#include "Screen.h"
#include "ShaderTools.h"
#include "TextBlock.h"
#include "VectorMath.h"

// library headers
#include <sstream>
#include <string>
#include <vector>

// global variables
extern Platform platform;
extern GamePrefs gamePrefs;

class DrawInfoBox {
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

	// fps tracking
	unsigned long int lastCheck;
	unsigned int frameCount;
	unsigned int currentFPS;

	// execution time tracking
	float currentExecTime;

public:
	DrawInfoBox(HUDArrangement* hudArrangement, Screen* screen, FontManager* fontManager);

	void draw();
};

#endif
