// GameState.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "state/GameState.h"
#include <iostream>
GameState::GameState() {
	// randomly generate the island
//FIXME this stuff should go into game standards
	size_t density = 256;
	const float rough = 0.5f;
	const float gradDist = 0.5f;
	const int blends = 4;
	const float sink = 0.5f;
	float height = gameSystem->getFloat("islandMaximumHeight");
	float diameter = gameSystem->getFloat("islandMaximumWidth");
	float textureStretch = 10.0f;

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
			island.addVertex(Vector3(
					((float) i / (float) density * 2.0f - 1.0f) * diameter / 2.0f,
					comboHeightMap[i][p] * height - gameSystem->getFloat("terrainDepth"),
					((float) p / (float) density * 2.0f - 1.0f) * diameter / 2.0f
				));
			island.addTexCoord(Vector2(
					island.vertices.back().x / textureStretch,
					island.vertices.back().z / textureStretch
				));

			if(i > 0 && p > 0) {
				if(
						island.vertices[p - 1 + (i * density)].y > -gameSystem->getFloat("terrainDepth") ||
						island.vertices[p - 1 + ((i - 1) * density)].y > -gameSystem->getFloat("terrainDepth") ||
						island.vertices[p + ((i - 1) * density)].y > -gameSystem->getFloat("terrainDepth")
					)
					island.addFace(
							p - 1 + (i * density),
							p - 1 + ((i - 1) * density),
							p + ((i - 1) * density),
							0, 0, 0,
							p - 1 + (i * density),
							p - 1 + ((i - 1) * density),
							p + ((i - 1) * density)
						);

				if(
						island.vertices[p + ((i - 1) * density)].y > -gameSystem->getFloat("terrainDepth") ||
						island.vertices[p + i * density].y > -gameSystem->getFloat("terrainDepth") ||
						island.vertices[p - 1 + (i * density)].y > -gameSystem->getFloat("terrainDepth")
					)
					island.addFace(
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

	// auto-normal
	island.autoNormal();

	// set the tower position
	fortress.position = Vector3(0.0f, 0.0f, 0.0f);

	for(
			std::vector<Vector3>::iterator itr = island.vertices.begin();
			itr != island.vertices.end();
			++itr
		)
		if(itr->y > fortress.position.y)
			fortress.position = *itr;

	// make ships
	Ship ship;
	ship.originAngle = 0.0f;

	ships.push_back(ship);

	// set start time
	isPaused = false;
	gameTimeMargin = platform->getExecMills();
}

GameState::~GameState() {
}

unsigned int GameState::execute() {
	// update/add ships as appropriate
	float shipOrbitDistance = (gameSystem->getFloat("islandMaximumWidth") * 0.5f + gameSystem->getFloat("stateShipOrbitMargin"));

	if(getGameMills() / 1000 / gameSystem->getFloat("stateShipAddRate") + 1 > ships.size()) {
		Ship ship;
static int originAngle = 0.0f; originAngle += 75.0f;
		ship.originAngle = originAngle;
		ships.push_back(ship);
	}

	for(size_t i = 0; i < ships.size(); ++i) {
		// determine phase
		float shipLifeTime = (float) (getGameMills() - i * gameSystem->getFloat("stateShipAddRate") * 1000) / 1000.0f;

		if(shipLifeTime > gameSystem->getFloat("stateShipEntryTime")) {
			// orbit phase
			ships[i].position = Vector3(-shipOrbitDistance - gameSystem->getFloat("stateShipMargin") * (float) i, 0.0f, 0.0f);
			Matrix3 rotationMatrix; rotationMatrix.identity();
			rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians((45.0f + (shipLifeTime - gameSystem->getFloat("stateShipEntryTime")) * gameSystem->getFloat("stateShipSpeed") / (2.0f * PI * (shipOrbitDistance + gameSystem->getFloat("stateShipMargin") * (float) i)) * 360.0f) * (i % 2 == 0 ? 1.0f : -1.0f) + ships[i].originAngle), rotationMatrix);
			ships[i].position = ships[i].position * rotationMatrix;

			ships[i].rotation = (45.0f + (shipLifeTime - gameSystem->getFloat("stateShipEntryTime")) * gameSystem->getFloat("stateShipSpeed") / (2.0f * PI * (shipOrbitDistance + gameSystem->getFloat("stateShipMargin") * (float) i)) * 360.0f - 90.0f) * (i % 2 == 0 ? 1.0f : -1.0f) + ships[i].originAngle;
		} else {
			float outerCircleRadius = -(shipOrbitDistance + gameSystem->getFloat("stateShipMargin") * (float) i) / (cos(radians(45.0f)) - 1.0f) - (shipOrbitDistance + gameSystem->getFloat("stateShipMargin") * (float) i);
			float entryPhaseDistance = outerCircleRadius * 2.0f * PI / 8.0f;
			float entryPhaseTime = entryPhaseDistance / gameSystem->getFloat("stateShipSpeed");

			if((gameSystem->getFloat("stateShipEntryTime") - shipLifeTime) * gameSystem->getFloat("stateShipSpeed") <= entryPhaseDistance) {
				// entry turn phase
				ships[i].position = Vector3(0.0f, 0.0f, -outerCircleRadius * (i % 2 == 0 ? 1.0f : -1.0f));
				Matrix3 rotationMatrix; rotationMatrix.identity();
				rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), -radians((shipLifeTime - (gameSystem->getFloat("stateShipEntryTime") - entryPhaseTime)) / entryPhaseTime * 45.0f) * (i % 2 == 0 ? 1.0f : -1.0f), rotationMatrix);
				ships[i].position = ships[i].position * rotationMatrix;
				ships[i].position += Vector3(-outerCircleRadius, 0.0f, outerCircleRadius * (i % 2 == 0 ? 1.0f : -1.0f));
				rotationMatrix.identity(); rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(ships[i].originAngle), rotationMatrix);
				ships[i].position = ships[i].position * rotationMatrix;

				ships[i].rotation = -((shipLifeTime - (gameSystem->getFloat("stateShipEntryTime") - entryPhaseTime)) / entryPhaseTime * 45.0f) * (i % 2 == 0 ? 1.0f : -1.0f) + ships[i].originAngle;
			} else {
				// approach phase
				ships[i].position = Vector3(-outerCircleRadius - gameSystem->getFloat("stateShipSpeed") * (gameSystem->getFloat("stateShipEntryTime") - entryPhaseTime - shipLifeTime), 0.0f, 0.0f);
				Matrix3 rotationMatrix; rotationMatrix.identity();
				rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(ships[i].originAngle), rotationMatrix);
				ships[i].position = ships[i].position * rotationMatrix;

				ships[i].rotation = ships[i].originAngle;
			}
		}
	}

	// calculate and return sleep time from superclass
	unsigned int frequency = (unsigned int) gameSystem->getFloat("stateUpdateFrequency");
	static const unsigned int idealSleepTime = (
			frequency != 0 ?
			1000 / frequency : 0
		);
	return getSleepTime(idealSleepTime);
}

//bool isPaused;
//unsigned int gameTimeMargin;

unsigned int GameState::getGameMills() {
	// execution time since game began (excluding pauses)
	if(isPaused)
		return(gameTimeMargin);
	else
		return platform->getExecMills() - gameTimeMargin;
}

void GameState::pause() {
	// freeze the game time
	if(isPaused)
		return;

	gameTimeMargin = getGameMills();
	isPaused = true;
}

void GameState::resume() {
	if(! isPaused)
		return;

	gameTimeMargin = platform->getExecMills() - gameTimeMargin;
	isPaused = false;
}
