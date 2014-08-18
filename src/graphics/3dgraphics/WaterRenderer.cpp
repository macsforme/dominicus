// WaterRenderer.cpp
// Crucible Island

#include "graphics/3dgraphics/WaterRenderer.h"

#include <vector>

#include "geometry/Mesh.h"
#include "graphics/GameGraphics.h"
#include "logic/Camera.h"
#include "math/MatrixMath.h"
#include "math/MiscMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "state/GameState.h"

extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern Platform* platform;

WaterRenderer::WaterRenderer() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "water"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "water"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["timer"] = glGetUniformLocation(shaderProgram, "timer");
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");
	uniforms["fortressTransformMatrix"] = glGetUniformLocation(shaderProgram, "fortressTransformMatrix");
	uniforms["insideColorMultiplier"] = glGetUniformLocation(shaderProgram, "insideColorMultiplier");
	uniforms["outsideColorMultiplier"] = glGetUniformLocation(shaderProgram, "outsideColorMultiplier");
	uniforms["colorChangeRadius"] = glGetUniformLocation(shaderProgram, "colorChangeRadius");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	float vDist = gameSystem->getFloat("renderingPerspectiveFarClip") * 3.0f;

	GLfloat vertDataBufferArray[] = {
			-vDist, 0.0f, -vDist,
			-vDist, 0.0f, vDist,
			vDist, 0.0f, vDist,
			vDist, 0.0f, -vDist
		};

	GLushort vertElementBufferArray[] = {
			0, 1, 2, 3
		};

	// send the buffer data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertDataBufferArray), vertDataBufferArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertElementBufferArray), vertElementBufferArray,
			GL_STATIC_DRAW);
}

WaterRenderer::~WaterRenderer() {
	// undo shader setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));

	if(! glIsShader(shaderProgram)) // sometimes duplicate shaders get optimized out so check for validity
		return;

	GLsizei shaderCount;
	GLuint* shaders = new GLuint[2];
	glGetAttachedShaders(shaderProgram, 2, &shaderCount, shaders);

	for(size_t i = 0; i < shaderCount; ++i) {
		glDetachShader(shaderProgram, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	delete[] shaders;

	glDeleteProgram(shaderProgram);
}

void WaterRenderer::execute(std::map<std::string, void*> arguments) {
	// prepare variables
	Matrix4 mvpMatrix = gameGraphics->currentCamera->mvMatrix;
	mvpMatrix *= (gameState->binoculars ? gameGraphics->ppBinoMatrix : gameGraphics->ppMatrix);
	float mvpMatrixArray[] = {
			mvpMatrix.m11, mvpMatrix.m12, mvpMatrix.m13, mvpMatrix.m14,
			mvpMatrix.m21, mvpMatrix.m22, mvpMatrix.m23, mvpMatrix.m24,
			mvpMatrix.m31, mvpMatrix.m32, mvpMatrix.m33, mvpMatrix.m34,
			mvpMatrix.m41, mvpMatrix.m42, mvpMatrix.m43, mvpMatrix.m44
		};

	Matrix4 fortressTransformMatrix; fortressTransformMatrix.identity();
	translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, fortressTransformMatrix);
	float fortressTransformMatrixArray[] = {
			fortressTransformMatrix.m11, fortressTransformMatrix.m12, fortressTransformMatrix.m13, fortressTransformMatrix.m14,
			fortressTransformMatrix.m21, fortressTransformMatrix.m22, fortressTransformMatrix.m23, fortressTransformMatrix.m24,
			fortressTransformMatrix.m31, fortressTransformMatrix.m32, fortressTransformMatrix.m33, fortressTransformMatrix.m34,
			fortressTransformMatrix.m41, fortressTransformMatrix.m42, fortressTransformMatrix.m43, fortressTransformMatrix.m44
		};

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniform1f(uniforms["timer"], (float) (gameState->getGameMills() % 3000) / 3000.0f);
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, mvpMatrixArray);
	glUniformMatrix4fv(uniforms["fortressTransformMatrix"], 1, GL_FALSE, fortressTransformMatrixArray);
	glUniform4f(uniforms["insideColorMultiplier"], 1.0f, 1.0f, 1.0f, 1.0f);
	float empColorMultiplier = (
			gameState->fortress.emp <= 0.0f || gameState->fortress.emp >= 1.0f ?
			1.0f :
			1.0f - gameState->fortress.emp + gameState->fortress.emp * gameSystem->getFloat("empColorMultiplier")
		);
	glUniform4f(
			uniforms["outsideColorMultiplier"],
			empColorMultiplier,
			empColorMultiplier,
			empColorMultiplier,
			1.0f
		);
	glUniform1f(uniforms["colorChangeRadius"],
			gameState->fortress.emp <= 0.0f || gameState->fortress.emp >= 1.0f ?
			0.0f :
			(1.0f - gameState->fortress.emp) * gameSystem->getFloat("stateEMPRange")
		);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);

	glEnableVertexAttribArray(attributes["position"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLvoid*) 0);

	glDisableVertexAttribArray(attributes["position"]);

	// undo state
	glDisable(GL_BLEND);
}
