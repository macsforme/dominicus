/*
 * UILayoutAuthority.h
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

#ifndef UILAYOUTAUTHORITY_H
#define UILAYOUTAUTHORITY_H

// program headers
#include "math/VectorMath.h"

// library headers
#include <vector>

// class definitions
class UIMetrics {
public:
	enum Bearing {
		BEARING_NONE,
		BEARING_LEFT,
		BEARING_RIGHT,
		BEARING_BOTTOM,
		BEARING_TOP
	};

	Bearing bearing1, bearing2;
	Vector2 position, size;

	UIMetrics(
			Bearing bearing1 = BEARING_NONE,
			Bearing bearing2 = BEARING_NONE,
			Vector2 position = Vector2(0.0f, 0.0f),
			Vector2 size = Vector2(0.0f, 0.0f)
		) :	 bearing1(bearing1), bearing2(bearing2), position(position), size(size) { }
};

class UILayoutAuthority {
private:
	UIMetrics* findPriorElementCenter(int index);

public:
	Vector2 elementMargin;

	Vector2 position;
	Vector2 size;

	std::vector<UIMetrics*> metrics;

	UILayoutAuthority(
			Vector2 elementMargin,
			Vector2 position = Vector2(0.0f, 0.0f),
			Vector2 size = Vector2(2.0f, 2.0f)
		) :	elementMargin(elementMargin), position(position), size(size) { };

	void rearrange();
};

#endif // UILAYOUTAUTHORITY_H
