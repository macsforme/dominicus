/*
 *  ShipRenderer.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 7/26/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef SHIPRENDERER_H
#define SHIPRENDERER_H

// program headers
#include "BMPImage.h"
#include "GamePrefs.h"
#include "MatrixMath.h"
#include "Mesh.h"
#include "OpenGLHeaders.h"
#include "Platform.h"
#include "ProgramLog.h"
#include "ScalarMath.h"
#include "ShaderTools.h"
#include "VectorMath.h"

// library headers
#include <stdint.h>
#include <string>
#include <vector>

// global variables
extern Platform platform;

class ShipRenderer {
private:
	// program/shader names
	GLuint modelProgram, modelVertexShader, modelFragmentShader;
	GLuint wireframeProgram, wireframeVertexShader, wireframeFragmentShader;

	// vertex/element buffers
//	GLuint vertexBuffer;
//	std::map<std::string,GLuint> elementBuffers;

	// attributes
	GLint positionAttrib, normalAttrib, texCoordAttrib;

	// uniforms
	GLint mvpMatrixUniform, textureUniform, clockUniform;

	// texture IDs
	std::map<std::string,GLuint> textureIDs;

	// view switches
	bool viewModel;
	bool viewWireFrame;

public:
	Mesh ship;

	ShipRenderer();
	~ShipRenderer();

	void render(Matrix4 mvpMatrix);
};

#endif
