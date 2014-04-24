// DrawProgressBar.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/2dgraphics/ui/DrawProgressBar.h"

DrawProgressBar::DrawProgressBar() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "color"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "color"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["color"] = glGetAttribLocation(shaderProgram, "color");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLushort elementBufferArray[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);
}

DrawProgressBar::~DrawProgressBar() {
	// delete buffers
	if(glIsBuffer(vertexBuffers["vertices"]))
		glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	if(glIsBuffer(vertexBuffers["elements"]))
		glDeleteBuffers(1, &(vertexBuffers["elements"]));

	// delete shader program
	if(glIsProgram(shaderProgram))
		glDeleteProgram(shaderProgram);
}

Vector2 DrawProgressBar::getSize(std::map<std::string, void*> arguments) {
	return Vector2(
			((Vector2*) arguments["size"])->x / (float) gameGraphics->resolutionX * 2.0f,
			((Vector2*) arguments["size"])->y / (float) gameGraphics->resolutionY * 2.0f
		);
}

void DrawProgressBar::execute(std::map<std::string, void*> arguments) {
	// collect arguments
	UIMetrics metrics = *((UIMetrics*) arguments["metrics"]);
	metrics.size = getSize(arguments);
	Vector4* color1Top = ((Vector4*) arguments["color1Top"]);
	Vector4* color1Bottom = ((Vector4*) arguments["color1Bottom"]);
	Vector4* color2Top = ((Vector4*) arguments["color2Top"]);
	Vector4* color2Bottom = ((Vector4*) arguments["color2Bottom"]);
	float* progression = ((float*) arguments["progression"]);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLfloat vertexBufferArray[] = {
			metrics.position.x - metrics.size.x / 2.0f,
			metrics.position.y - metrics.size.y / 2.0f,
			0.0f,
			color1Bottom->x,
			color1Bottom->y,
			color1Bottom->z,
			color1Bottom->w,

			metrics.position.x - metrics.size.x / 2.0f,
			metrics.position.y + metrics.size.y / 2.0f,
			0.0f,
			color1Top->x,
			color1Top->y,
			color1Top->z,
			color1Top->w,

			metrics.position.x - metrics.size.x / 2.0f + metrics.size.x * *progression,
			metrics.position.y + metrics.size.y / 2.0f,
			0.0f,
			color1Top->x,
			color1Top->y,
			color1Top->z,
			color1Top->w,

			metrics.position.x - metrics.size.x / 2.0f + metrics.size.x * *progression,
			metrics.position.y - metrics.size.y / 2.0f,
			0.0f,
			color1Bottom->x,
			color1Bottom->y,
			color1Bottom->z,
			color1Bottom->w,

			metrics.position.x - metrics.size.x / 2.0f + metrics.size.x * *progression,
			metrics.position.y - metrics.size.y / 2.0f,
			0.0f,
			color2Bottom->x,
			color2Bottom->y,
			color2Bottom->z,
			color2Bottom->w,

			metrics.position.x - metrics.size.x / 2.0f + metrics.size.x * *progression,
			metrics.position.y + metrics.size.y / 2.0f,
			0.0f,
			color2Top->x,
			color2Top->y,
			color2Top->z,
			color2Top->w,

			metrics.position.x + metrics.size.x / 2.0f,
			metrics.position.y + metrics.size.y / 2.0f,
			0.0f,
			color2Top->x,
			color2Top->y,
			color2Top->z,
			color2Top->w,

			metrics.position.x + metrics.size.x / 2.0f,
			metrics.position.y - metrics.size.y / 2.0f,
			0.0f,
			color2Bottom->x,
			color2Bottom->y,
			color2Bottom->z,
			color2Bottom->w
		};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferArray), vertexBufferArray, GL_STREAM_DRAW);

	// state
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, gameGraphics->idMatrixArray);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*) 0);
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT),
			(GLvoid*) (3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["color"]);

	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["color"]);

	// undo state
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}
