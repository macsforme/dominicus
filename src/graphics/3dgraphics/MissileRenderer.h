// MissileRenderer.h
// Crucible Island

#ifndef MISSILERENDERER_H
#define MISSILERENDERER_H

#include <map>
#include <string>

#include "geometry/Mesh.h"
#include "graphics/DrawTypes.h"

class MissileRenderer : public BaseDrawNode {
private:
	Mesh missileMesh;

public:
	MissileRenderer();
	~MissileRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif // MISSILERENDERER_H
