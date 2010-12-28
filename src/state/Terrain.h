/*
 *  Terrain.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef TERRAIN_H
#define TERRAIN_H

// definitions
#define TERRAIN_MAXWIDTH 100.0f
#define TERRAIN_MAXHEIGHT 10.0f

// program headers
#include "DiamondSquare.h"
#include "MatrixMath.h"
#include "Mesh.h"
#include "Platform.h"
#include "ScalarMath.h"
#include "VectorMath.h"

// library headers
#include <vector>

// global variables
extern Platform platform;

class Terrain {
public:
	Mesh mesh;

	Terrain();
};

#endif
