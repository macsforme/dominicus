/*
 * UILogic.cpp
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

#include "logic/UILayoutAuthority.h"

UIMetrics* UILayoutAuthority::findPriorElementCenter(int index) {
	// search for higher-priority elements with our same bearing (reverse order)
	for(int i = index - 1; i >= 0; --i) {
		if(
				metrics[i]->bearing1 == metrics[index]->bearing1 &&
				metrics[i]->bearing2 == metrics[index]->bearing2
			)
		return metrics[i];
	}

	// if none, the inverse bearing (standard order)
	for(int i = 0; i < index; ++i) {
		if(
				metrics[i]->bearing1 == metrics[index]->bearing2 &&
				metrics[i]->bearing2 == metrics[index]->bearing1
			)
		return metrics[i];
	}

	return NULL;
}

void UILayoutAuthority::rearrange() {
	// set each element position
	for(size_t i = 0; i < metrics.size(); ++i) {
		UIMetrics* thisElement = metrics[i];
		UIMetrics* neighbor = findPriorElementCenter(i);

		if(metrics[i]->bearing1 == UIMetrics::BEARING_LEFT) {
			if(metrics[i]->bearing2 == UIMetrics::BEARING_BOTTOM) {
				thisElement->position = Vector2(
						-1.0f + elementMargin.x + thisElement->size.x / 2.0f,
						(neighbor != NULL ? neighbor->position.y + neighbor->size.y / 2.0f : -1.0f) +
								elementMargin.y + thisElement->size.y / 2.0f
					);
			} else if(metrics[i]->bearing2 == UIMetrics::BEARING_TOP) {
				thisElement->position = Vector2(
						-1.0f + elementMargin.x + thisElement->size.x / 2.0f,
						(neighbor != NULL ? neighbor->position.y - neighbor->size.y / 2.0f : 1.0f) -
								elementMargin.y - thisElement->size.y / 2.0f
					);
			} else {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x + neighbor->size.x / 2.0f : -1.0f) +
								elementMargin.x + thisElement->size.x / 2.0f,
								0.0f
					);
			}
		}

		if(metrics[i]->bearing1 == UIMetrics::BEARING_RIGHT) {
			if(metrics[i]->bearing2 == UIMetrics::BEARING_BOTTOM) {
				thisElement->position = Vector2(
						1.0f - elementMargin.x - thisElement->size.x / 2.0f,
						(neighbor != NULL ? neighbor->position.y + neighbor->size.y / 2.0f : -1.0f) +
								elementMargin.y + thisElement->size.y / 2.0f
					);
			} else if(metrics[i]->bearing2 == UIMetrics::BEARING_TOP) {
				thisElement->position = Vector2(
						1.0f - elementMargin.x - thisElement->size.x / 2.0f,
						(neighbor != NULL ? neighbor->position.y - neighbor->size.y / 2.0f : 1.0f) -
								elementMargin.y - thisElement->size.y / 2.0f
					);
			} else {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x - neighbor->size.x / 2.0f : 1.0f) -
								elementMargin.x - thisElement->size.x / 2.0f,
								0.0f
					);
			}
		}

		if(metrics[i]->bearing1 == UIMetrics::BEARING_BOTTOM) {
			if(metrics[i]->bearing2 == UIMetrics::BEARING_LEFT) {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x + neighbor->size.x / 2.0f : -1.0f) +
								elementMargin.x + thisElement->size.x / 2.0f,
								-1.0f + elementMargin.y + thisElement->size.y / 2.0f
					);
			} else if(metrics[i]->bearing2 == UIMetrics::BEARING_RIGHT) {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x - neighbor->size.x / 2.0f : 1.0f) -
								elementMargin.x - thisElement->size.x / 2.0f,
								-1.0f + elementMargin.y + thisElement->size.y / 2.0f
					);
			} else {
				thisElement->position = Vector2(
						0.0f,
						(neighbor != NULL ? neighbor->position.y + neighbor->size.y / 2.0f : -1.0f) +
								elementMargin.y + thisElement->size.y / 2.0f
					);
			}
		}

		if(metrics[i]->bearing1 == UIMetrics::BEARING_TOP) {
			if(metrics[i]->bearing2 == UIMetrics::BEARING_LEFT) {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x + neighbor->size.x / 2.0f : -1.0f) +
								elementMargin.x + thisElement->size.x / 2.0f,
								1.0f - elementMargin.y - thisElement->size.y / 2.0f
					);
			} else if(metrics[i]->bearing2 == UIMetrics::BEARING_RIGHT) {
				thisElement->position = Vector2(
						(neighbor != NULL ? neighbor->position.x - neighbor->size.x / 2.0f : 1.0f) -
								elementMargin.x - thisElement->size.x / 2.0f,
								1.0f - elementMargin.y - thisElement->size.y / 2.0f
					);
			} else {
				thisElement->position = Vector2(
						0.0f,
						(neighbor != NULL ? neighbor->position.y - neighbor->size.y / 2.0f : 1.0f) -
								elementMargin.y - thisElement->size.y / 2.0f
					);
			}
		}
	}
}
