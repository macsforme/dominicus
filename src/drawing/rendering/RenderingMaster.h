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
#include "BackCamera.h"
#include "FirstPersonCamera.h"
#include "GameWindow.h"
#include "Keyboard.h"
#include "KeyTrap.h"
#include "ShipRenderer.h"
#include "SkyRenderer.h"
#include "TerrainRenderer.h"
#include "WorldViewCamera.h"

// global variables
extern Keyboard keyboard;

extern GameWindow* gameWindow;

class RenderingMaster {
private:
	BackCamera* bCamera;
	FirstPersonCamera* fpCamera;
	WorldViewCamera* wvCamera;

	Matrix4* activeMatrix;

	KeyTrap* cameraToggleKeyTrap;

	// renderers
	ShipRenderer shipRenderer;
	SkyRenderer skyRenderer;

public:
	TerrainRenderer terrainRenderer;

	RenderingMaster();

	void loop();
};

#endif
