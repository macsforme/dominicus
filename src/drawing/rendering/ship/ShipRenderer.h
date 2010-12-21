/*
 *  SubjectRenderer.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 7/26/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef SUBJECTRENDERER_H
#define SUBJECTRENDERER_H

// program headers
#include "BMPImage.h"
#include "Camera.h"
#include "MatrixMath.h"
#include "Mesh.h"
#include "Platform.h"
#include "OpenGLHeaders.h"
#include "Preferences.h"
#include "ProgramLog.h"
#include "ScalarMath.h"
#include "ShaderTools.h"
#include "VectorMath.h"

// library headers
#include <stdint.h>
#include <string>
#include <vector>

class SubjectRenderer {
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
	GLint MVPMatrixUniform, textureUniform, clockUniform;

	// texture IDs
	std::map<std::string,GLuint> textureIDs;

	// view switches
	bool viewModel;
	bool viewWireFrame;

public:
	Mesh subject;
	Camera camera;

	SubjectRenderer();
	~SubjectRenderer();

	void render();
};

#endif
