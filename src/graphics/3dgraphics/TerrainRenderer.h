// TerrainRenderer.h
// Crucible Island

#ifndef TERRAINRENDERER_H
#define TERRAINRENDERER_H

#include <map>
#include <string>

#include "graphics/DrawTypes.h"
#include "platform/OpenGLHeaders.h"

class TerrainRenderer : public BaseDrawNode {
private:
	GLuint noiseTextureID;

public:
	TerrainRenderer();
	~TerrainRenderer();

	void reloadState();

	void execute(std::map<std::string, void*> arguments);
};

#endif // TERRAINRENDERER_H
