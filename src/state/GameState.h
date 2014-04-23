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
	Vector3 position;
	float rotation;
	float tilt;

	float turretRecoil;

	Fortress() : position(Vector3(0.0f, 0.0f, 0.0f)), rotation(0.0f), tilt(0.0f), turretRecoil(0.0f) { };
};

class Shell {
public:
	Vector3 position;
	Vector3 direction;
};

class ShockField {
public:
	float span;
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
	ShockField shockField;
	std::vector<Ship> ships;
	std::vector<Missile> missiles;

	GameState();
	~GameState();

	unsigned int execute();
};

#endif // GAMESTATE_H
