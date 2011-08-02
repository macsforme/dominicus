/*
 * DrawContainer.h
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

#ifndef DRAWCONTAINER_H
#define DRAWCONTAINER_H

// program headers
#include "graphics/BaseDrawNode.h"
#include "graphics/GameGraphics.h"
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"

// library headers
#include <map>
#include <string.h>
#include <vector>

// global variables
extern GameGraphics* gameGraphics;
extern Platform* platform;

class DrawContainer : public BaseDrawNode {
protected:
	struct VertexEntry {
		Vector2 position;
		Vector2 primCoord;
		Vector2 curveOriginCoord;
		bool highlight;
		bool concave;
	};

	// utility drawing functions
	void drawCurve(std::vector<VertexEntry>* quadVertices, Vector2 position, Vector2 size, float rotation,
			bool highlight = false, bool concave = false);
	void drawBorder(std::vector<VertexEntry>* quadVertices, Vector2 position, Vector2 size, float rotation,
			bool highlight = false);
	void drawFiller(std::vector<VertexEntry>* quadVertices, Vector2 position, Vector2 size,
			bool highlight = false);

public:
	DrawContainer();

	void execute(std::map<std::string, void*> newDrawData);
};

#endif // DRAWCONTAINER_H
