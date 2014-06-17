// UILayoutAuthority.cpp
// Crucible Island

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
