// MissileRenderer.h
// Dominicus

#ifndef MISSILERENDERER_H
#define MISSILERENDERER_H

#include "geometry/Mesh.h"
#include "graphics/DrawTypes.h"

class MissileRenderer : public BaseDrawNode {
private:
	Mesh missileMesh;

public:
	MissileRenderer();
	~MissileRenderer();

	void execute(DrawStackArgList arguments);
};

#endif // MISSILERENDERER_H
