// TowerRenderer.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef TOWERRENDERER_H
#define TOWERRENDERER_H

// program headers
#include "core/GameSystem.h"
#include "geometry/Mesh.h"
#include "graphics/GameGraphics.h"
#include "logic/Camera.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "state/GameState.h"

// library headers
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

// global variables
extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;
extern Platform* platform;

// class definition
class TowerRenderer : public BaseDrawNode {
	// internal model data
public:
	Vector3 turretOrigin;
	Vector3 cameraOrigin;

private:
	Mesh towerMesh;

public:
	TowerRenderer();
	~TowerRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif
