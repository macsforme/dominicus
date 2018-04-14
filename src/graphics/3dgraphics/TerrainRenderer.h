// TerrainRenderer.h
// Dominicus

#ifndef TERRAINRENDERER_H
#define TERRAINRENDERER_H

#include "graphics/DrawTypes.h"
#include "platform/OpenGLHeaders.h"

class TerrainRenderer : public BaseDrawNode {
private:
	GLuint noiseTextureID;

public:
	TerrainRenderer();
	~TerrainRenderer();

	void reloadState();

	void execute(DrawStackArgList arguments);
};

#endif // TERRAINRENDERER_H
