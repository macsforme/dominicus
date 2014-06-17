// WaterRenderer.h
// Crucible Island

#ifndef WATERRENDERER_H
#define WATERRENDERER_H

// definitions
#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif

// program headers
#include "geometry/Mesh.h"
#include "graphics/GameGraphics.h"
#include "logic/Camera.h"
#include "math/MatrixMath.h"
#include "math/MiscMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "state/GameState.h"
//#include "state/Ship.h"

// library headers
#include <map>
#include <sstream>
#include <string>
#include <vector>

// global variables
extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern Platform* platform;

// class definition
class WaterRenderer : public BaseDrawNode {
public:
	WaterRenderer();
	~WaterRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif // WATERRENDERER_H
