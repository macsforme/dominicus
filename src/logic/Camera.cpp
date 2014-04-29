// Camera.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "logic/Camera.h"

#include "graphics/GameGraphics.h"
#include "graphics/3dgraphics/TowerRenderer.h"

extern GameGraphics* gameGraphics;

void OrbitCamera::execute() {
	if(gameState == NULL)
		return;

	mvMatrix.identity();
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 32000) / 32000.0f * 360.0f), mvMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f), mvMatrix);
	translateMatrix(0.0f, 0.0f, gameSystem->getFloat("islandMaximumWidth") * 0.75f, mvMatrix);

	lightMatrix.identity();
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 32000) / 32000.0f * 360.0f), lightMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f), lightMatrix);
	translateMatrix(0.0f, 0.0f, gameSystem->getFloat("islandMaximumWidth") * 0.75f, lightMatrix);
}

void PresentationCamera::execute() {
	if(gameState == NULL)
		return;

	mvMatrix.identity();
	translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, mvMatrix);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 32000) / 32000.0f * 360.0f), mvMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f * (sin(PI + (float) (platform->getExecMills() % 16000) / 16000.0f * 2.0f * PI) * 0.5f + 0.5f )), mvMatrix);
	translateMatrix(0.0f, 0.0f, 25.0f + (sin(-PI + (float) (platform->getExecMills() % 32000) / 32000.0f * 2.0f * PI) * 0.5f + 0.5f ) * 200.0f, mvMatrix);

	lightMatrix.identity();
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((float) (platform->getExecMills() % 32000) / 32000.0f * 360.0f), lightMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-30.0f * (sin(PI + (float) (platform->getExecMills() % 16000) / 16000.0f * 2.0f * PI) * 0.5f + 0.5f )), lightMatrix);
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

void TowerCamera::execute() {
	if(gameState == NULL)
		return;

	mvMatrix.identity();
	translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, mvMatrix);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), -radians((float) (gameState->getGameMills() % 8000) / 8000.0f * 360.0f), mvMatrix);
	translateMatrix(-((TowerRenderer*) gameGraphics->drawers["towerRenderer"])->turretOrigin.x, -((TowerRenderer*) gameGraphics->drawers["towerRenderer"])->turretOrigin.y, -((TowerRenderer*) gameGraphics->drawers["towerRenderer"])->turretOrigin.z, mvMatrix);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), -radians(90.0f), mvMatrix);
	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians((sin(-PI + (float) (gameState->getGameMills() % 4000) / 4000.0f * 2.0f * PI) * 0.5f + 0.5f ) * 45.0f), mvMatrix);
translateMatrix(0.0f, -1.0f, -2.0f, mvMatrix);
//	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(30.0f * (sin(PI + (float) (platform->getExecMills() % 16000) / 16000.0f * 2.0f * PI) * 0.5f + 0.5f )), mvMatrix);

	lightMatrix.identity();
}
