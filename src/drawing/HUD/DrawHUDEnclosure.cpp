/*
 *  DrawHUDEnclosure.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/8/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
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
