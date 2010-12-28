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
#include "GameWindow.h"
#include "HUDArrangement.h"
#include "HUDElement.h"
#include "OpenGLHeaders.h"
#include "Platform.h"
#include "ScalarMath.h"
#include "ShaderTools.h"
#include "Ship.h"
#include "TextBlock.h"
#include "VectorMath.h"

// library headers
#include <sstream>
#include <string>
#include <vector>

// global variables
extern GamePrefs gamePrefs;
extern Platform platform;
extern Ship ship;

class DrawInfoBox {
private:
	GameWindow* gameWindow;
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
	DrawInfoBox(HUDArrangement* hudArrangement, GameWindow* gameWindow, FontManager* fontManager);

	void draw();
};

#endif
