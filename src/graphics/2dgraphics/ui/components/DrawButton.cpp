/*
 * DrawButton.cpp
 * Dominicus
 *
 * Copyright 2010-2011, Joshua Bodine
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

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
