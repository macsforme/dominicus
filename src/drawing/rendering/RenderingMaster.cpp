/*
 *  RenderingMaster.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "RenderingMaster.h"

RenderingMaster::RenderingMaster(GameWindow* gameWindow) {
	wvCamera = new WorldViewCamera(gameWindow);
	oCamera = new OrthoCamera(gameWindow);

	cameraToggleKeyTrap = new KeyTrap("toggleCamera");

	activeMatrix = &(wvCamera->vpMatrix);
}

void RenderingMaster::loop() {
	wvCamera->loop();
	oCamera->loop();

	cameraToggleKeyTrap->loop();

	// toggle cameras if necessary
	if(cameraToggleKeyTrap->newPress()) {
		if(activeMatrix == &(oCamera->vpMatrix))
			activeMatrix = &(wvCamera->vpMatrix);
		else if(activeMatrix == &(wvCamera->vpMatrix))
			activeMatrix = &(oCamera->vpMatrix);
	}

	terrainRenderer.render(*activeMatrix);
}
