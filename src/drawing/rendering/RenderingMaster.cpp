/*
 *  RenderingMaster.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "RenderingMaster.h"

RenderingMaster::RenderingMaster(Screen* screen) {
	camera = new WorldViewCamera(screen);
}

void RenderingMaster::loop() {
	camera->loop();

	terrainRenderer.render(camera->vpMatrix);
}