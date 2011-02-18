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
#include "drawing/ShaderTools.h"
#include "input/Mouse.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "windowing/GameWindow.h"

// global variables
extern Mouse mouse;
extern Platform platform;

extern GameWindow* gameWindow;

class DrawControlBox {
private:
	// program/shader names
	GLuint program, vertexShader, fragmentShader;

	// attributes
	GLint positionAttrib, boxCoordAttrib;

	// uniforms
	GLint spotSizeUniform;

public:
	DrawControlBox();

	void draw();
};

#endif
