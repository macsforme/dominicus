// MissileTrailRenderer.h
// Crucible Island

#ifndef MISSILETRAILRENDERER_H
#define MISSILETRAILRENDERER_H

#include "geometry/Mesh.h"
#include "graphics/DrawTypes.h"
#include "platform/OpenGLHeaders.h"

class MissileTrailRenderer : public BaseDrawNode {
private:
	Mesh missileMesh;

	GLuint noiseTextureID;

public:
	MissileTrailRenderer();
	~MissileTrailRenderer();

	void execute(DrawStackArgList arguments);
};

#endif // MISSILETRAILRENDERER_H
