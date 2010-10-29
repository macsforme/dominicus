/*
 *  DiamondSquare.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/18/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 *  RIP Sergeant Sean Drenth #6894, Phoenix Police, EOW 10-18-2010.
 *
 */

#ifndef DIAMONDSQUARE_H
#define DIAMONDSQUARE_H

// program headers
#include "ScalarMath.h"

// library headers
#include <cstdlib>

class DiamondSquare {
public:
	unsigned int size;
	float** data;

	DiamondSquare(unsigned int size, float roughness);
};

#endif
