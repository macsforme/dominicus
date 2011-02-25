/*
 * HUDArrangement.cpp
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

#include "drawing/HUD/HUDArrangement.h"

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
			gamePrefs->getFloat("elementPadding") / aspectRatio,
			gamePrefs->getFloat("elementPadding")
		);

}

void HUDArrangement::rearrange() {
	// set each element position
	for(size_t i = 0; i < elementList.size(); ++i) {
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
