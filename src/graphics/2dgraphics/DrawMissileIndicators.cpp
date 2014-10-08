// DrawMissileIndicators.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawMissileIndicators.h"

#include <cstdlib>
#include <vector>

#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "math/MatrixMath.h"
#include "math/MiscMath.h"
#include "math/ScalarMath.h"
#include "platform/OpenGLHeaders.h"
#include "state/GameState.h"

extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;

DrawMissileIndicators::DrawMissileIndicators(DrawRoundedTriangle* roundedTriangleDrawer) :
		roundedTriangleDrawer(roundedTriangleDrawer) {
	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));
}

DrawMissileIndicators::~DrawMissileIndicators() {
	// undo vertex buffer setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

DrawStackArgList DrawMissileIndicators::instantiateArgList() {
	DrawStackArgList argList;

	argList["color"] = (void*) new Vector4;			// color of indicators
	argList["size"] = (void*) new Vector2;			// size of indicators in screen dimensions
	argList["arrowColor"] = (void*) new Vector4;	// color of arrows
	argList["arrowSize"] = (void*) new Vector2;		// size of arrows in screen dimensions
	return argList;
}

void DrawMissileIndicators::deleteArgList(DrawStackArgList argList) {
	if(argList.find("color") != argList.end()) delete (Vector4*) argList["color"];
	if(argList.find("size") != argList.end()) delete (Vector2*) argList["size"];
	if(argList.find("arrowColor") != argList.end()) delete (Vector4*) argList["arrowColor"];
	if(argList.find("arrowSize") != argList.end()) delete (Vector2*) argList["arrowSize"];
}

void DrawMissileIndicators::execute(DrawStackArgList argList) {
	// collect arguments
	Vector4 color = *((Vector4*) argList["color"]);
	Vector2 size = *((Vector2*) argList["size"]);
	Vector4 arrowInsideColor = *((Vector4*) argList["arrowColor"]);
	Vector2 arrowSize = *((Vector2*) argList["arrowSize"]);

	// determine active missile screen positions
	std::vector<Vector2> frontMissilePositions;
	std::vector<Vector2> allMissilePositions;

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
			frontMissilePositions.push_back(Vector2(thisMissilePosition.x / thisMissilePosition.w, thisMissilePosition.y / thisMissilePosition.w));

		if(thisMissilePosition.z >= 0.0f) {
			allMissilePositions.push_back(Vector2(thisMissilePosition.x / thisMissilePosition.w, thisMissilePosition.y / thisMissilePosition.w));
		} else {
//			if(gameState->lastUpdateGameTime % (unsigned int) (gameSystem->getFloat("hudMissileArrowBlinkRate") * 1000.0f) > (unsigned int) (gameSystem->getFloat("hudMissileArrowBlinkRate") * 1000.0f) / 2)
//				continue;

			if(thisMissilePosition.x / thisMissilePosition.w >= 0.0f)
				allMissilePositions.push_back(Vector2(-2.0f - thisMissilePosition.x / thisMissilePosition.w, -thisMissilePosition.y / thisMissilePosition.w));
			else
				allMissilePositions.push_back(Vector2(2.0f - thisMissilePosition.x / thisMissilePosition.w, -thisMissilePosition.y / thisMissilePosition.w));
		}
	}

	// draw locator arrows
	Vector4 arrowOutsideColor(
			arrowInsideColor.x,
			arrowInsideColor.y,
			arrowInsideColor.z,
			0.0f
		);
	Vector2 arrowPosition(0.0f, 0.0f);
	float arrowRotation = 0.0f;
	float arrowSoftEdge = 2.0f;

	DrawStackArgList drawerArguments;
	drawerArguments["size"] = (void*) &arrowSize;
	drawerArguments["position"] = (void*) &arrowPosition;
	drawerArguments["rotation"] = (void*) &arrowRotation;
	drawerArguments["softEdge"] = (void*) &arrowSoftEdge;
	drawerArguments["insideColor"] = (void*) &arrowInsideColor;
	drawerArguments["outsideColor"] = (void*) &arrowOutsideColor;

	float arrowPositionRadius = gameSystem->getFloat("hudControlAreaRadius") * 2.0f / (float) gameGraphics->resolutionY;
	for(size_t i = 0; i < allMissilePositions.size(); ++i) {
		if(
				mag(Vector2(allMissilePositions[i].x * gameGraphics->aspectRatio, allMissilePositions[i].y)) <
				arrowPositionRadius + arrowSize.y
			)
			continue;

		Vector4 thisArrowPosition(arrowPositionRadius, 0.0f, 0.0f, 0.0f);
		Matrix4 rotationMatrix; rotationMatrix.identity();
		float thisArrowRotation = getAngle(allMissilePositions[i]);
		rotateMatrix(Vector3(0.0f, 0.0f, 1.0f), radians(-thisArrowRotation), rotationMatrix);
		thisArrowPosition = thisArrowPosition * rotationMatrix;
		float correctAspectRatioMagnitude = mag(Vector2(thisArrowPosition.x * gameGraphics->aspectRatio, thisArrowPosition.y));
		if(correctAspectRatioMagnitude > arrowPositionRadius) {
			thisArrowPosition.x *= arrowPositionRadius / correctAspectRatioMagnitude;
			thisArrowPosition.y *= arrowPositionRadius / correctAspectRatioMagnitude;
		}
		arrowPosition = Vector2(thisArrowPosition.x, thisArrowPosition.y);

		arrowRotation = -thisArrowRotation - 90.0f;

		roundedTriangleDrawer->execute(drawerArguments);
	}

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	GLfloat* vertexBufferArray = new GLfloat[frontMissilePositions.size() * 28];

	for(size_t i = 0; i < frontMissilePositions.size(); ++i) {
		vertexBufferArray[i * 28 + 0] = frontMissilePositions[i].x - size.x / 2.0f;
		vertexBufferArray[i * 28 + 1] = frontMissilePositions[i].y - size.y / 2.0f;
		vertexBufferArray[i * 28 + 2] = 0.0f;
		vertexBufferArray[i * 28 + 3] = color.x;
		vertexBufferArray[i * 28 + 4] = color.y;
		vertexBufferArray[i * 28 + 5] = color.z;
		vertexBufferArray[i * 28 + 6] = color.w;

		vertexBufferArray[i * 28 + 7] = frontMissilePositions[i].x - size.x / 2.0f;
		vertexBufferArray[i * 28 + 8] = frontMissilePositions[i].y + size.y / 2.0f;
		vertexBufferArray[i * 28 + 9] = 0.0f;
		vertexBufferArray[i * 28 + 10] = color.x;
		vertexBufferArray[i * 28 + 11] = color.y;
		vertexBufferArray[i * 28 + 12] = color.z;
		vertexBufferArray[i * 28 + 13] = color.w;

		vertexBufferArray[i * 28 + 14] = frontMissilePositions[i].x + size.x / 2.0f;
		vertexBufferArray[i * 28 + 15] = frontMissilePositions[i].y + size.y / 2.0f;
		vertexBufferArray[i * 28 + 16] = 0.0f;
		vertexBufferArray[i * 28 + 17] = color.x;
		vertexBufferArray[i * 28 + 18] = color.y;
		vertexBufferArray[i * 28 + 19] = color.z;
		vertexBufferArray[i * 28 + 20] = color.w;

		vertexBufferArray[i * 28 + 21] = frontMissilePositions[i].x + size.x / 2.0f;
		vertexBufferArray[i * 28 + 22] = frontMissilePositions[i].y - size.y / 2.0f;
		vertexBufferArray[i * 28 + 23] = 0.0f;
		vertexBufferArray[i * 28 + 24] = color.x;
		vertexBufferArray[i * 28 + 25] = color.y;
		vertexBufferArray[i * 28 + 26] = color.z;
		vertexBufferArray[i * 28 + 27] = color.w;
	}

	glBufferData(GL_ARRAY_BUFFER, frontMissilePositions.size() * 28 * sizeof(GLfloat), vertexBufferArray, GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLushort* elementBufferArray = new GLushort[frontMissilePositions.size() * 4];

	for(size_t i = 0; i < frontMissilePositions.size() * 4; ++i)
		elementBufferArray[i] = i;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, frontMissilePositions.size() * 4 * sizeof(GLushort), elementBufferArray,
			GL_STATIC_DRAW);

	delete[] elementBufferArray;

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

	for(size_t i = 0; i < frontMissilePositions.size(); ++i) {
		glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("color"), "position"), 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*) (i * 28 * sizeof(GLfloat)));
		glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("color"), "color"), 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat) + i * 28 * sizeof(GLfloat)));

		glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("color"), "position"));
		glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("color"), "color"));

		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, NULL);

		glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("color"), "position"));
		glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("color"), "color"));
	}
}
