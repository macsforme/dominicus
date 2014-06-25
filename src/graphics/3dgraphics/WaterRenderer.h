// WaterRenderer.h
// Crucible Island

#ifndef WATERRENDERER_H
#define WATERRENDERER_H

#include <map>
#include <string>

#include "graphics/DrawTypes.h"

class WaterRenderer : public BaseDrawNode {
public:
	WaterRenderer();
	~WaterRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif // WATERRENDERER_H
