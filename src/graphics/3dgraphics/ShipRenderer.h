// ShipRenderer.h
// Crucible Island

#ifndef SHIPRENDERER_H
#define SHIPRENDERER_H

#include "geometry/Mesh.h"
#include "graphics/DrawTypes.h"

class ShipRenderer : public BaseDrawNode {
private:
	Mesh shipMesh;

public:
	ShipRenderer();
	~ShipRenderer();

	void execute(DrawStackArgList arguments);
};

#endif // SHIPRENDERER_H
