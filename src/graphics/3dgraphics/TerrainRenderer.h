// TerrainRenderer.h
// Crucible Island

#ifndef TERRAINRENDERER_H
#define TERRAINRENDERER_H

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

// class definition
class TerrainRenderer : public BaseDrawNode {
private:
	GLuint noiseTextureID;

public:
	TerrainRenderer();
	~TerrainRenderer();

	void reloadGraphics();

	void execute(std::map<std::string, void*> arguments);
};

#endif // TERRAINRENDERER_H
