// SkyRenderer.h
// Crucible Island

#ifndef SKYRENDERER_H
#define SKYRENDERER_H

#include <map>
#include <string>

#include "graphics/DrawTypes.h"

class SkyRenderer : public BaseDrawNode {
public:
	SkyRenderer();
	~SkyRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif // SKYRENDERER_H
