/*
 *  DrawHUD.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/7/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "DrawHUD.h"

DrawHUD::DrawHUD(Screen* screen, FontManager* fontManager) {
	// set variables
	hudArrangement = new HUDArrangement(screen);
	drawHUDEnclosure = new DrawHUDEnclosure(hudArrangement, screen);

	// set up children drawers
	consoleDrawer = new DrawConsole(hudArrangement, screen, fontManager);
	infoBoxDrawer = new DrawInfoBox(hudArrangement, screen, fontManager);
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
