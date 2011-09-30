// World.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef WORLD_H
#define WORLD_H

// program headers
#include "core/GameSystem.h"
#include "geometry/DiamondSquare.h"
#include "geometry/Mesh.h"
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "state/Collisions.h"

// library headers
#include <vector>
#include <sstream>

// global variables
extern GameSystem* gameSystem;

class World {
public:
	Mesh mesh;
	float longestEdge;

	CollisionTree* collisionTree;

	float diameter;
	float height;
	float lowestY;
	size_t density;

	World();
	~World();
};

#endif // WORLD_H
