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
#include "drawing/rendering/cameras/BackCamera.h"
#include "drawing/rendering/cameras/FirstPersonCamera.h"
#include "drawing/rendering/cameras/WorldViewCamera.h"
#include "drawing/rendering/ship/ShipRenderer.h"
#include "drawing/rendering/sky/SkyRenderer.h"
#include "drawing/rendering/terrain/TerrainRenderer.h"
#include "input/Keyboard.h"
#include "input/KeyTrap.h"
#include "windowing/GameWindow.h"

// global variables
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
