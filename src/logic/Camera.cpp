// Camera.cpp
// Crucible Island

#include "logic/Camera.h"

#include <cmath>
#include <cstdlib>

#include "core/GameSystem.h"
#include "graphics/DrawingMaster.h"
#include "graphics/3dgraphics/FortressRenderer.h"
#include "math/ScalarMath.h"
#include "state/GameState.h"

extern DrawingMaster* drawingMaster;
extern GameState* gameState;
extern GameSystem* gameSystem;

void OrbitCamera::execute() {
	if(gameState == NULL)
		return;

	mvMatrix.identity();
//	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (gameState->lastUpdateGameTime % 32000) / 32000.0f * 360.0f), mvMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-90.0f), mvMatrix);
	translateMatrix(0.0f, 0.0f, gameSystem->getFloat("islandMaximumWidth") * 6.0f, mvMatrix);

	lightMatrix.identity();
//	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (gameState->lastUpdateGameTime % 32000) / 32000.0f * 360.0f), lightMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-90.0f), lightMatrix);
	translateMatrix(0.0f, 0.0f, gameSystem->getFloat("islandMaximumWidth") * 6.0f, lightMatrix);
}

void PresentationCamera::execute() {
	if(gameState == NULL)
		return;

	mvMatrix.identity();
	translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, mvMatrix);
	translateMatrix(0.0f, -5.0f, 0.0f, mvMatrix);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (gameState->lastUpdateGameTime % 16000) / 16000.0f * 360.0f), mvMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f * (sin(PI + (float) (gameState->lastUpdateGameTime % 8000) / 8000.0f * 2.0f * PI) * 0.5f + 0.5f )), mvMatrix);
	translateMatrix(0.0f, 0.0f, 25.0f + (sin(-PI + (float) (gameState->lastUpdateGameTime % 16000) / 16000.0f * 2.0f * PI) * 0.5f + 0.5f ) * 50.0f, mvMatrix);

	lightMatrix.identity();
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (gameState->lastUpdateGameTime % 16000) / 16000.0f * 360.0f), lightMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f * (sin(PI + (float) (gameState->lastUpdateGameTime % 8000) / 8000.0f * 2.0f * PI) * 0.5f + 0.5f )), lightMatrix);
}

void RoamingCamera::execute() {
	if(gameState == NULL)
		return;

	mvMatrix.identity();
	translateMatrix(-position.x, -position.y, -position.z, mvMatrix);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(rotationX), mvMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(rotationY), mvMatrix);

	lightMatrix.identity();
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(rotationX), lightMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(rotationY), lightMatrix);
}

void IntroCamera::execute() {
	if(gameState == NULL)
		return;

	if(gameState->ships.size() == 0)
		return;

	float progression = gameState->lastUpdateGameTime / 1000.0f / gameSystem->getFloat("stateShipEntryTime");

	mvMatrix.identity();
	lightMatrix.identity();

	if(progression < 2.5f / 20.0f) {
		float segmentProgression = progression / (2.5f / 20.0f);

		translateMatrix(
				-gameState->ships[0].position.x,
				-gameState->ships[0].position.y,
				-gameState->ships[0].position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((1.0f - segmentProgression) * -90.0f), mvMatrix);
		translateMatrix(0.0f, -30.0f, pow(1.0f - segmentProgression, 2.0f) * 1200.0f + 400.0f, mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((1.0f - segmentProgression) * -90.0f), lightMatrix);
	} else if(progression < 5.0f / 20.0f) {
		float segmentProgression = (progression - 2.5f / 20.0f) / (2.5f / 20.0f);

		translateMatrix(
				-gameState->ships[0].position.x,
				-gameState->ships[0].position.y,
				-gameState->ships[0].position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(segmentProgression * 90.0f), mvMatrix);
		translateMatrix(0.0f, -30.0f, 400.0f, mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(segmentProgression * 90.0f), lightMatrix);
	} else if(progression < 7.5f / 20.0f) {
		float segmentProgression = (progression - 5.0f / 20.0f) / (2.5f / 20.0f);

		translateMatrix(
				-gameState->ships[0].position.x,
				-gameState->ships[0].position.y,
				-gameState->ships[0].position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(90.0f + segmentProgression * 90.0f), mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(segmentProgression, 2.0f) * -15.0f), mvMatrix);
		translateMatrix(0.0f, -30.0f, pow(segmentProgression, 2.0f) * -50.0f + 400.0f, mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(90.0f + segmentProgression * 90.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(segmentProgression, 2.0f) * -15.0f), lightMatrix);
	} else if(progression < 10.0f / 20.0f) {
		float segmentProgression = (progression - 7.5f / 20.0f) / (2.5f / 20.0f);

		translateMatrix(
				-gameState->ships[0].position.x,
				-gameState->ships[0].position.y,
				-gameState->ships[0].position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(180.0f + segmentProgression * 90.0f), mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(1.0f - segmentProgression, 2.0f) * 15.0f - 30.0f), mvMatrix);
		translateMatrix(0.0f, pow(segmentProgression, 2.0f) * 15.0f - 30.0f, pow(1.0f - segmentProgression, 2.0f) * 50.0f + 300.0f, mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(180.0f + segmentProgression * 90.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(1.0f - segmentProgression, 2.0f) * 15.0f - 30.0f), lightMatrix);
	} else if(progression < 11.25f / 20.0f) {
		float segmentProgression = (progression - 10.0f / 20.0f) / (1.25f / 20.0f);

		translateMatrix(
				-gameState->fortress.position.x,
				-gameState->fortress.position.y,
				-gameState->fortress.position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(270.0f + segmentProgression * 45.0f), mvMatrix);
		translateMatrix(
				(gameState->ships[0].position.z - gameState->fortress.position.z) * (1.0f - pow(segmentProgression, 2.0f) / 2.0f),
				-(gameState->ships[0].position.y - gameState->fortress.position.y) * (1.0f - pow(segmentProgression, 2.0f) / 2.0f),
				-(gameState->ships[0].position.x - gameState->fortress.position.x) * (1.0f - pow(segmentProgression, 2.0f) / 2.0f),
				mvMatrix
			);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(segmentProgression / 2.0f, 2.0) * 15.0f - 30.0f), mvMatrix);
		translateMatrix(0.0f, pow(1.0f - segmentProgression, 2.0f) * -7.5f - 7.5f, pow(segmentProgression, 2.0f) * -150.0f + 300.0f + pow(segmentProgression, 2.0f) * 25.0f, mvMatrix);


		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(270.0f + segmentProgression * 45.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(segmentProgression / 2.0f, 2.0) * 15.0f - 30.0f), lightMatrix);
	} else if(progression < 12.5f / 20.0f) {
		float segmentProgression = (progression - 11.25f / 20.0f) / (1.25f / 20.0f);

		translateMatrix(
				-gameState->fortress.position.x,
				-gameState->fortress.position.y,
				-gameState->fortress.position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(315.0f + segmentProgression * 45.0f), mvMatrix);
		translateMatrix(
				(gameState->ships[0].position.z - gameState->fortress.position.z) * pow(1.0f - segmentProgression, 2.0f) / 2.0f,
				-(gameState->ships[0].position.y - gameState->fortress.position.y) * pow(1.0f - segmentProgression, 2.0f) / 2.0f,
				-(gameState->ships[0].position.x - gameState->fortress.position.x) * pow(1.0f - segmentProgression, 2.0f) / 2.0f,
				mvMatrix
			);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(segmentProgression / 2.0f + 0.5f, 2.0) * 15.0f - 30.0f), mvMatrix);
		translateMatrix(0.0f, pow(1.0f - segmentProgression, 2.0f) * -7.5f, pow(1.0f - segmentProgression, 2.0f) * 150.0f + pow(1.0f - segmentProgression, 2.0f) * -25.0f + 50.0f, mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(315.0f + segmentProgression * 45.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(segmentProgression / 2.0f + 0.5f, 2.0) * 15.0f - 30.0f), lightMatrix);
	} else if(progression < 15.0f / 20.0f) {
		float segmentProgression = (progression - 12.5f / 20.0f) / (2.5f / 20.0f);

		translateMatrix(
				-gameState->fortress.position.x,
				-gameState->fortress.position.y,
				-gameState->fortress.position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(segmentProgression * 90.0f), mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(1.0f - segmentProgression, 2.0) * -15.0f), mvMatrix);
		translateMatrix(0.0f, 0.0f, 50.0f, mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(segmentProgression * 90.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(1.0f - segmentProgression, 2.0) * -15.0f), lightMatrix);
	} else if(progression < 16.25f / 20.0f) {
		float segmentProgression = (progression - 15.0f / 20.0f) / (1.25f / 20.0f);

		translateMatrix(
				-gameState->fortress.position.x,
				-gameState->fortress.position.y,
				-gameState->fortress.position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(90.0f + segmentProgression * 45.0f), mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(segmentProgression, 2.0) * -45.0f), mvMatrix);
		translateMatrix(0.0f, 0.0f, 50.0f, mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(90.0f + segmentProgression * 45.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(segmentProgression, 2.0) * -45.0f), lightMatrix);
	} else if(progression < 17.5f / 20.0f) {
		float segmentProgression = (progression - 16.25f / 20.0f) / (1.25f / 20.0f);

		translateMatrix(
				-gameState->fortress.position.x,
				-gameState->fortress.position.y,
				-gameState->fortress.position.z,
				mvMatrix
			);
		translateMatrix(
				pow(segmentProgression / 2.0f, 2.0f) * -((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.x,
				pow(segmentProgression / 2.0f, 2.0f) * -((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.y,
				pow(segmentProgression / 2.0f, 2.0f) * -((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(135.0f + segmentProgression * 45.0f), mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(1.0f - segmentProgression, 2.0) * 45.0f - 90.0f), mvMatrix);
		translateMatrix(0.0f, 0.0f, 50.0f, mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(135.0f + segmentProgression * 45.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(1.0f - segmentProgression, 2.0) * 45.0f - 90.0f), lightMatrix);
	} else if(progression < 18.75f / 20.0f) {
		float segmentProgression = (progression - 17.5f / 20.0f) / (1.25f / 20.0f);

		translateMatrix(
				-gameState->fortress.position.x,
				-gameState->fortress.position.y,
				-gameState->fortress.position.z,
				mvMatrix
			);
		translateMatrix(
				pow(segmentProgression / 2.0f + 0.5f, 2.0f) * -((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.x,
				pow(segmentProgression / 2.0f + 0.5f, 2.0f) * -((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.y,
				pow(segmentProgression / 2.0f + 0.5f, 2.0f) * -((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(180.0f + segmentProgression * 45.0f), mvMatrix);
		translateMatrix(0.0f, pow(segmentProgression, 2.0f) * 25.0f - 50.0f, 0.0f, mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(segmentProgression, 2.0) * 45.0f - 90.0f), mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(180.0f + segmentProgression * 45.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(segmentProgression, 2.0) * 45.0f - 90.0f), lightMatrix);
	} else if(progression < 20.0f / 20.0f) {
		float segmentProgression = (progression - 18.75f / 20.0f) / (1.25f / 20.0f);

		translateMatrix(
				-gameState->fortress.position.x,
				-gameState->fortress.position.y,
				-gameState->fortress.position.z,
				mvMatrix
			);
		translateMatrix(
				-((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.x,
				-((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.y,
				-((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(225.0f + segmentProgression * 45.0f), mvMatrix);
		translateMatrix(0.0f, pow(1.0f - segmentProgression, 2.0f) * -25.0f, 0.0f, mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(1.0f - segmentProgression, 2.0) * -45.0f), mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(225.0f + segmentProgression * 45.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(pow(1.0f - segmentProgression, 2.0) * -45.0f), lightMatrix);
	} else {
		translateMatrix(
				-gameState->fortress.position.x,
				-gameState->fortress.position.y,
				-gameState->fortress.position.z,
				mvMatrix
			);
		translateMatrix(
				-((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.x,
				-((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.y,
				-((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(270.0f), mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(270.0f), lightMatrix);
	}
}

void FortressCamera::execute() {
	if(gameState == NULL)
		return;

	mvMatrix.identity();
	translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, mvMatrix);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), -radians(gameState->fortress.rotation), mvMatrix);
	translateMatrix(
			-((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.x,
			-((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.y,
			-((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.z,
			mvMatrix
		);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(270.0f), mvMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(gameState->fortress.tilt), mvMatrix);

	lightMatrix.identity();
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), -radians(gameState->fortress.rotation), lightMatrix);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(270.0f), lightMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(gameState->fortress.tilt), lightMatrix);
}
