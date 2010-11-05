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
#include "Mouse.h"
#include "Platform.h"
#include "Screen.h"
#include "ShaderTools.h"
#include "VectorMath.h"

// library headers
#include <OpenGL/OpenGL.h>

// global variables
extern Mouse mouse;
extern Platform platform;

class DrawCursor {
private:
	// screen dimensions
	Screen* screen;

	// program/shader names
	GLuint program, vertexShader, fragmentShader;

	// attributes
	GLint positionAttrib, boxCoordAttrib;

	// uniforms
	GLint thicknessUniform;

public:
	DrawCursor(Screen* screen);

	void draw();
};

#endif
