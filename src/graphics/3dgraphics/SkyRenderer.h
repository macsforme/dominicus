// SkyRenderer.h
// Crucible Island

#ifndef SKYRENDERER_H
#define SKYRENDERER_H

#include "graphics/DrawTypes.h"

class SkyRenderer : public BaseDrawNode {
public:
	SkyRenderer();
	~SkyRenderer();

	void execute(DrawStackArgList arguments);
};

#endif // SKYRENDERER_H
