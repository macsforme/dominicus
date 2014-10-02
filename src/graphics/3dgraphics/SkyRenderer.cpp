// SkyRenderer.cpp
// Crucible Island

#include "graphics/3dgraphics/SkyRenderer.h"

#include <cmath>

#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "math/MatrixMath.h"
#include "math/MiscMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "state/GameState.h"

extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;

SkyRenderer::SkyRenderer() {
	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLfloat vertDataBufferArray[] = {
			-1.0f, -1.0f,
			-1.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, -1.0f
		};

	GLushort vertElementBufferArray[] = {
			0, 1, 2, 3
		};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertDataBufferArray), vertDataBufferArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertElementBufferArray), vertElementBufferArray,
			GL_STATIC_DRAW);
}

SkyRenderer::~SkyRenderer() {
	// undo vertex buffer setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

void SkyRenderer::execute(DrawStackArgList arguments) {
	// prepare variables
	Matrix4 mvpMatrix; mvpMatrix.identity();
	scaleMatrix(
			tan(radians(gameState->binoculars ? gameSystem->getFloat("renderingPerspectiveBinocularsFOV") : gameSystem->getFloat("renderingPerspectiveFOV"))),
			1.0f / (gameGraphics->aspectRatio / tan(radians(gameState->binoculars ? gameSystem->getFloat("renderingPerspectiveBinocularsFOV") : gameSystem->getFloat("renderingPerspectiveFOV")))),
			1.0f,
			mvpMatrix
		);

	rotateMatrix(
			Vector3(1.0f, 0.0f, 0.0f),
			radians(
					getAngle(Vector2(
							mag(Vector2(gameGraphics->currentCamera->mvMatrix.m13, gameGraphics->currentCamera->mvMatrix.m33)),
							gameGraphics->currentCamera->mvMatrix.m23
						))
				),
			mvpMatrix
		);
	float mvpMatrixArray[] = {
			mvpMatrix.m11, mvpMatrix.m12, mvpMatrix.m13, mvpMatrix.m14,
			mvpMatrix.m21, mvpMatrix.m22, mvpMatrix.m23, mvpMatrix.m24,
			mvpMatrix.m31, mvpMatrix.m32, mvpMatrix.m33, mvpMatrix.m34,
			mvpMatrix.m41, mvpMatrix.m42, mvpMatrix.m43, mvpMatrix.m44
		};

	// no state changes

	// enable shader
	glUseProgram(gameGraphics->getProgramID("sky"));

	// set uniforms
	glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("sky"), "mvpMatrix"), 1, GL_FALSE, mvpMatrixArray);
	float empColorMultiplier = (
			gameState->fortress.emp <= 0.0f || gameState->fortress.emp >= 1.0f ?
			1.0f :
			1.0f - gameState->fortress.emp + gameState->fortress.emp * gameSystem->getFloat("empColorMultiplier")
		);

	Vector4 waterColor = gameSystem->getColor("waterColor") * empColorMultiplier;
	glUniform4f(glGetUniformLocation(gameGraphics->getProgramID("sky"), "waterColor"), waterColor.x, waterColor.y, waterColor.z, waterColor.w);
	Vector4 horizonColor = gameSystem->getColor("horizonColor") * empColorMultiplier;
	glUniform4f(glGetUniformLocation(gameGraphics->getProgramID("sky"), "horizonColor"), horizonColor.x, horizonColor.y, horizonColor.z, waterColor.w);
	Vector4 baseSkyColor = gameSystem->getColor("baseSkyColor") * empColorMultiplier;
	glUniform4f(glGetUniformLocation(gameGraphics->getProgramID("sky"), "baseSkyColor"), baseSkyColor.x, baseSkyColor.y, baseSkyColor.z, baseSkyColor.w);
	Vector4 apexColor = gameSystem->getColor("apexColor") * empColorMultiplier;;
	glUniform4f(glGetUniformLocation(gameGraphics->getProgramID("sky"), "apexColor"), apexColor.x, apexColor.y, apexColor.z, apexColor.w);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("sky"), "position"), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);

	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("sky"), "position"));

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLvoid*) 0);

	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("sky"), "position"));

	// no state changes to undo
}
