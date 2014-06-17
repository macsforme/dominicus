// SkyRenderer.h
// Crucible Island

#ifndef SKYRENDERER_H
#define SKYRENDERER_H

// program headers
#include "graphics/GameGraphics.h"
#include "logic/Camera.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "state/GameState.h"

// library headers
#include <map>
#include <sstream>
#include <string>
#include <vector>

// global variables
extern GameGraphics* gameGraphics;
extern GameState* gameState;

// class definition
class SkyRenderer : public BaseDrawNode {
public:
	SkyRenderer();
	~SkyRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif // SKYRENDERER_H
