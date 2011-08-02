/*
 * WaterRenderer.cpp
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

#include "graphics/3dgraphics/renderers/WaterRenderer.h"

WaterRenderer::WaterRenderer() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "default"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "default"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");
	uniforms["useTexture"] = glGetUniformLocation(shaderProgram, "useTexture");
	uniforms["texture"] = glGetUniformLocation(shaderProgram, "texture");
	uniforms["useColor"] = glGetUniformLocation(shaderProgram, "useColor");
	uniforms["useLighting"] = glGetUniformLocation(shaderProgram, "useLighting");
	uniforms["lightPosition"] = glGetUniformLocation(shaderProgram, "lightPosition");
	uniforms["lightColor"] = glGetUniformLocation(shaderProgram, "lightColor");
	uniforms["specularColor"] = glGetUniformLocation(shaderProgram, "specularColor");
	uniforms["shininess"] = glGetUniformLocation(shaderProgram, "shininess");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["color"] = glGetAttribLocation(shaderProgram, "color");
	attributes["texCoord"] = glGetAttribLocation(shaderProgram, "texCoord");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLfloat vertDataBufferArray[] = {
			-10000.0f, 0.0f, -10000.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			-10000.0f, 0.0f, 10000.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 50.0f,
			10000.0f, 0.0f, 10000.0f, 1.0f, 1.0f, 1.0f, 1.0f, 50.0f, 50.0f,
			10000.0f, 0.0f, -10000.0f, 1.0f, 1.0f, 1.0f, 1.0f, 50.0f, 0.0f
		};

	GLushort vertElementBufferArray[] = {
			0, 1, 2, 3
		};

	// send the buffer data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertDataBufferArray), vertDataBufferArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertElementBufferArray), vertElementBufferArray,
			GL_STATIC_DRAW);
}

WaterRenderer::~WaterRenderer() {
	;
}

void WaterRenderer::execute(std::map<std::string, void*> arguments) {
	// collect arguments
	Camera* camera = ((Camera*) arguments["camera"]);

	// call for an updated camera
	camera->execute();

	// prepare variables
	Matrix4* mvpMatrix = &(camera->terrainMatrix);
	float mvpMatrixArray[] = {
			mvpMatrix->m11, mvpMatrix->m12, mvpMatrix->m13, mvpMatrix->m14,
			mvpMatrix->m21, mvpMatrix->m22, mvpMatrix->m23, mvpMatrix->m24,
			mvpMatrix->m31, mvpMatrix->m32, mvpMatrix->m33, mvpMatrix->m34,
			mvpMatrix->m41, mvpMatrix->m42, mvpMatrix->m43, mvpMatrix->m44
		};

	// state
	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, mvpMatrixArray);
/*
	Matrix3 orientation = completeMatrix(gameState->ships[0]->direction);
	orientation = Matrix3(
			-orientation.m31, orientation.m32, -orientation.m33,
			orientation.m21, orientation.m22, orientation.m23,
			orientation.m11, orientation.m12, orientation.m13
		);
	orientation.transpose();
	Vector3 lightPosition(1.0f, 1.0f, -1.0f);
	lightPosition.norm();
	lightPosition = lightPosition * orientation;
*/
	glUniform1i(uniforms["useLighting"], 0);
	glUniform1i(uniforms["useTexture"], 1);
	glUniform1i(uniforms["useColor"], 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameGraphics->getTextureID("water/water"));
	glUniform1i(uniforms["texture"], 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	float maxAniso;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), (void*) 0);
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT),
			(GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["texCoord"], 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT),
			(GLvoid*) (7 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["color"]);
	glEnableVertexAttribArray(attributes["texCoord"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["color"]);
	glDisableVertexAttribArray(attributes["texCoord"]);

	// undo state
	glDisable(GL_TEXTURE_2D);
//	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}
