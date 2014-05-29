// ShellRenderer.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef SHELLRENDERER_H
#define SHELLRENDERER_H

// program headers
#include "core/GameSystem.h"
#include "geometry/Mesh.h"
#include "geometry/Sphere.h"
#include "graphics/GameGraphics.h"
#include "logic/Camera.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"

// library headers
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

// global variables
extern GameGraphics* gameGraphics;
extern GameSystem* gameSystem;
extern Platform* platform;

// class definition
class ShellRenderer : public BaseDrawNode {
private:
	// internal model data
	Mesh sphere;

public:
	ShellRenderer();
	~ShellRenderer();

	void execute(std::map<std::string, void*> arguments);
};

#endif
