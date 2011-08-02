/*
 * DrawGrayOut.cpp
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

#include "graphics/2dgraphics/ui/DrawGrayOut.h"

DrawGrayOut::DrawGrayOut() {
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLushort elementBufferArray[] = { 0, 1, 2, 3 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);
}

DrawGrayOut::~DrawGrayOut() {
	// delete buffers
	if(glIsBuffer(vertexBuffers["vertices"]))
		glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	if(glIsBuffer(vertexBuffers["elements"]))
		glDeleteBuffers(1, &(vertexBuffers["elements"]));

	// delete shader program
	if(glIsProgram(shaderProgram))
		glDeleteProgram(shaderProgram);
}

Vector2 DrawGrayOut::getSize(std::map<std::string, void*> arguments) {
	return Vector2(2.0f, 2.0f);
}

void DrawGrayOut::execute(std::map<std::string, void*> arguments) {
	// collect arguments
	Vector4 color = *((Vector4*) arguments["color"]);

	// get the actual size so possibly incorrect metrics don't skew the aspect ratio
	Vector2 actualSize = getSize(arguments);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	GLfloat vertexBufferArray[] = {
			-1.0f, -1.0f, 0.0f, color.x, color.y, color.z, color.w,
			-1.0f, 1.0f, 0.0f, color.x, color.y, color.z, color.w,
			1.0f, 1.0f, 0.0f, color.x, color.y, color.z, color.w,
			1.0f, -1.0f, 0.0f, color.x, color.y, color.z, color.w
		};

	glBufferData(GL_ARRAY_BUFFER, 28 * sizeof(GL_FLOAT), vertexBufferArray, GL_STREAM_DRAW);

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, gameGraphics->idMatrixArray);
	glUniform1i(uniforms["useTexture"], 0);
	glUniform1i(uniforms["useColor"], 1);
	glUniform1i(uniforms["useLighting"], 0);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*) 0);
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT),
			(GLvoid*) (3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["color"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["color"]);

	// undo state
	glDisable(GL_BLEND);
}
