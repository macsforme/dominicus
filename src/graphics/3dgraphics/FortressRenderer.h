// FortressRenderer.h
// Crucible Island

#ifndef FORTRESSRENDERER_H
#define FORTRESSRENDERER_H

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
class FortressRenderer : public BaseDrawNode {
	// internal model data
public:
	Vector3 turretOrigin;
	Vector3 cameraOrigin;

private:
	Mesh fortressMesh;

public:
	FortressRenderer();
	~FortressRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif // FORTRESSRENDERER_H
