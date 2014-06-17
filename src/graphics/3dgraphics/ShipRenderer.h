// ShipRenderer.h
// Crucible Island

#ifndef SHIPRENDERER_H
#define SHIPRENDERER_H

// program headers
#include "core/GameSystem.h"
#include "geometry/Mesh.h"
#include "graphics/GameGraphics.h"
#include "logic/Camera.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"

// library headers
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

// global variables
extern GameGraphics* gameGraphics;
extern GameSystem* gameSystem;
extern Platform* platform;

// class definition
class ShipRenderer : public BaseDrawNode {
private:
	// internal model data
	Mesh shipMesh;

public:
	ShipRenderer();
	~ShipRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif // SHIPRENDERER_H
