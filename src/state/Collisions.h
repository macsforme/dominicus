/*
 * Collisions.h
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
