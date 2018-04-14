// WaterRenderer.h
// Dominicus

#ifndef WATERRENDERER_H
#define WATERRENDERER_H

#include "graphics/DrawTypes.h"

class WaterRenderer : public BaseDrawNode {
public:
	WaterRenderer();
	~WaterRenderer();

	void execute(DrawStackArgList arguments);
};

#endif // WATERRENDERER_H
