/*
 *  RenderingMaster.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef RENDERINGMASTER_H
#define RENDERINGMASTER_H

// program headers
#include "Screen.h"
#include "TerrainRenderer.h"
#include "WorldViewCamera.h"

class RenderingMaster {
private:
	WorldViewCamera* camera;

	// renderers
	TerrainRenderer terrainRenderer;

public:
	RenderingMaster(Screen* screen);

	void loop();
};

#endif
