// World.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "state/World.h"

World::World() {
	// randomly generate our input variables within the set limits
//	#define TERRAIN_MAXWIDTH 2.0f
//	#define TERRAIN_MAXHEIGHT 0.5f
	#define TERRAIN_MAXWIDTH 1000.0f
	#define TERRAIN_MAXHEIGHT 100.0f
	density = 256;
	const float rough = 0.5f;
	const float gradDist = 0.5f;
	const int blends = 4;
	const float sink = 0.5f;
	const float textureRepeat = 10.0f;

	height = TERRAIN_MAXHEIGHT;
	diameter = TERRAIN_MAXWIDTH;
//srand(632);
	// generate the initial diamond-square heightmap (it repeats 2x over width/length)
	DiamondSquare diamondSquare(density / 2, rough);

	// set the initial terrain values from the heightmap and re-map to positive (except sink)
	float dsHeightMap[density][density];

	for(size_t i = 0; i < density; ++i) {
		for(size_t p = 0; p < density; ++p) {
			int realI = (i >= density / 2 ? i - density / 2 : i);
			int realP = (p >= density / 2 ? p - density / 2 : p);

			dsHeightMap[i][p] = (diamondSquare.data[realI][realP] + 1.0f - sink) * (1 / (2.0f - sink));
		}
	}

	// blending
	for(int q = 0; q < blends; ++q) {
		float blendData[density][density];
		memcpy((void*) blendData, (const void*) dsHeightMap, density * density * sizeof(float));

		for(unsigned int i = 0; i < density; ++i) {
			for(unsigned int p = 0; p < density; ++p) {
				unsigned int
							nCoord = (p + 1 == density ? 0 : p + 1),
							sCoord = (p == 0 ? density - 1 : p - 1),
							wCoord = (i == 0 ? density - 1 : i - 1),
							eCoord = (i + 1 == density ? 0 : i + 1);

				blendData[i][p] =
						dsHeightMap[i][p] * 0.1f +
						(dsHeightMap[wCoord][sCoord] +
								dsHeightMap[wCoord][nCoord] +
								dsHeightMap[eCoord][nCoord] +
								dsHeightMap[eCoord][sCoord]) / 4.0f * 0.4f +
						(dsHeightMap[wCoord][p] +
								dsHeightMap[i][nCoord] +
								dsHeightMap[eCoord][p] +
								dsHeightMap[i][sCoord]) / 4.0f * 0.5;
			}
		}

		memcpy((void*) dsHeightMap, (const void*) blendData, density * density * sizeof(float));
	}

	// generate an "alphaBump" mappings for height variance in island
	float alphaHeightMap[density][density];

	Vector2 midpoint = Vector2(0.0f, 0.0f);

	// fill the mask with values
	for(unsigned int p = 0; p < density; ++p) {
		for(unsigned int j = 0; j < density; ++j) {
			// coordinate of these indices
			Vector2 coord(
				(float) p / (float) density * 2.0f - 1.0f,
				(float) j / (float) density * 2.0f - 1.0f
			);

			// vary the circumference
			float dist = distance(coord, midpoint);

			if(dist < 1.0f - gradDist)
					alphaHeightMap[p][j] = 1.0f;
			else if(dist < 1.0f)
					alphaHeightMap[p][j] = (1.0f - dist) / gradDist;
			else
				alphaHeightMap[p][j] = 0.0f;
		}
	}

	// adjust the heightmap for the alpha values
	float comboHeightMap[density][density];

	for(unsigned int i = 0; i < density; ++i)
		for(unsigned int p = 0; p < density; ++p)
			comboHeightMap[i][p] = (dsHeightMap[i][p]) * alphaHeightMap[i][p];


	// re-map all the values to the range 1.0, sink
	float min = 0.0f, max = 0.0f;

	for(size_t i = 0; i < density; ++i) {
		for(size_t p = 0; p < density; ++p) {
			if(comboHeightMap[i][p] > max)
				max = comboHeightMap[i][p];
			else if(comboHeightMap[i][p] < min)
				min = comboHeightMap[i][p];
		}
	}

	float range = 0.0f - min + max;

	for(size_t i = 0; i < density; ++i)
		for(size_t p = 0; p < density; ++p)
			comboHeightMap[i][p] = (-min + comboHeightMap[i][p]) / range * (1.0f + sink) - sink;

	// create a mesh with that terrain data
	for(unsigned int i = 0; i < density; ++i) {
		for(unsigned int p = 0; p < density; ++p) {
			mesh.addVertex(Vector3(
					((float) i / (float) density * 2.0f - 1.0f) * diameter / 2.0f,
					comboHeightMap[i][p] * height - gameSystem->getFloat("terrainDepth"),
					((float) p / (float) density * 2.0f - 1.0f) * diameter / 2.0f
				));
			mesh.addTexCoord(Vector2(
					mesh.vertices.back().x / textureRepeat,
					mesh.vertices.back().z / textureRepeat
				));

			if(i > 0 && p > 0) {
				if(
						mesh.vertices[p - 1 + (i * density)].y > -gameSystem->getFloat("terrainDepth") ||
						mesh.vertices[p - 1 + ((i - 1) * density)].y > -gameSystem->getFloat("terrainDepth") ||
						mesh.vertices[p + ((i - 1) * density)].y > -gameSystem->getFloat("terrainDepth")
					)
					mesh.addFace(
							p - 1 + (i * density),
							p - 1 + ((i - 1) * density),
							p + ((i - 1) * density),
							0, 0, 0,
							p - 1 + (i * density),
							p - 1 + ((i - 1) * density),
							p + ((i - 1) * density)
						);

				if(
						mesh.vertices[p + ((i - 1) * density)].y > -gameSystem->getFloat("terrainDepth") ||
						mesh.vertices[p + i * density].y > -gameSystem->getFloat("terrainDepth") ||
						mesh.vertices[p - 1 + (i * density)].y > -gameSystem->getFloat("terrainDepth")
					)
					mesh.addFace(
							p + ((i - 1) * density),
							p + i * density,
							p - 1 + (i * density),
							0, 0, 0,
							p + ((i - 1) * density),
							p + i * density,
							p - 1 + (i * density)
						);
			}
		}
	}

	// determine the lowest Y value actually used and set the variable
	lowestY = 0.0f;
	for(
			std::vector<Mesh::Face>::iterator itr = mesh.faceGroups.begin()->second.begin();
			itr != mesh.faceGroups.begin()->second.end();
			++itr
		)
		for(size_t i = 0; i < 3; ++i)
			if(mesh.vertices[itr->vertices[i]].y < lowestY)
				lowestY = mesh.vertices[itr->vertices[i]].y;

/*
	mesh = Mesh();

mesh.addVertex(Vector3(-100.0f, 0.0f, -100.0f));
mesh.addVertex(Vector3(-100.0f, 0.0f, 0.0f));
mesh.addVertex(Vector3(-100.0f, 0.0f, 100.0f));

mesh.addVertex(Vector3(0.0f, 0.0f, -100.0f));
mesh.addVertex(Vector3(0.0f, 0.0f, 0.0f));
mesh.addVertex(Vector3(0.0f, 0.0f, 100.0f));

mesh.addVertex(Vector3(100.0f, 0.0f, -100.0f));
mesh.addVertex(Vector3(100.0f, 0.0f, 0.0f));
mesh.addVertex(Vector3(100.0f, 0.0f, 100.0f));



mesh.addTexCoord(Vector2(0.0f, 0.0f));
mesh.addTexCoord(Vector2(0.0f, 1.0f));
mesh.addTexCoord(Vector2(1.0f, 1.0f));
mesh.addTexCoord(Vector2(1.0f, 0.0f));

mesh.addFace(0, 1, 4, 0, 0, 0, 0, 1, 2);
mesh.addFace(4, 3, 0, 0, 0, 0, 2, 3, 0);

mesh.addFace(1, 2, 5, 0, 0, 0, 0, 1, 2);
mesh.addFace(5, 4, 1, 0, 0, 0, 2, 3, 0);

mesh.addFace(3, 4, 7, 0, 0, 0, 0, 1, 2);
mesh.addFace(7, 6, 3, 0, 0, 0, 2, 3, 0);

mesh.addFace(4, 5, 8, 0, 0, 0, 0, 1, 2);
mesh.addFace(8, 7, 4, 0, 0, 0, 2, 3, 0);
*/
	// auto-normal
	mesh.autoNormal();

	// detect the longest edge for collision detection purposes
	longestEdge = 0.0f;
	for(
			std::vector<Mesh::Face>::iterator itr = mesh.faceGroups.begin()->second.begin();
			itr != mesh.faceGroups.begin()->second.end();
			++itr
		) {
		if(distance(mesh.vertices[itr->vertices[0]], mesh.vertices[itr->vertices[1]]) > longestEdge)
			longestEdge = distance(mesh.vertices[itr->vertices[0]], mesh.vertices[itr->vertices[1]]);

		if(distance(mesh.vertices[itr->vertices[1]], mesh.vertices[itr->vertices[2]]) > longestEdge)
			longestEdge = distance(mesh.vertices[itr->vertices[1]], mesh.vertices[itr->vertices[2]]);

		if(distance(mesh.vertices[itr->vertices[2]], mesh.vertices[itr->vertices[0]]) > longestEdge)
			longestEdge = distance(mesh.vertices[itr->vertices[2]], mesh.vertices[itr->vertices[0]]);
	}

	// log it
	gameSystem->log(GameSystem::LOG_VERBOSE, "World mesh created");

	std::stringstream logStream;
	logStream << "        Density: " << density << "x" << density << " = " << density * density;
	gameSystem->log(GameSystem::LOG_VERBOSE, logStream.str().c_str());

	// build the collision quadtree
	collisionTree = new CollisionTree(&mesh);
}

World::~World() {
	delete(collisionTree);
}
