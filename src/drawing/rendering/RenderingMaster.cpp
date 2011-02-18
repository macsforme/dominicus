/*
 *  RenderingMaster.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "drawing/rendering/RenderingMaster.h"

RenderingMaster::RenderingMaster() {
	bCamera = new BackCamera();
	fpCamera = new FirstPersonCamera();
	wvCamera = new WorldViewCamera();

	cameraToggleKeyTrap = new KeyTrap("toggleCamera");

	activeMatrix = &(bCamera->terrainVPMatrix);
}

void RenderingMaster::loop() {
	bCamera->loop();
	fpCamera->loop();
	wvCamera->loop();

	cameraToggleKeyTrap->loop();

	// toggle cameras if necessary
	if(cameraToggleKeyTrap->newPress()) {
		if(activeMatrix == &(bCamera->terrainVPMatrix))
			activeMatrix = &(fpCamera->vpMatrix);
		else if(activeMatrix == &(fpCamera->vpMatrix))
			activeMatrix = &(wvCamera->vpMatrix);
		else if(activeMatrix == &(wvCamera->vpMatrix))
			activeMatrix = &(bCamera->terrainVPMatrix);
	}

//	skyRenderer.render(*activeMatrix);
	terrainRenderer.render(*activeMatrix);
	if(activeMatrix == &(bCamera->terrainVPMatrix))
		shipRenderer.render(bCamera->shipVPMatrix);
}
