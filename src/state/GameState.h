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
#include "math/MiscMath.h"
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

	void addRotation(float value) {
		rotation += value;
		while(rotation > 360.0f) rotation -= 360.0f;
		while(rotation < 0.0f) rotation += 360.0f;
	}
	void addTilt(float value) {
		tilt += value;
		if(tilt > gameSystem->getFloat("stateFortressMaximumTilt"))
			tilt = gameSystem->getFloat("stateFortressMaximumTilt");
		else if(tilt < gameSystem->getFloat("stateFortressMinimumTilt"))
			tilt = gameSystem->getFloat("stateFortressMinimumTilt");
	}

	void missileStrike() {
		health -= gameSystem->getFloat("stateMissileStrikeDepletion");

		if(health < 0.0f)
			health = 0.0f;
	}
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
	size_t originShip;
	unsigned int launchTime;
	Vector3 originPosition;
	bool alive;

	Vector3 position;
	float rotation;
	float tilt;
};

class GameState : public MainLoopMember {
private:
	Vector3 shipMissileOrigin;

public:
	Mesh island;
	Fortress fortress;
	std::vector<Shell> shells;
	std::vector<Ship> ships;
	std::vector<Missile> missiles;

	bool isPaused;
	unsigned int lastUpdateGameTime;
	int gameTimeMargin;

	int lastSimulationUpdate;
	unsigned int execute();

	GameState();
	~GameState();

	unsigned int getGameMills();
	void pause();
	void resume();
	void bumpStart();
};

#endif // GAMESTATE_H
