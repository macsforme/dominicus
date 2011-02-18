/*
 *  DrawCursor.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/5/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef DRAWCURSOR_H
#define DRAWCURSOR_H

// program headers
#include "drawing/ShaderTools.h"
#include "input/InputHandler.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "windowing/GameWindow.h"

// global variables
extern InputHandler* inputHandler;
extern Platform platform;

extern GameWindow* gameWindow;

class DrawCursor {
private:
	// program/shader names
	GLuint program, vertexShader, fragmentShader;

	// attributes
	GLint positionAttrib, boxCoordAttrib;

	// uniforms
	GLint thicknessUniform;

public:
	DrawCursor();

	void draw();
};

#endif
