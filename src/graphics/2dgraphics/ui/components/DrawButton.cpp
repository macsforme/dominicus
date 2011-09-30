// DrawButton.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/2dgraphics/ui/components/DrawButton.h"

DrawButton::DrawButton() :
		containerDrawer(new DrawContainer()), labelDrawer(new DrawLabel()) { }

DrawButton::~DrawButton() {
	// memory management
	delete(containerDrawer);
	delete(labelDrawer);
}

Vector2 DrawButton::getSize(std::map<std::string, void*> arguments) {
	// call children's functions to calculate button size
	Vector2 padding = Vector2(
		*((float*) arguments["padding"]) / (float) gameGraphics->resolutionX * 2.0f,
		*((float*) arguments["padding"]) / (float) gameGraphics->resolutionY * 2.0f
	);

	Vector2 newSize = labelDrawer->getSize(arguments);

	newSize.x += padding.x * 2.0f;
	newSize.y += padding.y * 2.0f;

	return newSize;
}

void DrawButton::execute(std::map<std::string, void*> arguments) {
	// call children's functions to draw stuff
	containerDrawer->execute(arguments);

	// DrawLabel already does internal checks on the size of the area
	labelDrawer->execute(arguments);
}
