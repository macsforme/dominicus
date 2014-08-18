// DrawMissileIndicators.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawMissileIndicators.h"

#include <vector>

#include "graphics/GameGraphics.h"
#include "graphics/UILayoutAuthority.h"
#include "math/MatrixMath.h"
#include "platform/OpenGLHeaders.h"
#include "state/GameState.h"

extern GameGraphics* gameGraphics;
extern GameState* gameState;

DrawMissileIndicators::DrawMissileIndicators() {
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
}

DrawMissileIndicators::~DrawMissileIndicators() {
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

DrawStackArgList DrawMissileIndicators::instantiateArgList() {
	DrawStackArgList argList;

	argList["color"] = (void*) new Vector4;		// color of indicators
	argList["size"] = (void*) new Vector2;		// size of indicators in screen dimensions
	return argList;
}

void DrawMissileIndicators::deleteArgList(DrawStackArgList argList) {
	if(argList.find("color") != argList.end()) delete (Vector4*) argList["color"];
	if(argList.find("size") != argList.end()) delete (Vector2*) argList["size"];
}

void DrawMissileIndicators::execute(DrawStackArgList argList) {
	// collect arguments
	Vector4 color = *((Vector4*) argList["color"]);
	Vector2 size = *((Vector2*) argList["size"]);

	// determine active missile screen positions
	std::vector<Vector2> missilePositions;

	for(size_t i = 0; i < gameState->missiles.size(); ++i) {
		if(! gameState->missiles[i].alive)
			continue;

		Vector4 halfDirectionalVector(gameState->missileLength / 2.0f, 0.0f, 0.0f, 0.0f);

		Matrix4 missileDirectionMatrix; missileDirectionMatrix.identity();
		rotateMatrix(Vector3(0.0f, 0.0f, 1.0f), radians(gameState->missiles[i].tilt), missileDirectionMatrix);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(gameState->missiles[i].rotation), missileDirectionMatrix);

		halfDirectionalVector = halfDirectionalVector * missileDirectionMatrix;

		Vector4 thisMissilePosition(
				gameState->missiles[i].position.x + halfDirectionalVector.x,
				gameState->missiles[i].position.y + halfDirectionalVector.y,
				gameState->missiles[i].position.z + halfDirectionalVector.z,
				1.0f
			);

		thisMissilePosition = thisMissilePosition * gameGraphics->currentCamera->mvMatrix * (gameState->binoculars ? gameGraphics->ppBinoMatrix : gameGraphics->ppMatrix);

		if(
				absolute(thisMissilePosition.x / thisMissilePosition.w) <= 1.0f &&
				absolute(thisMissilePosition.y / thisMissilePosition.w) <= 1.0f &&
				thisMissilePosition.z >= 0.0f
			)
			missilePositions.push_back(Vector2(thisMissilePosition.x / thisMissilePosition.w, thisMissilePosition.y / thisMissilePosition.w));
	}

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	GLfloat* vertexBufferArray = new GLfloat[missilePositions.size() * 28];

	for(size_t i = 0; i < missilePositions.size(); ++i) {
		vertexBufferArray[i * 28 + 0] = missilePositions[i].x - size.x / 2.0f;
		vertexBufferArray[i * 28 + 1] = missilePositions[i].y - size.y / 2.0f;
		vertexBufferArray[i * 28 + 2] = 0.0f;
		vertexBufferArray[i * 28 + 3] = color.x;
		vertexBufferArray[i * 28 + 4] = color.y;
		vertexBufferArray[i * 28 + 5] = color.z;
		vertexBufferArray[i * 28 + 6] = color.w;

		vertexBufferArray[i * 28 + 7] = missilePositions[i].x - size.x / 2.0f;
		vertexBufferArray[i * 28 + 8] = missilePositions[i].y + size.y / 2.0f;
		vertexBufferArray[i * 28 + 9] = 0.0f;
		vertexBufferArray[i * 28 + 10] = color.x;
		vertexBufferArray[i * 28 + 11] = color.y;
		vertexBufferArray[i * 28 + 12] = color.z;
		vertexBufferArray[i * 28 + 13] = color.w;

		vertexBufferArray[i * 28 + 14] = missilePositions[i].x + size.x / 2.0f;
		vertexBufferArray[i * 28 + 15] = missilePositions[i].y + size.y / 2.0f;
		vertexBufferArray[i * 28 + 16] = 0.0f;
		vertexBufferArray[i * 28 + 17] = color.x;
		vertexBufferArray[i * 28 + 18] = color.y;
		vertexBufferArray[i * 28 + 19] = color.z;
		vertexBufferArray[i * 28 + 20] = color.w;

		vertexBufferArray[i * 28 + 21] = missilePositions[i].x + size.x / 2.0f;
		vertexBufferArray[i * 28 + 22] = missilePositions[i].y - size.y / 2.0f;
		vertexBufferArray[i * 28 + 23] = 0.0f;
		vertexBufferArray[i * 28 + 24] = color.x;
		vertexBufferArray[i * 28 + 25] = color.y;
		vertexBufferArray[i * 28 + 26] = color.z;
		vertexBufferArray[i * 28 + 27] = color.w;
	}

	glBufferData(GL_ARRAY_BUFFER, missilePositions.size() * 28 * sizeof(GLfloat), vertexBufferArray, GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLushort* elementBufferArray = new GLushort[missilePositions.size() * 4];

	for(size_t i = 0; i < missilePositions.size() * 4; ++i)
		elementBufferArray[i] = i;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, missilePositions.size() * 4 * sizeof(GLushort), elementBufferArray,
			GL_STATIC_DRAW);

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, gameGraphics->idMatrixArray);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	for(size_t i = 0; i < missilePositions.size(); ++i) {
		glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*) (i * 28 * sizeof(GLfloat)));
		glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat) + i * 28 * sizeof(GLfloat)));

		glEnableVertexAttribArray(attributes["position"]);
		glEnableVertexAttribArray(attributes["color"]);

		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, NULL);

		glDisableVertexAttribArray(attributes["position"]);
		glDisableVertexAttribArray(attributes["color"]);
	}

	// undo state
	glDisable(GL_BLEND);
}
