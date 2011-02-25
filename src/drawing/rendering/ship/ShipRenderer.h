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
#include "core/GamePrefs.h"
#include "core/ProgramLog.h"
#include "drawing/texture/BMPImage.h"
#include "drawing/ShaderTools.h"
#include "geometry/Mesh.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "math/ScalarMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "state/Ship.h"

// library headers
#include <stdint.h>
#include <string>
#include <vector>

// global variables
extern GamePrefs* gamePrefs;
extern Platform* platform;
extern ProgramLog* programLog;

class ShipRenderer {
private:
	// program/shader names
	GLuint modelProgram, modelVertexShader, modelFragmentShader;
	GLuint wireframeProgram, wireframeVertexShader, wireframeFragmentShader;

	// attributes
	GLint positionAttrib, normalAttrib, texCoordAttrib, colorAttrib;

	// uniforms
	GLint mvpMatrixUniform, useTextureUniform, textureUniform;
	GLint useLightingUniform, ambientColorUniform, lightPositionUniform, lightColorUniform;
	GLint specularColorUniform, shininessUniform;

	// texture IDs
	std::map<std::string,GLuint> textureIDs;

	// buffers
	GLuint vertDataBuffer;
	std::map<std::string,GLuint> vertElementBuffers;

	// view switches
	bool viewModel;
	bool viewWireFrame;

	Vector3 leftPropellerOrigin, rightPropellerOrigin;

public:
	Mesh ship;

	ShipRenderer();
	~ShipRenderer();

	void render(Matrix4 mvMatrix, Matrix4 pMatrix);
};

#endif
