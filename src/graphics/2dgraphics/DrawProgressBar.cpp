// DrawProgressBar.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawProgressBar.h"

#include <vector>

#include "graphics/GameGraphics.h"
#include "graphics/UILayoutAuthority.h"
#include "platform/OpenGLHeaders.h"

extern GameGraphics* gameGraphics;

DrawProgressBar::DrawProgressBar() {
	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLushort elementBufferArray[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);
}

DrawProgressBar::~DrawProgressBar() {
	// undo vertex buffer setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

DrawStackArgList DrawProgressBar::instantiateArgList() {
	DrawStackArgList argList;

	argList["color1Bottom"] = (void*) new Vector4;		// color of first section at bottom
	argList["color1Top"] = (void*) new Vector4;			// color of first section at top
	argList["color2Bottom"] = (void*) new Vector4;		// color of second section at bottom
	argList["color2Top"] = (void*) new Vector4;			// color of second section at top
	argList["metrics"] = (void*) new UIMetrics;			// UI element metrics
	argList["progression"] = (void*) new float;			// progress bar completion ratio
	argList["size"] = (void*) new Vector2;				// width/height of field in screen dimensions

	return argList;
}

void DrawProgressBar::deleteArgList(DrawStackArgList argList) {
	if(argList.find("color1Bottom") != argList.end()) delete (Vector4*) argList["color1Bottom"];
	if(argList.find("color1Top") != argList.end()) delete (Vector4*) argList["color1Top"];
	if(argList.find("color2Bottom") != argList.end()) delete (Vector4*) argList["color1Bottom"];
	if(argList.find("color2Top") != argList.end()) delete (Vector4*) argList["color2Top"];
	if(argList.find("metrics") != argList.end()) delete (UIMetrics*) argList["metrics"];
	if(argList.find("progression") != argList.end()) delete (float*) argList["progression"];
	if(argList.find("size") != argList.end()) delete (Vector2*) argList["size"];
}

void DrawProgressBar::execute(DrawStackArgList argList) {
	// collect arguments
	UIMetrics metrics = *((UIMetrics*) argList["metrics"]);
	Vector2 size = getSize(argList);
	Vector4* color1Top = ((Vector4*) argList["color1Top"]);
	Vector4* color1Bottom = ((Vector4*) argList["color1Bottom"]);
	Vector4* color2Top = ((Vector4*) argList["color2Top"]);
	Vector4* color2Bottom = ((Vector4*) argList["color2Bottom"]);
	float* progression = ((float*) argList["progression"]);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLfloat vertexBufferArray[] = {
			metrics.position.x - size.x / 2.0f,
			metrics.position.y - size.y / 2.0f,
			0.0f,
			color1Bottom->x,
			color1Bottom->y,
			color1Bottom->z,
			color1Bottom->w,

			metrics.position.x - size.x / 2.0f,
			metrics.position.y + size.y / 2.0f,
			0.0f,
			color1Top->x,
			color1Top->y,
			color1Top->z,
			color1Top->w,

			metrics.position.x - size.x / 2.0f + size.x * *progression,
			metrics.position.y + size.y / 2.0f,
			0.0f,
			color1Top->x,
			color1Top->y,
			color1Top->z,
			color1Top->w,

			metrics.position.x - size.x / 2.0f + size.x * *progression,
			metrics.position.y - size.y / 2.0f,
			0.0f,
			color1Bottom->x,
			color1Bottom->y,
			color1Bottom->z,
			color1Bottom->w,

			metrics.position.x - size.x / 2.0f + size.x * *progression,
			metrics.position.y - size.y / 2.0f,
			0.0f,
			color2Bottom->x,
			color2Bottom->y,
			color2Bottom->z,
			color2Bottom->w,

			metrics.position.x - size.x / 2.0f + size.x * *progression,
			metrics.position.y + size.y / 2.0f,
			0.0f,
			color2Top->x,
			color2Top->y,
			color2Top->z,
			color2Top->w,

			metrics.position.x + size.x / 2.0f,
			metrics.position.y + size.y / 2.0f,
			0.0f,
			color2Top->x,
			color2Top->y,
			color2Top->z,
			color2Top->w,

			metrics.position.x + size.x / 2.0f,
			metrics.position.y - size.y / 2.0f,
			0.0f,
			color2Bottom->x,
			color2Bottom->y,
			color2Bottom->z,
			color2Bottom->w
		};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferArray), vertexBufferArray, GL_STREAM_DRAW);

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(gameGraphics->getProgramID("color"));

	// set uniforms
	glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("color"), "mvpMatrix"), 1, GL_FALSE, gameGraphics->idMatrixArray);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("color"), "position"), 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) 0);
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("color"), "color"), 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
			(GLvoid*) (3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("color"), "position"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("color"), "color"));

	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("color"), "position"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("color"), "color"));

	// undo state
	glDisable(GL_BLEND);
}
