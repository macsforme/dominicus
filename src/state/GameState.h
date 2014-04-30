// GameState.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef GAMESTATE_H
#define GAMESTATE_H

// program headers
#include "core/GameSystem.h"
#include "core/MainLoopMember.h"
#include "math/VectorMath.h"
#include "math/MatrixMath.h"
#include "geometry/DiamondSquare.h"
#include "geometry/Mesh.h"

// library headers
#include <vector>

// global variables
extern GameSystem* gameSystem;

// class definitions
class Fortress {
public:
	float health;
	float ammunition;
	float shock;

	Vector3 position;
	float rotation;
	float tilt;

	float turretRecoil;

	Fortress() :
			health(1.0f),
			ammunition(1.0f),
			shock(0.0f),
			position(Vector3(0.0f, 0.0f, 0.0f)),
			rotation(0.0f),
			tilt(0.0f),
			turretRecoil(0.0f) { };
};

class Shell {
public:
	Vector3 position;
	Vector3 direction;
};

class Ship {
public:
	Vector3 position;
	float rotation;
	float originAngle;
};

class Missile {
public:
	Vector3 position;
	Vector3 direction;
	float speed;
};

class GameState : public MainLoopMember {
public:
	Mesh island;
	Fortress fortress;
	std::vector<Shell> shells;
	std::vector<Ship> ships;
	std::vector<Missile> missiles;

	bool isPaused;
	unsigned int gameTimeMargin;

	unsigned int execute();

	GameState();
	~GameState();

	unsigned int getGameMills();
	void pause();
	void resume();
};

#endif // GAMESTATE_H
