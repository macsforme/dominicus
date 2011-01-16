/*
 *  HUDArrangement.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/22/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "HUDArrangement.h"

HUDElement* HUDArrangement::findPriorElementCenter(int index) {
	HUDElementEntry* thisElement = &(elementList[index]);

	// search for higher-priority elements with our same bearing (reverse order)
	for(int i = index - 1; i >= 0; --i) {
		if(
				elementList[i].bearing1 == thisElement->bearing1 &&
				elementList[i].bearing2 == thisElement->bearing2
			)
		return elementList[i].element;
	}

	// if none, the inverse bearing (standard order)
	for(int i = 0; i < index; ++i) {
		if(
				elementList[i].bearing1 == thisElement->bearing2 &&
				elementList[i].bearing2 == thisElement->bearing1
			)
		return elementList[i].element;
	}

	return NULL;
}

HUDArrangement::HUDArrangement() {
	// set our variables
	float aspectRatio = (float) gameWindow->width / (float) gameWindow->height;
	elementPadding = Vector2(
			gamePrefs.getFloat("elementPadding") / aspectRatio,
			gamePrefs.getFloat("elementPadding")
		);

}

void HUDArrangement::rearrange() {
	// set each element position
	for(int i = 0; i < elementList.size(); ++i) {
		HUDElement* thisElement = elementList[i].element;
		HUDElement* neighbor = findPriorElementCenter(i);

		if(elementList[i].bearing1 == HUDArrangement::BEARING_LEFT) {
			if(elementList[i].bearing2 == HUDArrangement::BEARING_BOTTOM) {
				thisElement->position = Vector2(
						-1.0f + elementPadding.x + thisElement->size.x / 2.0f,
						(neighbor != NULL ? neighbor->position.y + neighbor->size.y / 2.0f : -1.0f) +
								elementPadding.y + thisElement->size.y / 2.0f
					);
			} else if(elementList[i].bearing2 == HUDArrangement::BEARING_TOP) {
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

		if(elementList[i].bearing1 == HUDArrangement::BEARING_RIGHT) {
			if(elementList[i].bearing2 == HUDArrangement::BEARING_BOTTOM) {
				thisElement->position = Vector2(
						1.0f - elementPadding.x - thisElement->size.x / 2.0f,
						(neighbor != NULL ? neighbor->position.y + neighbor->size.y / 2.0f : -1.0f) +
								elementPadding.y + thisElement->size.y / 2.0f
					);
			} else if(elementList[i].bearing2 == HUDArrangement::BEARING_TOP) {
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

		if(elementList[i].bearing1 == HUDArrangement::BEARING_BOTTOM) {
			if(elementList[i].bearing2 == HUDArrangement::BEARING_LEFT) {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x + neighbor->size.x / 2.0f : -1.0f) +
								elementPadding.x + thisElement->size.x / 2.0f,
								-1.0f + elementPadding.y + thisElement->size.y / 2.0f
					);
			} else if(elementList[i].bearing2 == HUDArrangement::BEARING_RIGHT) {
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

		if(elementList[i].bearing1 == HUDArrangement::BEARING_TOP) {
			if(elementList[i].bearing2 == HUDArrangement::BEARING_LEFT) {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x + neighbor->size.x / 2.0f : -1.0f) +
								elementPadding.x + thisElement->size.x / 2.0f,
								1.0f - elementPadding.y - thisElement->size.y / 2.0f
					);
			} else if(elementList[i].bearing2 == HUDArrangement::BEARING_RIGHT) {
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
