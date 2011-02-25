/*
 * DrawHUDContainerUtility.h
 * Dominicus
 *
 * Copyright 2010-2011, Joshua Bodine
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef DRAWHUDCONTAINERUTILITY_H
#define DRAWHUDCONTAINERUTILITY_H

// program headers
#include "drawing/ShaderTools.h"
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"

// library headers
#include <string.h>

// global variables
extern Platform* platform;

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
