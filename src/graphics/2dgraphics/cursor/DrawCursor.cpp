// DrawCursor.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/2dgraphics/cursor/DrawCursor.h"

DrawCursor::DrawCursor() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "cursor"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "cursor"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["thickness"] = glGetUniformLocation(shaderProgram, "thickness");
	uniforms["color"] = glGetUniformLocation(shaderProgram, "color");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["boxCoord"] = glGetAttribLocation(shaderProgram, "boxCoord");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLushort elementBufferArray[] = { 0, 1, 2, 3 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);
}

DrawCursor::~DrawCursor() {
	// delete buffers
	if(glIsBuffer(vertexBuffers["vertices"]))
		glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	if(glIsBuffer(vertexBuffers["elements"]))
		glDeleteBuffers(1, &(vertexBuffers["elements"]));

	// delete shader program
	if(glIsProgram(shaderProgram))
		glDeleteProgram(shaderProgram);
}

void DrawCursor::execute(std::map<std::string, void*> arguments) {
	// update position
	Vector2* position = *((Vector2**) arguments["position"]);
	float cursorSize = *((float*) arguments["size"]);
	float distY = cursorSize / (float) gameGraphics->resolutionY;
	float distX = distY / gameGraphics->aspectRatio;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	GLfloat vertexBufferArray[] = {
			position->x - distX, position->y - distY, -1.0f, -1.0f,
			position->x - distX, position->y + distY, -1.0f, 1.0f,
			position->x + distX, position->y + distY, 1.0f, 1.0f,
			position->x + distX, position->y - distY, 1.0f, -1.0f
		};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferArray), vertexBufferArray, GL_STREAM_DRAW);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexBufferArray), vertexBufferArray);

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniform1f(uniforms["thickness"],
			*((float*) arguments["thickness"]) /
			gameGraphics->resolutionY / distY
		);	// effectively half
	Vector4 color = *((Vector4*) arguments["color"]);
	glUniform4f(uniforms["color"], color.x, color.y, color.z, color.w);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*) 0);
	glVertexAttribPointer(attributes["boxCoord"], 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT),
			(GLvoid*) (2 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["boxCoord"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["boxCoord"]);

	// undo state
	glDisable(GL_BLEND);
}
