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

	if(progression < 2.0f / 20.0f) {
		float segmentProgression = progression / (2.0f / 20.0f);

		translateMatrix(
				-gameState->ships[0].position.x,
				-gameState->ships[0].position.y,
				-gameState->ships[0].position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(-90.0f), mvMatrix);
		translateMatrix(0.0f, -10.0f, (1.0f - segmentProgression) * 800.0f + 400.0f, mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(-90.0f), lightMatrix);
	} else if(progression < 4.0f / 20.0f) {
		float segmentProgression = (progression - 2.0f / 20.0f) / (2.0f / 20.0f);

		translateMatrix(
				-gameState->ships[0].position.x,
				-gameState->ships[0].position.y,
				-gameState->ships[0].position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((1.0f - segmentProgression) * -90.0f), mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(segmentProgression * -30.0f), mvMatrix);
		translateMatrix(0.0f, -10.0f, 400.0f, mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((1.0f - segmentProgression) * -90.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(segmentProgression * -30.0f), lightMatrix);
	} else if(progression < 6.0f / 20.0f) {
		float segmentProgression = (progression - 4.0f / 20.0f) / (2.0f / 20.0f);

		translateMatrix(
				-gameState->ships[0].position.x,
				-gameState->ships[0].position.y,
				-gameState->ships[0].position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(segmentProgression * 180.0f), mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f), mvMatrix);
		translateMatrix(0.0f, -10.0f, segmentProgression * 200.0f + 400.0f, mvMatrix);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(segmentProgression * 180.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f), lightMatrix);
	} else if(progression < 9.0f / 20.0f) {
		float segmentProgression = (progression - 6.0f / 20.0f) / (3.0f / 20.0f);

		translateMatrix(
				(1.0f - segmentProgression) * -gameState->ships[0].position.x,
				(1.0f - segmentProgression) * -gameState->ships[0].position.y + segmentProgression * -gameState->fortress.position.y,
				(1.0f - segmentProgression) * -gameState->ships[0].position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(180.0f + segmentProgression * 90.0f), mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians((1.0f - segmentProgression) * -30.0f), mvMatrix);
		translateMatrix(
				0.0f,
				(1.0f - segmentProgression) * -10.0f,
				(1.0f - segmentProgression) * 100.0f + 500.0f,
				mvMatrix
			);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(180.0f + segmentProgression * 90.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians((1.0f - segmentProgression) * -30.0f), lightMatrix);
	} else if(progression < 12.0f / 20.0f) {
		float segmentProgression = (progression - 9.0f / 20.0f) / (3.0f / 20.0f);

		translateMatrix(
				segmentProgression * -gameState->fortress.position.x,
				-gameState->fortress.position.y,
				segmentProgression * -gameState->fortress.position.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(270.0f + segmentProgression * 135.0f), mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(segmentProgression * -30.0f), mvMatrix);
		translateMatrix(
				0.0f,
				0.0f,
				(1.0f - segmentProgression) * 450.0f + 50.0f,
				mvMatrix
			);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(270.0f + segmentProgression * 135.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(segmentProgression * -30.0f), lightMatrix);
	} else if(progression < 16.0f / 20.0f) {
		float segmentProgression = (progression - 12.0f / 20.0f) / (4.0f / 20.0f);

		translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, mvMatrix);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(45.0f + segmentProgression * 90.0f), mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f), mvMatrix);
		translateMatrix(
				0.0f,
				0.0f,
				50.0f,
				mvMatrix
			);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(45.0f + segmentProgression * 90.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f), lightMatrix);
	} else if(progression < 18.0f / 20.0f) {
		float segmentProgression = (progression - 16.0f / 20.0f) / (2.0f / 20.0f);

		translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, mvMatrix);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(135.0f + segmentProgression * 135.0f), mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f + segmentProgression * -60.0f), mvMatrix);
		translateMatrix(
				0.0f,
				0.0f,
				(1.0f - segmentProgression) * 25.0f + 25.0f,
				mvMatrix
			);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(135.0f + segmentProgression * 135.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f + segmentProgression * -60.0f), lightMatrix);
	} else if(progression < 19.5f / 20.0f) {
		float segmentProgression = (progression - 18.0f / 20.0f) / (1.5f / 20.0f);

		translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, mvMatrix);
		translateMatrix(
				segmentProgression * -((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.x,
				segmentProgression * -((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.y,
				segmentProgression * -((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(270.0f), mvMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians((1.0f - segmentProgression) * -90.0f), mvMatrix);
		translateMatrix(
				0.0f,
				segmentProgression * -5.0f,
				(1.0f - segmentProgression) * 25.0f,
				mvMatrix
			);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(270.0f), lightMatrix);
		rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians((1.0f - segmentProgression) * -90.0f), lightMatrix);
	} else if(progression < 20.0f / 20.0f) {
		float segmentProgression = (progression - 19.5f / 20.0f) / (0.5f / 20.0f);

		translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, mvMatrix);
		translateMatrix(
				-((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.x,
				-((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.y,
				-((FortressRenderer*) drawingMaster->drawers["fortressRenderer"])->cameraOrigin.z,
				mvMatrix
			);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(270.0f), mvMatrix);
		translateMatrix(
				0.0f,
				(1.0f - segmentProgression) * -5.0f,
				0.0f,
				mvMatrix
			);

		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(270.0f), lightMatrix);
	} else {
		translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, mvMatrix);
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
