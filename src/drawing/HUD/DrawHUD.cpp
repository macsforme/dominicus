/*
 *  DrawHUD.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/7/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "DrawHUD.h"

DrawHUD::DrawHUD(GameWindow* gameWindow, FontManager* fontManager) {
	// set variables
	hudArrangement = new HUDArrangement(gameWindow);
	drawHUDEnclosure = new DrawHUDEnclosure(hudArrangement, gameWindow);

	// set up children drawers
	consoleDrawer = new DrawConsole(hudArrangement, gameWindow, fontManager);
	infoBoxDrawer = new DrawInfoBox(hudArrangement, gameWindow, fontManager);
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
