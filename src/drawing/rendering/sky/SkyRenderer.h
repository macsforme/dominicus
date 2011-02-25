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
#include "drawing/texture/BMPImage.h"
#include "drawing/ShaderTools.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "state/Ship.h"

// global variables
extern Platform* platform;
extern Ship* ship;

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
