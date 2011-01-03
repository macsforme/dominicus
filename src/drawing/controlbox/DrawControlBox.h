/*
 *  DrawControlBox.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 12/26/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef DRAWCONTROLBOX_H
#define DRAWCONTROLBOX_H

// program headers
#include "GameWindow.h"
#include "Mouse.h"
#include "OpenGLHeaders.h"
#include "Platform.h"
#include "ShaderTools.h"
#include "VectorMath.h"

// global variables
extern Mouse mouse;
extern Platform platform;

class DrawControlBox {
private:
	// screen dimensions
	GameWindow* gameWindow;

	// program/shader names
	GLuint program, vertexShader, fragmentShader;

	// attributes
	GLint positionAttrib, boxCoordAttrib;

	// uniforms
	GLint spotSizeUniform;

public:
	DrawControlBox(GameWindow* gameWindow);

	void draw();
};

#endif
