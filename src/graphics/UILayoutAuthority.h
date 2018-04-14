// UILayoutAuthority.h
// Dominicus

#ifndef UILAYOUTAUTHORITY_H
#define UILAYOUTAUTHORITY_H

#include <vector>

#include "math/VectorMath.h"

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
