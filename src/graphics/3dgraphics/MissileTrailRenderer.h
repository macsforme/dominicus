// MissileRenderer.h
// Crucible Island

#ifndef MISSILETRAILRENDERER_H
#define MISSILETRAILRENDERER_H

#include <map>
#include <string>

#include "geometry/Mesh.h"
#include "graphics/DrawTypes.h"

class MissileTrailRenderer : public BaseDrawNode {
private:
	Mesh missileMesh;

	GLuint noiseTextureID;

public:
	MissileTrailRenderer();
	~MissileTrailRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif // MISSILETRAILRENDERER_H
