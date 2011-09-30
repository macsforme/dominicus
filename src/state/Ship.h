// Ship.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef SHIP_H
#define SHIP_H

// forward declarations
class GameState;

// program headers
#include "core/GameSystem.h"
#include "geometry/Mesh.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "state/Collisions.h"
#include "state/GameState.h"

// global variables
extern GameState* gameState;
extern GameSystem* gameSystem;
extern Platform* platform;

class Ship {
public:
	enum Team {
		TEAM_DESERT,
		TEAM_FOREST,
		TEAM_AQUA
	};

	Team team;
	float power;
	Vector3 position;
	Vector3 direction;
	float pitch, roll;
	float speed;

	bool acceleration;
	Vector2 steering;

private:
	unsigned int lastUpdate;

public:
	Ship();

	void execute();
};

#endif
