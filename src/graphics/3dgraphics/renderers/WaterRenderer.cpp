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

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "water"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "water"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["timer"] = glGetUniformLocation(shaderProgram, "timer");
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");
//	uniforms["color"] = glGetUniformLocation(shaderProgram, "color");
//	uniforms["highlightColor"] = glGetUniformLocation(shaderProgram, "highlightColor");
	uniforms["cameraPosition"] = glGetUniformLocation(shaderProgram, "cameraPosition");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	float vDist = gameSystem->getFloat("renderingPerspectiveFarClip") * 3.0f;

	GLfloat vertDataBufferArray[] = {
			-vDist, 0.0f, -vDist,
			-vDist, 0.0f, vDist,
			vDist, 0.0f, vDist,
			vDist, 0.0f, -vDist
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniform1f(uniforms["timer"], (float) (platform->getExecMills() % 3000) / 3000.0f);
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, mvpMatrixArray);
//	glUniform4f(uniforms["color"], 0.0314f, 0.0392f, 0.2470f, 1.0f);
//	Vector4 highlightColor = gameSystem->getColor("colorClear");
//	glUniform4f(uniforms["highlightColor"], highlightColor.x, highlightColor.y, highlightColor.z, 1.0f);
	glUniform3f(uniforms["cameraPosition"],  camera->position.x, camera->position.y, camera->position.z);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*) 0);

	glEnableVertexAttribArray(attributes["position"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLvoid*) 0);

	glDisableVertexAttribArray(attributes["position"]);

	// undo state
	glDisable(GL_BLEND);
}
