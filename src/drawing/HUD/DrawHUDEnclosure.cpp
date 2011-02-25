/*
 * DrawHUDEnclosure.cpp
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

#include "drawing/HUD/DrawHUDEnclosure.h"

HUDElement* DrawHUDEnclosure::findPriorElementCenter(int index) {
	HUDArrangement::HUDElementEntry* thisElement = &(hudArrangement->elementList[index]);

	// search for higher-priority elements with our same bearing (reverse order)
	for(int i = index - 1; i >= 0; --i) {
		if(
				hudArrangement->elementList[i].bearing1 == thisElement->bearing1 &&
				hudArrangement->elementList[i].bearing2 == thisElement->bearing2
			)
		return hudArrangement->elementList[i].element;
	}

	// if none, the inverse bearing (standard order)
	for(int i = 0; i < index; ++i) {
		if(
				hudArrangement->elementList[i].bearing1 == thisElement->bearing2 &&
				hudArrangement->elementList[i].bearing2 == thisElement->bearing1
			)
		return hudArrangement->elementList[i].element;
	}

	return NULL;
}

DrawHUDEnclosure::DrawHUDEnclosure(HUDArrangement* hudArrangement) : hudArrangement(hudArrangement) {
	float insideColor[4] = { 0.0f, 0.01f, 0.13f, 0.24f };
	float highlightColor[4] = { 0.0f, 0.01f, 0.13f, 0.24f };
	float borderColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float outsideColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	containerUtility = new DrawHUDContainerUtility(
			-0.95f,
			0.0f,
			insideColor,
			highlightColor,
			borderColor,
			outsideColor
		);
}

void DrawHUDEnclosure::draw() {
	float aspectRatio = (float) gameWindow->aspectRatio;
	Vector2 elementPadding(
			gamePrefs->getFloat("elementPadding") / aspectRatio,
			gamePrefs->getFloat("elementPadding")
		);
	elementPadding /= 2.0f;

	// draw the padding around each element
	for(size_t i = 0; i < hudArrangement->elementList.size(); ++i) {
		HUDArrangement::HUDElementEntry* thisElementEntry = &(hudArrangement->elementList[i]);
		HUDElement* thisElement = thisElementEntry->element;

		Vector2 paddingSize = elementPadding;

		// background
		containerUtility->drawFiller(thisElement->position, thisElement->size, false, 0.0f);

		// sides
		containerUtility->drawBorder(
				Vector2(thisElement->position.x - thisElement->size.x / 2.0f -
						paddingSize.x / 2.0f, thisElement->position.y),
				Vector2(paddingSize.x, thisElement->size.y),
				180.0f,
				false,
				0.0f);
		containerUtility->drawBorder(
				Vector2(thisElement->position.x + thisElement->size.x / 2.0f +
						paddingSize.x / 2.0f, thisElement->position.y),
				Vector2(paddingSize.x, thisElement->size.y),
				0.0f,
				false,
				0.0f);

		containerUtility->drawBorder(
				Vector2(thisElement->position.x, thisElement->position.y -
						thisElement->size.y / 2.0f - paddingSize.y / 2.0f),
				Vector2(thisElement->size.x, paddingSize.y),
				270.0f,
				false,
				0.005f);
		containerUtility->drawBorder(
				Vector2(thisElement->position.x, thisElement->position.y +
						thisElement->size.y / 2.0f + paddingSize.y / 2.0f),
				Vector2(thisElement->size.x, paddingSize.y),
				90.0f,
				false,
				0.005f);

		// corners
		containerUtility->drawCurve(
				thisElement->position + thisElement->size / 2.0f + paddingSize / 2.0f,
				paddingSize,
				0.0f,
				false,
				0.0f,
				false
			);
		containerUtility->drawCurve(
				Vector2(
						thisElement->position.x - thisElement->size.x / 2.0f - paddingSize.x / 2.0f,
						thisElement->position.y + thisElement->size.y / 2.0f + paddingSize.y / 2.0f
					),
				paddingSize,
				90.0f,
				false,
				0.0f,
				false
			);
		containerUtility->drawCurve(
				thisElement->position - thisElement->size / 2.0 - paddingSize / 2.0f,
				paddingSize,
				180.0f,
				false,
				0.0f,
				false
			);
		containerUtility->drawCurve(
				Vector2(
						thisElement->position.x + thisElement->size.x / 2.0f + paddingSize.x / 2.0f,
						thisElement->position.y - thisElement->size.y / 2.0f - paddingSize.y / 2.0f
					),
				paddingSize,
				270.0f,
				false,
				0.0f,
				false
			);
	}
}
