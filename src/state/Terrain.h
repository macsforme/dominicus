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
#define TERRAIN_MAXWIDTH 1000.0f
#define TERRAIN_MAXHEIGHT 100.0f

// program headers
#include "geometry/DiamondSquare.h"
#include "geometry/Mesh.h"
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"

// library headers
#include <vector>

class Terrain {
public:
	Mesh mesh;

	Terrain();
};

#endif
