// DrawControlBox.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/2dgraphics/controlbox/DrawControlBox.h"

DrawControlBox::DrawControlBox() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "controlBox"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "controlBox"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["boxSize"] = glGetUniformLocation(shaderProgram, "boxSize");
	uniforms["spotSize"] = glGetUniformLocation(shaderProgram, "spotSize");
	uniforms["color"] = glGetUniformLocation(shaderProgram, "color");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["boxCoord"] = glGetAttribLocation(shaderProgram, "boxCoord");

	// set up texture
	std::stringstream filename;
	filename <<
			platform->dataPath <<
			"/data/textures/splash/splash.bmp";
	Texture texture(filename.str().c_str());

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLfloat vertexBufferArray[] = {
			-1.0f, -1.0f, 0.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, 0.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, -1.0f
		};

	GLushort elementBufferArray[] = { 0, 1, 2, 3 };

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferArray), vertexBufferArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);
}

DrawControlBox::~DrawControlBox() {
	// delete buffers
	if(glIsBuffer(vertexBuffers["vertices"]))
		glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	if(glIsBuffer(vertexBuffers["elements"]))
		glDeleteBuffers(1, &(vertexBuffers["elements"]));

	// delete shader program
	if(glIsProgram(shaderProgram))
		glDeleteProgram(shaderProgram);
}

void DrawControlBox::execute(std::map<std::string, void*> arguments) {
	// state
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	Vector2 boxSize = Vector2(
			*((float*) arguments["boxSize"]) / 100.0f / gameGraphics->aspectRatio,
			*((float*) arguments["boxSize"]) / 100.0f
		);

	float spotSize = *((float*) arguments["spotSize"]) / 100.0f;
	Vector4 color = *((Vector4*) arguments["color"]);

	glUniform2f(uniforms["boxSize"], boxSize.x, boxSize.y);
	glUniform1f(uniforms["spotSize"], spotSize / boxSize.y);
	glUniform4f(uniforms["color"], color.x, color.y, color.z, color.w);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), NULL);
	glVertexAttribPointer(attributes["boxCoord"], 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
			(void*) (3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["boxCoord"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["boxCoord"]);

	// undo state
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
