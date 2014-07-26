// ShellRenderer.h
// Crucible Island

#ifndef SHELLRENDERER_H
#define SHELLRENDERER_H

#include <map>
#include <string>

#include "geometry/Mesh.h"
#include "graphics/DrawTypes.h"

class ShellRenderer : public BaseDrawNode {
private:
	// internal model data
	Mesh sphere;

public:
	ShellRenderer();
	~ShellRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif // SHELLRENDERER_H
