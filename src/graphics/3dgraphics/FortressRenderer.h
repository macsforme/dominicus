// FortressRenderer.h
// Crucible Island

#ifndef FORTRESSRENDERER_H
#define FORTRESSRENDERER_H

#include <map>
#include <string>

#include "geometry/Mesh.h"
#include "graphics/DrawTypes.h"
#include "math/VectorMath.h"

class FortressRenderer : public BaseDrawNode {
private:
	Mesh fortressMesh;

public:
	Vector3 turretOrigin;
	Vector3 cameraOrigin;

	FortressRenderer();
	~FortressRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif // FORTRESSRENDERER_H
