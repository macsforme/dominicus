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
#include "Keyboard.h"
#include "KeyTrap.h"
#include "OrthoCamera.h"
#include "Screen.h"
#include "TerrainRenderer.h"
#include "WorldViewCamera.h"

// global variables
extern Keyboard keyboard;

class RenderingMaster {
private:
	WorldViewCamera* wvCamera;
	OrthoCamera *oCamera;

	Matrix4* activeMatrix;

	KeyTrap* cameraToggleKeyTrap;

	// renderers
	TerrainRenderer terrainRenderer;

public:
	RenderingMaster(Screen* screen);

	void loop();
};

#endif
