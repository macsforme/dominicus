// DrawStrikeEffect.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawStrikeEffect.h"

#include <cstdlib>

#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "state/GameState.h"

extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;

void DrawStrikeEffect::execute(DrawStackArgList argList) {
	float effectProgression = (float) (gameState->lastUpdateGameTime - gameState->lastStrikeTime) / (gameSystem->getFloat("hudStrikeEffectTime") * 1000.0f);

	if(effectProgression >= 1.0f)
		return;

	float randomColor = (float) rand() / (float) RAND_MAX;
	float randomAlpha = (1.0f - effectProgression);

	Vector4 color(randomColor, randomColor, randomColor, randomAlpha);

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

	// undo state
	glDisable(GL_BLEND);
}
