// SkyRenderer.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/3dgraphics/renderers/SkyRenderer.h"

SkyRenderer::SkyRenderer() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "default"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "default"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");
	uniforms["useTexture"] = glGetUniformLocation(shaderProgram, "useTexture");
	uniforms["useColor"] = glGetUniformLocation(shaderProgram, "useColor");
	uniforms["useLighting"] = glGetUniformLocation(shaderProgram, "useLighting");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["color"] = glGetAttribLocation(shaderProgram, "color");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLfloat vertDataBufferArray[] = {
			-1.0f, -1.0f, 0.0f, 0.62f, 0.52f, 0.20f, 1.0f,
			-1.0f, 1.0f, 0.0f, 0.68f, 0.73f, 0.89f, 1.0f,
			1.0f, 1.0f, 0.0f, 0.68f, 0.73f, 0.89f, 1.0f,
			1.0f, -1.0f, 0.0f, 0.62f, 0.52f, 0.20f, 1.0f
		};

	GLushort vertElementBufferArray[] = {
			0, 1, 2, 3
		};

	// send the buffer data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertDataBufferArray), vertDataBufferArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertElementBufferArray), vertElementBufferArray,
			GL_STATIC_DRAW);
}

SkyRenderer::~SkyRenderer() {
	;
}

void SkyRenderer::execute(std::map<std::string, void*> arguments) {
	// collect arguments

	// prepare variables
	Matrix4* mvpMatrix = &(gameGraphics->idMatrix);
	float mvpMatrixArray[] = {
			mvpMatrix->m11, mvpMatrix->m12, mvpMatrix->m13, mvpMatrix->m14,
			mvpMatrix->m21, mvpMatrix->m22, mvpMatrix->m23, mvpMatrix->m24,
			mvpMatrix->m31, mvpMatrix->m32, mvpMatrix->m33, mvpMatrix->m34,
			mvpMatrix->m41, mvpMatrix->m42, mvpMatrix->m43, mvpMatrix->m44
		};

	// state

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, mvpMatrixArray);
	glUniform1i(uniforms["useTexture"], 0);
	glUniform1i(uniforms["useColor"], 1);
	glUniform1i(uniforms["useLighting"], 0);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (GLvoid*) 0);
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT),
			(GLvoid*) (3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["color"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLvoid*) 0);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["color"]);

	// undo state
	glDisable(GL_BLEND);
}
