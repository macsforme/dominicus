// DrawTexture.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawTexture.h"

#include <cstdlib>
#include <string>

#include "graphics/GameGraphics.h"
#include "graphics/UILayoutAuthority.h"
#include "platform/OpenGLHeaders.h"

extern GameGraphics* gameGraphics;

DrawTexture::DrawTexture() {
	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLushort elementBufferArray[] = { 0, 1, 2, 3 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);
}

DrawTexture::~DrawTexture() {
	// undo vertex buffer setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

DrawStackArgList DrawTexture::instantiateArgList() {
	DrawStackArgList argList;

	argList["metrics"] = (void*) new UIMetrics;		// UI element metrics
	argList["size"] = (void*) new Vector2;			// manually-specified width/height of texture in screen dimensions (optional)
	argList["texture"] = (void*) new std::string;	// name of texture to draw

	return argList;
}

void DrawTexture::deleteArgList(DrawStackArgList argList) {
	if(argList.find("metrics") != argList.end()) delete (UIMetrics*) argList["metrics"];
	if(argList.find("size") != argList.end()) delete (Vector2*) argList["size"];
	if(argList.find("texture") != argList.end()) delete (std::string*) argList["texture"];
}

Vector2 DrawTexture::getSize(DrawStackArgList argList) {
	if(argList.find("size") != argList.end())
		return *((Vector2*) argList["size"]);

	std::string texture = *((std::string*) argList["texture"]);

	return Vector2(
			(float) gameGraphics->getTexture(texture)->width / (float) gameGraphics->resolutionX * 2.0f,
			(float) gameGraphics->getTexture(texture)->height / (float) gameGraphics->resolutionY * 2.0f
		);
}

void DrawTexture::execute(DrawStackArgList argList) {
	// collect arguments
	UIMetrics metrics = *((UIMetrics*) argList["metrics"]);
	Vector2 size = getSize(argList);
	std::string texture = *((std::string*) argList["texture"]);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLfloat vertexBufferArray[] = {
			metrics.position.x - size.x / 2.0f,
			metrics.position.y - size.y / 2.0f,
			0.0f,
			0.0f,
			0.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f,

			metrics.position.x - size.x / 2.0f,
			metrics.position.y + size.y / 2.0f,
			0.0f,
			0.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f,

			metrics.position.x + size.x / 2.0f,
			metrics.position.y + size.y / 2.0f,
			0.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f,

			metrics.position.x + size.x / 2.0f,
			metrics.position.y - size.y / 2.0f,
			0.0f,
			1.0f,
			0.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f
		};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferArray), vertexBufferArray, GL_STREAM_DRAW);

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	if(gameGraphics->supportsMultisampling) glDisable(GL_MULTISAMPLE);
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_TEXTURE_2D);

	// enable shader
	glUseProgram(gameGraphics->getProgramID("colorTexture"));

	// set uniforms
	glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("colorTexture"), "mvpMatrix"), 1, GL_FALSE, gameGraphics->idMatrixArray);
	glUniform1i(glGetUniformLocation(gameGraphics->getProgramID("colorTexture"), "texture"), 0);

	// activate the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameGraphics->getTextureID(texture));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "position"), 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*) 0);
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "texCoord"), 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "color"), 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*) (5 * sizeof(GLfloat)));

	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "position"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "texCoord"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "color"));

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "position"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "texCoord"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "color"));
}
