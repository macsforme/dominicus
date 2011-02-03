/*
 *  SkyRenderer.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef SKYRENDERER_H
#define SKYRENDERER_H

// program headers
#include "BMPImage.h"
#include "MatrixMath.h"
#include "OpenGLHeaders.h"
#include "Platform.h"
#include "ShaderTools.h"
#include "Ship.h"
#include "VectorMath.h"

// global variables
extern Platform platform;
extern Ship ship;

class SkyRenderer {
private:
	// program/shader names
	GLuint program, vertexShader, fragmentShader;

	// attributes
	GLint positionAttrib, degreesAttrib;

	// uniforms
	GLint mvpMatrixUniform, horizonColorUniform, skyColorUniform;

	// texture IDs
	GLuint textureID;

	// buffers
	GLuint vertDataBuffer, vertElementBuffer;

public:
	SkyRenderer();
	~SkyRenderer();

	void render(Matrix4 mvpMatrix);
};

#endif
