// BaseUIElement.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef BASEUIELEMENT_H
#define BASEUIELEMENT_H

// program headers
#include "graphics/BaseDrawNode.h"
#include "math/VectorMath.h"

// library headers
#include <map>
#include <string>

class BaseUIElement : public BaseDrawNode {
public:
	virtual Vector2 getSize(std::map<std::string, void*> arguments) = 0;
};

#endif // BASEUIELEMENT_H
