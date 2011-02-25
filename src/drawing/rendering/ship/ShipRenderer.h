/*
 * ShipRenderer.h
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
