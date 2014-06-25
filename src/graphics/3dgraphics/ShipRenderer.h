// ShipRenderer.h
// Crucible Island

#ifndef SHIPRENDERER_H
#define SHIPRENDERER_H

#include <map>
#include <string>

#include "geometry/Mesh.h"
#include "graphics/DrawTypes.h"

class ShipRenderer : public BaseDrawNode {
private:
	Mesh shipMesh;

public:
	ShipRenderer();
	~ShipRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif // SHIPRENDERER_H
