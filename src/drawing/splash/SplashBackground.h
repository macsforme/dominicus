/*
 *  SplashBackground.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/6/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef SPLASHBACKGROUND_H
#define SPLASHBACKGROUND_H

// program headers
#include "BMPImage.h"
#include "MatrixMath.h"
#include "Platform.h"
#include "ShaderTools.h"

// library headers
#include <OpenGL/OpenGL.h>

// global variables
extern Platform platform;

class SplashBackground {
private:
	// program/shader names
	GLuint program, vertexShader, fragmentShader;

	// attributes
	GLint positionAttrib, texCoordAttrib;

	// uniforms
	GLint textureUniform, mvpMatrixUniform;

	// texture id
	GLuint textureID;

public:
	SplashBackground();

	void draw();
};

#endif
