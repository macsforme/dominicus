// DiamondSquare.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.
//
// RIP Sergeant Sean Drenth #6894, Phoenix Police, EOW 10-18-2010.

#ifndef DIAMONDSQUARE_H
#define DIAMONDSQUARE_H

// program headers
#include "math/ScalarMath.h"

// library headers
#include <cstdlib>

class DiamondSquare {
public:
	unsigned int size;
	float** data;

	DiamondSquare(unsigned int size, float roughness);
};

#endif
