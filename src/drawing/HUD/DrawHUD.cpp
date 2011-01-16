/*
 *  DrawHUD.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/7/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "DrawHUD.h"

DrawHUD::DrawHUD(FontManager* fontManager) {
	// set variables
	hudArrangement = new HUDArrangement();
	drawHUDEnclosure = new DrawHUDEnclosure(hudArrangement);

	// set up children drawers
	consoleDrawer = new DrawConsole(hudArrangement, fontManager);
	infoBoxDrawer = new DrawInfoBox(hudArrangement, fontManager);
}

DrawHUD::~DrawHUD() {
	// delete variables
	delete(drawHUDEnclosure);

	// delete children
	delete(consoleDrawer);
}

void DrawHUD::draw() {
	// draw the HUD enclosure
	drawHUDEnclosure->draw();

	// draw the children
	consoleDrawer->draw();
	infoBoxDrawer->draw();
}
