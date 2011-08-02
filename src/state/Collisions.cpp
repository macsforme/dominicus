/*
 * Collisions.cpp
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

#include "state/Collisions.h"

bool testTriangleSphereCollision(Vector3 triangleIn[3], Vector3 position, float radius) {
	// does passing an array really pass a reference/pointer??? somehow the values are being modified
	Vector3 triangle[] = { triangleIn[0], triangleIn[1], triangleIn[2] };

	// this function tests the triangle against a sphere for intersection
	triangle[0] -= position;
	triangle[1] -= position;
	triangle[2] -= position;

	Vector3 normal = getNormal(triangle[0], triangle[1], triangle[2]);

	Matrix3 transformation = completeMatrix(normal);
	transformation.transpose();

	rotateMatrix(Vector3(0.0f, 0.0f, 1.0f), radians(90.0f), transformation);

	triangle[0] = triangle[0] * transformation;
	triangle[1] = triangle[1] * transformation;
	triangle[2] = triangle[2] * transformation;

	if(triangle[0].y > 0.0f)
		// already through
		return false;

	if(triangle[0].y < -radius)
		// no collision
		return false;

	bool allZArePositive = true;

	for(size_t i = 0; i < 3; ++i) {
		// test each edge
		size_t index1 = i, index2 = (i == 2 ? 0 : i + 1);

		Vector3 diffVec = triangle[index2] - triangle[index1];
		diffVec.norm();

		Matrix3 edgeTransformation = completeMatrix(diffVec);
		edgeTransformation.transpose();

		Vector3 edgeTestVertices[2];
		edgeTestVertices[0] = triangle[index1] * edgeTransformation;
		edgeTestVertices[1] = triangle[index2] * edgeTransformation;

		if(edgeTestVertices[0].z <= 0.0f)
			allZArePositive = false;

		// test corners and top edge
		Vector3 originVector(0.0f, 0.0f, 0.0f);

		if(distance(originVector, edgeTestVertices[0]) <= radius)
			// hit on vertex #1
			return true;

		if(distance(originVector, edgeTestVertices[1]) <= radius)
			// hit on vertex #2
			return true;

		Vector2 angleVec1(-edgeTestVertices[0].z, 0.0f);
		Vector2 angleVec2(0.0f, -edgeTestVertices[0].y);
		if(
			edgeTestVertices[0].x <= 0.0f &&
			edgeTestVertices[1].x >= 0.0f &&
			distance(angleVec1, angleVec2) < radius
			)
			// edge hit
			return true;
	}

	if(allZArePositive)
		// hit somewhere in the middle
		return true;

	return false;
}

CollisionTree::CollisionTree(Mesh* mesh) : mesh(mesh) {
	// populate the face map
	for(
			size_t i = 0; i < mesh->faceGroups.begin()->second.size(); ++i
		) {
		faceMap[mesh->faceGroups.begin()->second[i].vertices[0]].push_back(i);
		faceMap[mesh->faceGroups.begin()->second[i].vertices[1]].push_back(i);
		faceMap[mesh->faceGroups.begin()->second[i].vertices[2]].push_back(i);
	}

	// populate the vertex list
	Vector2 ll(0.0f, 0.0f), ur(0.0f, 0.0f);

	for(std::map< size_t, std::vector<size_t> >::iterator itr = faceMap.begin(); itr != faceMap.end(); ++itr) {
		vertices.push_back(itr->first);

		// update the bounding box if necessary
		Vector2 xzRep(mesh->vertices[itr->first].x, mesh->vertices[itr->first].z);

		if(xzRep.x < ll.x) ll.x = xzRep.x;
		else if(xzRep.y < ll.y) ll.y = xzRep.y;
		else if(xzRep.x > ur.x) ur.x = xzRep.x;
		else if(xzRep.y > ur.y) ur.y = xzRep.y;
	}

	// populate the collision tree
	unsigned int maxDepth = 0, childNodes = 0;
	entities = new CollisionEntity(mesh, vertices, faceMap, 0, maxDepth, childNodes, ll, ur);

	// log specifications
	gameSystem->log(GameSystem::LOG_VERBOSE, "Collision tree built");

	std::stringstream logStream;

	logStream << "        Total leaf nodes: " << childNodes;
	gameSystem->log(GameSystem::LOG_VERBOSE, logStream.str().c_str());
	logStream.str("");

	logStream << "        Maximum tree depth: " << maxDepth;
	gameSystem->log(GameSystem::LOG_VERBOSE, logStream.str().c_str());
	logStream.str("");

	logStream << "        Total vertices: " << faceMap.size();
	gameSystem->log(GameSystem::LOG_VERBOSE, logStream.str().c_str());
	logStream.str("");

	logStream << "        Total faces: " << mesh->faceGroups.begin()->second.size();
	gameSystem->log(GameSystem::LOG_VERBOSE, logStream.str().c_str());
	logStream.str("");
}

CollisionTree::~CollisionTree() {
	delete(entities);
}

std::vector<size_t> CollisionTree::traverseTree(Mesh* mesh, CollisionEntity* entity, Vector2 position, float dist) {
	std::vector<size_t> vertexList;

	// see if the point is within the bounding area
	if(
			entity->subEntities.size() == 0 ||
			(position.x >= entity->ll.x && position.y >= entity->ll.y && position.x <= entity->ur.x && position.y <= entity->ur.y) ||

			(position.x + dist >= entity->ll.x && position.x < entity->ur.x && position.y >= entity->ll.y && position.y <= entity->ur.y) ||
			(position.y + dist >= entity->ll.y && position.y < entity->ur.y && position.x >= entity->ll.x && position.x <= entity->ur.x) ||
			(position.x - dist <= entity->ur.x && position.x > entity->ll.x && position.y >= entity->ll.y && position.y <= entity->ur.y) ||
			(position.y - dist <= entity->ur.y && position.y > entity->ll.y && position.x >= entity->ll.x && position.x <= entity->ur.x) ||

			distance(position, entity->ll) <= dist ||
			distance(position, entity->ur) <= dist ||
			distance(position, Vector2(entity->ll.x, entity->ur.y)) <= dist ||
			distance(position, Vector2(entity->ur.x, entity->ll.y)) <= dist
		) {
		if(entity->subEntities.size() == 0) {
			// no further subnodes... test and return (if applicable) entry
			if(distance(position, Vector2(mesh->vertices[entity->myVertex].x, mesh->vertices[entity->myVertex].z)) <= dist)
				vertexList.push_back(entity->myVertex);
		} else {
			// traverse subnodes
			for(size_t i = 0; i < entity->subEntities.size(); ++i) {
				std::vector<size_t> newVerts = traverseTree(mesh, entity->subEntities[i], position, dist);

				for(size_t p = 0; p < newVerts.size(); ++p)
					vertexList.push_back(newVerts[p]);
			}
		}
	}

	return vertexList;
}

std::vector<size_t> CollisionTree::getFaces(Vector2 position, float dist) {
	std::vector<size_t> vertexList = traverseTree(mesh, entities, position, dist);

	std::vector<size_t> faceList;
	std::map<size_t, bool> noDoubles;

	for(size_t i = 0; i < vertexList.size(); ++i) {
		for(size_t p = 0; p < faceMap[vertexList[i]].size(); ++p) {
			if(noDoubles.find(faceMap[vertexList[i]][p]) == noDoubles.end()) {
				faceList.push_back(faceMap[vertexList[i]][p]);

				noDoubles[faceMap[vertexList[i]][p]] = true;
			}
		}
	}

	return faceList;
}

CollisionEntity::CollisionEntity(
		Mesh* mesh,
		std::vector<size_t>& vertices,
		std::map< size_t, std::vector<size_t> >& faceMap,
		unsigned int depth,
		unsigned int& maxDepth,
		unsigned int& totalNodes,
		Vector2 ll,
		Vector2 ur
	) : ll(ll), ur(ur) {
	// do tracking
	++totalNodes;
	if(depth > maxDepth) maxDepth = depth;

	// if there is only one vertex left, it is ours and we are done
	if(vertices.size() == 1) {
		myVertex = vertices[0];

		return;
	}

	// if not, then subdivide up to four times
	Vector2 midpoint = ll + (ur - ll) / 2.0f;

	std::vector<size_t> swVerts, nwVerts, neVerts, seVerts;

	for(size_t i = 0; i < vertices.size(); ++i) {
		Vector2 xzRep(mesh->vertices[vertices[i]].x, mesh->vertices[vertices[i]].z);

		if(xzRep.x < midpoint.x && xzRep.y < midpoint.y) swVerts.push_back(vertices[i]);
		else if(xzRep.x < midpoint.x && xzRep.y >= midpoint.y) nwVerts.push_back(vertices[i]);
		else if(xzRep.x >= midpoint.x && xzRep.y >= midpoint.y) neVerts.push_back(vertices[i]);
		else if(xzRep.x >= midpoint.x && xzRep.y < midpoint.y) seVerts.push_back(vertices[i]);
	}

	if(swVerts.size() > 0) {
		subEntities.push_back(new CollisionEntity(
				mesh,
				swVerts,
				faceMap,
				depth + 1,
				maxDepth,
				totalNodes,
				ll,
				midpoint
			));
	}

	if(nwVerts.size() > 0) {
		subEntities.push_back(new CollisionEntity(
				mesh,
				nwVerts,
				faceMap,
				depth + 1,
				maxDepth,
				totalNodes,
				Vector2(ll.x, midpoint.y),
				Vector2(midpoint.x, ur.y)
			));
	}

	if(neVerts.size() > 0) {
		subEntities.push_back(new CollisionEntity(
				mesh,
				neVerts,
				faceMap,
				depth + 1,
				maxDepth,
				totalNodes,
				midpoint,
				ur
			));
	}

	if(seVerts.size() > 0) {
		subEntities.push_back(new CollisionEntity(
				mesh,
				seVerts,
				faceMap,
				depth + 1,
				maxDepth,
				totalNodes,
				Vector2(midpoint.x, ll.y),
				Vector2(ur.x, midpoint.y)
			));
	}
}

CollisionEntity::~CollisionEntity() {
	for(size_t i = 0; i < subEntities.size(); ++i)
		delete(subEntities[i]);
}
