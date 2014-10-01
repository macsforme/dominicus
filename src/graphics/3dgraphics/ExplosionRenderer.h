// ExplosionRenderer.h
// Crucible Island

#ifndef EXPLOSIONRENDERER_H
#define EXPLOSIONRENDERER_H

#include <cstdlib>
#include <vector>

#include "geometry/Mesh.h"
#include "graphics/DrawTypes.h"
#include "math/VectorMath.h"

class ExplosionRenderer : public BaseDrawNode {
private:
	Mesh sphere;

	std::vector<size_t> activeMissiles;

	struct Explosion {
		unsigned int beginTime;
		unsigned int duration;
		float radius;
		Vector3 position;
		Vector3 movement;
	};
	struct ExplodingMissile {
		Explosion explosions[73];
	};
	std::vector<ExplodingMissile> explodingMissiles;

public:
	ExplosionRenderer();
	~ExplosionRenderer();

	void execute(DrawStackArgList);

	void reloadState();
};

#endif // EXPLOSIONRENDERER_H
