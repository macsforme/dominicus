// ShellRenderer.cpp
// Dominicus

#include "graphics/3dgraphics/ShellRenderer.h"

#include <cstdlib>

#include "core/GameSystem.h"
#include "geometry/Sphere.h"
#include "graphics/GameGraphics.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "state/GameState.h"

extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;

ShellRenderer::ShellRenderer() : sphere(makeSphere((size_t) gameSystem->getFloat("shellDensity"))) {
	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	GLfloat* vertDataBufferArray = new GLfloat[sphere.faceGroups[""].size() * 30];	// 3 vertices + 3 normals + 4 colors * 3 faces

	// insert the vertex attribute data
	for(size_t i = 0; i < sphere.faceGroups[""].size(); ++i) {
		for(size_t p = 0; p < 3; ++p) {
			vertDataBufferArray[i * 30 + p * 10 + 0] = sphere.vertices[sphere.faceGroups[""][i].vertices[p]].x;
			vertDataBufferArray[i * 30 + p * 10 + 1] = sphere.vertices[sphere.faceGroups[""][i].vertices[p]].y;
			vertDataBufferArray[i * 30 + p * 10 + 2] = sphere.vertices[sphere.faceGroups[""][i].vertices[p]].z;

			vertDataBufferArray[i * 30 + p * 10 + 3] = sphere.vertices[sphere.faceGroups[""][i].normals[p]].x;
			vertDataBufferArray[i * 30 + p * 10 + 4] = sphere.vertices[sphere.faceGroups[""][i].normals[p]].y;
			vertDataBufferArray[i * 30 + p * 10 + 5] = sphere.vertices[sphere.faceGroups[""][i].normals[p]].z;

			vertDataBufferArray[i * 30 + p * 10 + 6] = 1.0f;
			vertDataBufferArray[i * 30 + p * 10 + 7] = 1.0f;
			vertDataBufferArray[i * 30 + p * 10 + 8] = 1.0f;
			vertDataBufferArray[i * 30 + p * 10 + 9] = 1.0f;
		}
	}

	glBufferData(GL_ARRAY_BUFFER, sphere.faceGroups[""].size() * 30 * sizeof(GLfloat), vertDataBufferArray, GL_STATIC_DRAW);

	delete[] vertDataBufferArray;

	glGenBuffers(1, &(vertexBuffers["elements"]));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLuint* vertElementBufferArray = new GLuint[sphere.faceGroups[""].size() * 3];

	for(size_t i = 0; i < sphere.faceGroups[""].size() * 3; ++i) {
		vertElementBufferArray[i] = i;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.faceGroups[""].size() * 3 * sizeof(GLuint), vertElementBufferArray, GL_STATIC_DRAW);

	delete[] vertElementBufferArray;
}

ShellRenderer::~ShellRenderer() {
	// undo vertex buffer setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

void ShellRenderer::execute(DrawStackArgList arguments) {
	// state
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	if(gameGraphics->supportsMultisampling) glEnable(GL_MULTISAMPLE);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_TEXTURE_2D);

	// enable shader
	glUseProgram(gameGraphics->getProgramID("colorLighting"));

	// set uniforms
	glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("colorLighting"), "pMatrix"), 1, GL_FALSE, (gameState->binoculars ? gameGraphics->ppBinoMatrixArray : gameGraphics->ppMatrixArray));
	glUniform3f(glGetUniformLocation(gameGraphics->getProgramID("colorLighting"), "ambientColor"), 0.15f, 0.15f, 0.15f);
	glUniform3f(glGetUniformLocation(gameGraphics->getProgramID("colorLighting"), "diffuseColor"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(gameGraphics->getProgramID("colorLighting"), "specularColor"), 0.5f, 0.5f, 0.5f);
	Vector4 lightPosition = Vector4(1.0f, 1.0f, -1.0f, 0.0f) * gameGraphics->currentCamera->lightMatrix;
	glUniform3f(glGetUniformLocation(gameGraphics->getProgramID("colorLighting"), "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform1f(glGetUniformLocation(gameGraphics->getProgramID("colorLighting"), "shininess"), 50.0f);

	// set the overall drawing state
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorLighting"), "position"), 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*) 0);
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorLighting"), "normal"), 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorLighting"), "color"), 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));

	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorLighting"), "position"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorLighting"), "normal"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorLighting"), "color"));

	// draw the geometry
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	for(size_t i = 0; i < gameState->shells.size(); ++i) {
		Matrix4 mvMatrix; mvMatrix.identity();
		scaleMatrix(gameState->shellRadius, gameState->shellRadius, gameState->shellRadius, mvMatrix);
		translateMatrix(gameState->shells[i].position.x, gameState->shells[i].position.y, gameState->shells[i].position.z, mvMatrix);
		mvMatrix = mvMatrix * gameGraphics->currentCamera->mvMatrix;

		float mvMatrixArray[] = {
				mvMatrix.m11, mvMatrix.m12, mvMatrix.m13, mvMatrix.m14,
				mvMatrix.m21, mvMatrix.m22, mvMatrix.m23, mvMatrix.m24,
				mvMatrix.m31, mvMatrix.m32, mvMatrix.m33, mvMatrix.m34,
				mvMatrix.m41, mvMatrix.m42, mvMatrix.m43, mvMatrix.m44
			};

			glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("colorLighting"), "mvMatrix"), 1, GL_FALSE, mvMatrixArray);

		glDrawElements(GL_TRIANGLES, sphere.faceGroups[""].size() * 3, GL_UNSIGNED_INT, NULL);
	}

	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorLighting"), "position"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorLighting"), "normal"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorLighting"), "color"));
}
