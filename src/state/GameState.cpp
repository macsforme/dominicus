// GameState.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "state/GameState.h"

GameState::GameState() : MainLoopMember((unsigned int) gameSystem->getFloat("stateUpdateFrequency")) {
	// randomly generate the island
	size_t density = (size_t) gameSystem->getFloat("islandTerrainDensity");
	const float rough = gameSystem->getFloat("islandTerrainRoughness");
	const float gradDist = gameSystem->getFloat("islandTerrainGradDist");
	const int blends = (int) gameSystem->getFloat("islandTerrainBlends");
	const float sink = gameSystem->getFloat("islandTerrainSink");
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

	// determine missile origin
	Mesh shipMesh("ship");
	shipMissileOrigin = (
			shipMesh.vertices[shipMesh.faceGroups["missileorigin"][0].vertices[0]] +
			shipMesh.vertices[shipMesh.faceGroups["missileorigin"][0].vertices[1]] +
			shipMesh.vertices[shipMesh.faceGroups["missileorigin"][0].vertices[2]]
		) / 3.0f;

	// set start time
	isPaused = false;
	lastUpdateGameTime = 0;
	gameTimeMargin = platform->getExecMills();
	lastSimulationUpdate = 0;

	// set starting score
	score = 0;

	// initialize shock charging status
	shockIsCharging = false;
}

GameState::~GameState() {
}

unsigned int GameState::execute(bool unScheduled) {
	// mark current game time for this update for consistency
	lastUpdateGameTime = getGameMills();

	// get a delta time for stuff that doesn't use precomputed state
	float deltaTime = 0.0f;
	if(! isPaused) {
		unsigned int currentGameTime = getGameMills();
		deltaTime = (float) (currentGameTime - lastSimulationUpdate) / 1000.0f;
		lastSimulationUpdate = currentGameTime;
	}

	// update/add ships as appropriate
	float shipOrbitDistance = (gameSystem->getFloat("islandMaximumWidth") * 0.5f + gameSystem->getFloat("stateShipOrbitMargin"));

	while(lastUpdateGameTime / 1000 / gameSystem->getFloat("stateShipAddRate") + 1 > ships.size()) {
		Ship ship;
		ship.originAngle = fortress.rotation;
		ships.push_back(ship);
	}

	for(size_t i = 0; i < ships.size(); ++i) {
		// determine phase
		float shipLifeTime = (float) (lastUpdateGameTime - i * gameSystem->getFloat("stateShipAddRate") * 1000) / 1000.0f;

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

	// update/add missiles as appropriate
	unsigned int* shipMissiles = new unsigned int [ships.size()];
	for(size_t i = 0; i < ships.size(); ++i)
		shipMissiles[i] = 0;

	for(size_t i = 0; i < missiles.size(); ++i) {
		for(size_t p = 0; p < ships.size(); ++p) {
			if(missiles[i].originShip == p) {
				++shipMissiles[p];

				break;
			}
		}
	}

	for(size_t i = 0; i < ships.size(); ++i) {
		float shipLifeTime = (float) (lastUpdateGameTime - i * gameSystem->getFloat("stateShipAddRate") * 1000) / 1000.0f;

		if(
				shipLifeTime > gameSystem->getFloat("stateShipEntryTime") &&
				(shipLifeTime - gameSystem->getFloat("stateShipEntryTime")) / gameSystem->getFloat("stateMissileFiringRate") > (float) shipMissiles[i]
			) {
			// this ship is due to fire a missile
			Missile missile;
			missile.originShip = i;
			missile.launchTime = lastUpdateGameTime;
			missile.originPosition = Vector3(0.0f, 0.0f, 0.0f);
			missile.alive = true;
			missile.position = Vector3(0.0f, 0.0f, 0.0f);
			missile.rotation = 0.0f;
			missile.tilt = 90.0f;

			missiles.push_back(missile);
		}
	}

	delete[] shipMissiles;

	for(size_t i = 0; i < missiles.size(); ++i) {
		if(! missiles[i].alive)
			continue;

		Matrix3 missileOriginMatrix; missileOriginMatrix.identity();
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(ships[missiles[i].originShip].rotation), missileOriginMatrix);
		Vector3 thisShipMissileOrigin(
				shipMissileOrigin.x,
				shipMissileOrigin.y,
				shipMissileOrigin.z
			);
		thisShipMissileOrigin = thisShipMissileOrigin * missileOriginMatrix;

		float distanceTraveled = (float) (lastUpdateGameTime - missiles[i].launchTime) / 1000.0f * gameSystem->getFloat("stateMissileSpeed");
		if(distanceTraveled <= shipMissileOrigin.y) {
			// still launching from ship
			missiles[i].position = Vector3(
					ships[missiles[i].originShip].position.x + thisShipMissileOrigin.x,
					distanceTraveled,
					ships[missiles[i].originShip].position.z + thisShipMissileOrigin.z
				);

			missiles[i].originPosition = missiles[i].position;
			missiles[i].originPosition.y = 0.0f;
			missiles[i].rotation = getAngle(Vector2(
					missiles[i].originPosition.x - fortress.position.x,
					missiles[i].originPosition.z - fortress.position.z
				)) + 180.0f;
		} else if(distanceTraveled <= fortress.position.y) {
			// below fortress altitude
			missiles[i].position.y = distanceTraveled;
		} else {
			// initial turn phase or final turn phase
			float lateralDistanceToTower = distance(missiles[i].originPosition, Vector3(fortress.position.x, 0.0f, fortress.position.y));
			static float phaseSplitFactor = 1.0f + cos(radians(45.0f)) * 1.0f / (1.0f - sin(radians(45.0f)));
			float initialTurnPhaseLatDist = 1.0f / phaseSplitFactor * lateralDistanceToTower;
			float finalTurnPhaseRadius = 1.0f / (1.0f - (sin(radians(45.0f)))) * initialTurnPhaseLatDist;
			float distanceTraveledIntoTurnPhases = distanceTraveled - fortress.position.y;

			if(distanceTraveledIntoTurnPhases < 0.5f * initialTurnPhaseLatDist * PI) {
				// initial turn phase
				float completionFactor = (distanceTraveledIntoTurnPhases / (0.5f * initialTurnPhaseLatDist * PI));

				missiles[i].position = Vector3(
						missiles[i].originPosition.x + (1.0f / (phaseSplitFactor)) * (fortress.position.x - missiles[i].originPosition.x) * (1.0f - cos(radians(completionFactor * 90.0f))),
						fortress.position.y + sin(radians(completionFactor * 90.0f)) * initialTurnPhaseLatDist,
						missiles[i].originPosition.z + (1.0f / (phaseSplitFactor)) * (fortress.position.z - missiles[i].originPosition.z) * (1.0f - cos(radians(completionFactor * 90.0f)))
					);
				missiles[i].tilt = (1.0f - completionFactor) * 90.0f;
			} else if(distanceTraveledIntoTurnPhases < 0.5f * initialTurnPhaseLatDist * PI + 0.25f * finalTurnPhaseRadius * PI) {
				// final turn phase
				float completionFactor = (distanceTraveledIntoTurnPhases - 0.5f * initialTurnPhaseLatDist * PI) / (0.25f * finalTurnPhaseRadius * PI);

				missiles[i].position = Vector3(
						missiles[i].originPosition.x + (1.0f / (phaseSplitFactor)) * (fortress.position.x - missiles[i].originPosition.x) +
								((phaseSplitFactor - 1.0f) / phaseSplitFactor) * (fortress.position.x - missiles[i].originPosition.x) * sin(radians(completionFactor * 45.0f)) / sin(radians(45.0f)),
						fortress.position.y + initialTurnPhaseLatDist * (cos(radians(completionFactor * 45.0f)) - cos(radians(45.0f))) / (cos(radians(0.0f)) - cos(radians(45.0f))),
						missiles[i].originPosition.z + (1.0f / (phaseSplitFactor)) * (fortress.position.z - missiles[i].originPosition.z) +
								((phaseSplitFactor - 1.0f) / phaseSplitFactor) * (fortress.position.z - missiles[i].originPosition.z) * sin(radians(completionFactor * 45.0f)) / sin(radians(45.0f))
					);
				missiles[i].tilt = 0.0f - completionFactor * 45.0f;
			} else {
				// end of path... boom
				missiles[i].alive = false;

				fortress.missileStrike();
			}
		}
	}

	// update EMP status (< 0 = firing, 0 = not in use, > 0 = charging
	if(fortress.shock < 0.0f) {
		// firing
		fortress.shock += deltaTime / gameSystem->getFloat("stateEMPDuration");

		if(fortress.shock > 0.0f)
			fortress.shock = 0.0f;
	} else if(shockIsCharging) {
		if(fortress.shock == 0.0f) {
			if(
					fortress.health > gameSystem->getFloat("stateEMPHealthCost") &&
					fortress.ammunition > gameSystem->getFloat("stateEMPFiringCost")
				) {
				// begin charging
				fortress.health -= gameSystem->getFloat("stateEMPHealthCost");
				fortress.ammunition -= gameSystem->getFloat("stateEMPFiringCost");

				fortress.shock += deltaTime / gameSystem->getFloat("stateEMPChargingTime");
			}
		} else {
			// continue charging
			fortress.shock += deltaTime / gameSystem->getFloat("stateEMPChargingTime");

			if(fortress.shock > 1.0f)
				fortress.shock = 1.0f;
		}
	} else if(fortress.shock > 0.0f) {
		if(fortress.shock == 1.0f) {
			// fire EMP
			fortress.shock = -1.0f;
		} else {
			// cancel charge and return stocks (overages corrected by subsequent code)
			fortress.shock = 0.0f;
			fortress.health += gameSystem->getFloat("stateEMPHealthCost");
			fortress.ammunition += gameSystem->getFloat("stateEMPFiringCost");
		}
	}

	// update health status
	if(fortress.health > 0.0f)
		fortress.health += deltaTime * gameSystem->getFloat("stateHealthRegenerationRate");
	if(fortress.health > 1.0f) fortress.health = 1.0f;

	// update ammo status
	fortress.ammunition += deltaTime * gameSystem->getFloat("stateAmmoFiringCost") * ships.size() / (int) gameSystem->getFloat("stateMissileFiringRate") * 2.0f;
	if(fortress.ammunition > 1.0f) fortress.ammunition = 1.0f;

	// missile/EMP collisions
	if(fortress.shock < 0.0f) {
		for(size_t i = 0; i < missiles.size(); ++i) {
			if(! missiles[i].alive)
				continue;

			if(distance(fortress.position, missiles[i].position) < (1.0f + fortress.shock) * gameSystem->getFloat("stateEMPRange")) {
				missiles[i].alive = false;
				++score;
			}
		}
	}

	// track runcount
	trackRunCount();

	// calculate and return sleep time from superclass
	if(unScheduled)
		return 0;
	else
		return getSleepTime();
}

unsigned int GameState::getGameMills() {
	// execution time since game began (excluding pauses)
	if(isPaused)
		return((unsigned int) gameTimeMargin);
	else
		return (unsigned int) (platform->getExecMills() - gameTimeMargin);
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

	gameTimeMargin = (int) platform->getExecMills() - gameTimeMargin;
	isPaused = false;
}

void GameState::bumpStart() {
	int fullEntryTime = (int) (gameSystem->getFloat("stateShipEntryTime") * 1000.0f);
	gameTimeMargin -= fullEntryTime - (int) getGameMills();
}
