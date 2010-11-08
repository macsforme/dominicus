/*
 *  DrawHUD.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/7/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "DrawHUD.h"

DrawHUD::DrawHUD(Screen* screen) : screen(screen) {
	// set our variables
	float aspectRatio = (float) screen->width / (float) screen->height;
	elementPadding = Vector2(
			gamePrefs.getFloat("elementPadding") / aspectRatio,
			gamePrefs.getFloat("elementPadding")
		);

	// add our HUD elements to the arranger
	HUDArrangement::HUDElementEntry entry;

	consoleHUDElement = new HUDElement(Vector2(1.0f, 0.333f));
	entry.element = consoleHUDElement;
	entry.bearing1 = HUDArrangement::BEARING_BOTTOM;
	entry.bearing2 = HUDArrangement::BEARING_LEFT;
	hudArrangement.elementList.push_back(entry);

	rearViewMirrorHUDElement = new HUDElement(Vector2(0.8f, 0.25f));
	entry.element = rearViewMirrorHUDElement;
	entry.bearing1 = HUDArrangement::BEARING_TOP;
	entry.bearing2 = HUDArrangement::BEARING_NONE;
	hudArrangement.elementList.push_back(entry);

	playerInfoHUDElement = new HUDElement(Vector2(0.333f, 0.4f));
	entry.element = playerInfoHUDElement;
	entry.bearing1 = HUDArrangement::BEARING_TOP;
	entry.bearing2 = HUDArrangement::BEARING_RIGHT;
	hudArrangement.elementList.push_back(entry);

	radarElement = new HUDElement(Vector2(0.5f, 0.5f * aspectRatio));
	entry.element = radarElement;
	entry.bearing1 = HUDArrangement::BEARING_BOTTOM;
	entry.bearing2 = HUDArrangement::BEARING_RIGHT;
	hudArrangement.elementList.push_back(entry);
}

HUDElement* DrawHUD::findPriorElementCenter(int index) {
	HUDArrangement::HUDElementEntry* thisElement = &(hudArrangement.elementList[index]);

	// search for higher-priority elements with our same bearing (reverse order)
	for(int i = index - 1; i >= 0; --i) {
		if(
				hudArrangement.elementList[i].bearing1 == thisElement->bearing1 &&
				hudArrangement.elementList[i].bearing2 == thisElement->bearing2
			)
		return hudArrangement.elementList[i].element;
	}

	// if none, the inverse bearing (standard order)
	for(int i = 0; i < index; ++i) {
		if(
				hudArrangement.elementList[i].bearing1 == thisElement->bearing2 &&
				hudArrangement.elementList[i].bearing2 == thisElement->bearing1
			)
		return hudArrangement.elementList[i].element;
	}

	return NULL;
}

void DrawHUD::draw() {
	// draw each element box, keeping track of where we're at
	for(int i = 0; i < hudArrangement.elementList.size(); ++i) {
		HUDElement* thisElement = hudArrangement.elementList[i].element;
		HUDElement* neighbor = findPriorElementCenter(i);

		if(hudArrangement.elementList[i].bearing1 == HUDArrangement::BEARING_LEFT) {
			if(hudArrangement.elementList[i].bearing2 == HUDArrangement::BEARING_BOTTOM) {
				thisElement->position = Vector2(
						-1.0f + elementPadding.x + thisElement->size.x / 2.0f,
						(neighbor != NULL ? neighbor->position.y + neighbor->size.y / 2.0f : -1.0f) +
								elementPadding.y + thisElement->size.y / 2.0f
					);
			} else if(hudArrangement.elementList[i].bearing2 == HUDArrangement::BEARING_TOP) {
				thisElement->position = Vector2(
						-1.0f + elementPadding.x + thisElement->size.x / 2.0f,
						(neighbor != NULL ? neighbor->position.y - neighbor->size.y / 2.0f : 1.0f) -
								elementPadding.y - thisElement->size.y / 2.0f
					);
			} else {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x + neighbor->size.x / 2.0f : -1.0f) +
								elementPadding.x + thisElement->size.x / 2.0f,
								0.0f
					);
			}
		}

		if(hudArrangement.elementList[i].bearing1 == HUDArrangement::BEARING_RIGHT) {
			if(hudArrangement.elementList[i].bearing2 == HUDArrangement::BEARING_BOTTOM) {
				thisElement->position = Vector2(
						1.0f - elementPadding.x - thisElement->size.x / 2.0f,
						(neighbor != NULL ? neighbor->position.y + neighbor->size.y / 2.0f : -1.0f) +
								elementPadding.y + thisElement->size.y / 2.0f
					);
			} else if(hudArrangement.elementList[i].bearing2 == HUDArrangement::BEARING_TOP) {
				thisElement->position = Vector2(
						1.0f - elementPadding.x - thisElement->size.x / 2.0f,
						(neighbor != NULL ? neighbor->position.y - neighbor->size.y / 2.0f : 1.0f) -
								elementPadding.y - thisElement->size.y / 2.0f
					);
			} else {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x - neighbor->size.x / 2.0f : 1.0f) -
								elementPadding.x - thisElement->size.x / 2.0f,
								0.0f
					);
			}
		}

		if(hudArrangement.elementList[i].bearing1 == HUDArrangement::BEARING_BOTTOM) {
			if(hudArrangement.elementList[i].bearing2 == HUDArrangement::BEARING_LEFT) {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x + neighbor->size.x / 2.0f : -1.0f) +
								elementPadding.x + thisElement->size.x / 2.0f,
								-1.0f + elementPadding.y + thisElement->size.y / 2.0f
					);
			} else if(hudArrangement.elementList[i].bearing2 == HUDArrangement::BEARING_RIGHT) {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x - neighbor->size.x / 2.0f : 1.0f) -
								elementPadding.x - thisElement->size.x / 2.0f,
								-1.0f + elementPadding.y + thisElement->size.y / 2.0f
					);
			} else {
				thisElement->position = Vector2(
						0.0f,
						(neighbor != NULL ? neighbor->position.y + neighbor->size.y / 2.0f : -1.0f) +
								elementPadding.y + thisElement->size.y / 2.0f
					);
			}
		}

		if(hudArrangement.elementList[i].bearing1 == HUDArrangement::BEARING_TOP) {
			if(hudArrangement.elementList[i].bearing2 == HUDArrangement::BEARING_LEFT) {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x + neighbor->size.x / 2.0f : -1.0f) +
								elementPadding.x + thisElement->size.x / 2.0f,
								1.0f - elementPadding.y - thisElement->size.y / 2.0f
					);
			} else if(hudArrangement.elementList[i].bearing2 == HUDArrangement::BEARING_RIGHT) {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x - neighbor->size.x / 2.0f : 1.0f) -
								elementPadding.x - thisElement->size.x / 2.0f,
								1.0f - elementPadding.y - thisElement->size.y / 2.0f
					);
			} else {
				thisElement->position = Vector2(
						0.0f,
						(neighbor != NULL ? neighbor->position.y - neighbor->size.y / 2.0f : 1.0f) -
								elementPadding.y - thisElement->size.y / 2.0f
					);
			}
		}
	}
}
