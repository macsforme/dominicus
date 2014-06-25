// DrawSplash.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawSplash.h"

#include "graphics/GameGraphics.h"
#include "graphics/texture/Texture.h"
#include "platform/OpenGLHeaders.h"

extern GameGraphics* gameGraphics;

void DrawSplash::execute(DrawStackArgList argList) {
	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	// preserve aspect ratio of image
	Texture* texture = gameGraphics->getTexture("branding/splash");
	float yBump = ((float) gameGraphics->resolutionY / (float) gameGraphics->resolutionX) / ((float) texture->height / (float) texture->width) / 2.0f;
	GLfloat vertexBufferArray[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.5f - yBump, 1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 0.5f + yBump, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 0.5f + yBump, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.5f - yBump, 1.0f, 1.0f, 1.0f, 1.0f
		};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferArray), vertexBufferArray, GL_STREAM_DRAW);

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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), (void*) 0);
	glVertexAttribPointer(attributes["texCoord"], 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT),
			(GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT),
			(GLvoid*) (5 * sizeof(GLfloat)));

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
