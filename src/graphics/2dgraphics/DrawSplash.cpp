// DrawSplash.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/2dgraphics/DrawSplash.h"

DrawSplash::DrawSplash() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "colorTexture"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "colorTexture"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");
	uniforms["texture"] = glGetUniformLocation(shaderProgram, "texture");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["texCoord"] = glGetAttribLocation(shaderProgram, "texCoord");
	attributes["color"] = glGetAttribLocation(shaderProgram, "color");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	Texture* texture = gameGraphics->getTexture("branding/splash");
	float yBump = (
			((float) gameGraphics->resolutionX / (float) gameGraphics->resolutionY) -
			((float) texture->width / (float) texture->height)
		) * 0.5f;	// preserve aspect ratio of image
	GLfloat vertexBufferArray[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f + yBump, 1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f - yBump, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f - yBump, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f + yBump, 1.0f, 1.0f, 1.0f, 1.0f
		};

	GLushort elementBufferArray[] = { 0, 1, 2, 3 };

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferArray), vertexBufferArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);
}

DrawSplash::~DrawSplash() {
	// delete buffers
	if(glIsBuffer(vertexBuffers["vertices"]))
		glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	if(glIsBuffer(vertexBuffers["elements"]))
		glDeleteBuffers(1, &(vertexBuffers["elements"]));

	// delete shader program
	if(glIsProgram(shaderProgram))
		glDeleteProgram(shaderProgram);
}

void DrawSplash::execute(std::map<std::string, void*> arguments) {
	// state
	glEnable(GL_TEXTURE_2D);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, gameGraphics->idMatrixArray);
	glUniform1i(uniforms["texture"], 0);

	// activate the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameGraphics->getTextureID("branding/splash"));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), NULL);
	glVertexAttribPointer(attributes["texCoord"], 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT),
			(void*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT),
			(void*) (5 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["texCoord"]);
	glEnableVertexAttribArray(attributes["color"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["texCoord"]);
	glDisableVertexAttribArray(attributes["color"]);

	// undo state
	glDisable(GL_TEXTURE_2D);
}
