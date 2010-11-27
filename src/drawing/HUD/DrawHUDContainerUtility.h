/*
 *  DrawHUDContainerUtility.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/8/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef DRAWHUDCONTAINERUTILITY_H
#define DRAWHUDCONTAINERUTILITY_H

// program headers
#include "MatrixMath.h"
#include "OpenGLHeaders.h"
#include "Platform.h"
#include "ScalarMath.h"
#include "ShaderTools.h"
#include "VectorMath.h"

// library headers
#include <string.h>

// global variables
extern Platform platform;

class DrawHUDContainerUtility {
private:
	float zStart;
	float borderWidth;

	float insideColor[4];
	float highlightColor[4];
	float borderColor[4];
	float outsideColor[4];

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
	DrawHUDContainerUtility(
			float zStart,
			float borderWidth,
			float newInsideColor[4],
			float newHighlightColor[4],
			float newBorderColor[4],
			float newOutsideColor[4]
		);

	// utility drawing functions
	void drawCurve(Vector2 position, Vector2 dimensions, float rotation, bool highlight,
			float zMod, bool concave);
	void drawBorder(Vector2 position, Vector2 dimensions, float rotation, bool highlight, float zMod);
	void drawFiller(Vector2 position, Vector2 dimensions, bool highlight, float zMod);
};

#endif
