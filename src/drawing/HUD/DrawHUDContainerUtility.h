/*
 *  DrawConsole.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/8/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef DRAWCONSOLE_H
#define DRAWCONSOLE_H

// Note: all screen coordinates are in units of 2.0 per screen width (-1.0 to 1.0)

// program headers
#include "MatrixMath.h"
#include "Platform.h"
#include "ScalarMath.h"
#include "ShaderTools.h"
#include "VectorMath.h"

// library headers
#include <OpenGL/OpenGL.h>

// global variables
extern Platform platform;

// console container constants
#define INSIDE_COLOR 0.047058823529412f, 0.043137254901961f, 0.137254901960784f, 0.6f
#define HIGHLIGHT_COLOR 0.274509803921569f, 0.298039215686275f, 0.403921568627451f, 1.0f
#define BORDER_COLOR 0.52156862745098f, 0.568627450980392f, 0.537254901960784f, 1.0f
#define OUTSIDE_COLOR 0.0f, 0.0f, 0.0f, 0.0f
#define CONTAINER_BORDER_WIDTH 8.0f / screenWidth * 2.0f

class DrawConsole {
private:
	// internal variables
	unsigned int screenWidth, screenHeight;

	// internal drawing functions
	void drawCurve(Vector2 position, Vector2 dimensions, float rotation, bool highlight, float zMod, bool concave);
	void drawBorder(Vector2 position, Vector2 dimensions, float rotation, bool highlight, float zMod);
	void drawFiller(Vector2 position, Vector2 dimensions, bool highlight, float zMod);

	// container shader
	GLuint containerProgram;
	GLuint containerVertexShader;
	GLuint containerFragmentShader;

	// container shader uniforms
	GLuint insideColorUniform;		// vec4
	GLuint borderColorUniform;		// vec4
	GLuint outsideColorUniform;		// vec4

	GLuint curveOriginCoordUniform;	// vec2
	GLuint border1DistUniform;		// float
	GLuint border2DistUniform;		// float
	GLuint zModUniform;				// float

	// container shader attributes
	GLuint positionAttrib;			// vec2
	GLuint primCoordAttrib;			// vec2

public:
	DrawConsole(unsigned int screenWidth, unsigned int screenHeight);

	void render();
};

#endif
