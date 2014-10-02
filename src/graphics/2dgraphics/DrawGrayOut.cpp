// DrawGrayOut.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawGrayOut.h"

#include <cstdlib>

#include "graphics/GameGraphics.h"
#include "platform/OpenGLHeaders.h"

extern GameGraphics* gameGraphics;

DrawGrayOut::DrawGrayOut() {
	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLushort elementBufferArray[] = { 0, 1, 2, 3 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);
}

DrawGrayOut::~DrawGrayOut() {
	// undo vertex buffer setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

DrawStackArgList DrawGrayOut::instantiateArgList() {
	DrawStackArgList argList;

	argList["color"] = (void*) new Vector4;		// color of overlay

	return argList;
}

void DrawGrayOut::deleteArgList(DrawStackArgList argList) {
	if(argList.find("color") != argList.end()) delete (Vector4*) argList["color"];
}

void DrawGrayOut::execute(DrawStackArgList argList) {
	// collect arguments
	Vector4 color = *((Vector4*) argList["color"]);

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

	glBufferData(GL_ARRAY_BUFFER, 28 * sizeof(GLfloat), vertexBufferArray, GL_STREAM_DRAW);

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	if(gameGraphics->supportsMultisampling) glDisable(GL_MULTISAMPLE);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_TEXTURE_2D);

	// enable shader
	glUseProgram(gameGraphics->getProgramID("color"));

	// set uniforms
	glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("color"), "mvpMatrix"), 1, GL_FALSE, gameGraphics->idMatrixArray);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("color"), "position"), 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*) 0);
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("color"), "color"), 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("color"), "position"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("color"), "color"));

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("color"), "position"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("color"), "color"));
}
