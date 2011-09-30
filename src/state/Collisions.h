// Collisions.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef COLLISIONS_H
#define COLLISIONS_H

// forward declarations
class CollisionEntity;

// program headers
#include "core/GameSystem.h"
#include "geometry/Mesh.h"
#include "math/MatrixMath.h"
#include "math/MiscMath.h"
#include "math/VectorMath.h"

// library headers
#include <map>
#include <vector>
#include <sstream>

// global variables
extern GameSystem* gameSystem;

// collision utility functions
bool testTriangleSphereCollision(Vector3 triangleIn[3], Vector3 position, float radius);

// class definitions
class CollisionTree {
public:
	Mesh* mesh;

	std::map< size_t, std::vector<size_t> > faceMap;
	std::vector<size_t> vertices;

	CollisionEntity* entities;

	CollisionTree(Mesh* mesh);
	~CollisionTree();

private:
	std::vector<size_t> traverseTree(Mesh* mesh, CollisionEntity* entity, Vector2 position, float dist);
public:
	std::vector<size_t> getFaces(Vector2 position, float dist);
};

class CollisionEntity {
public:
	Vector2 ll, ur;

	size_t myVertex;

	std::vector<CollisionEntity*> subEntities;

	CollisionEntity(
			Mesh* mesh,
			std::vector<size_t>& vertices,
			std::map< size_t, std::vector<size_t> >& faceMap,
			unsigned int depth,
			unsigned int& maxDepth,
			unsigned int& totalNodes,
			Vector2 ll,
			Vector2 ur
		);
	~CollisionEntity();
};


#endif
