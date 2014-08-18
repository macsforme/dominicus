// GamemState.h
// Crucible Island

#ifndef GAMESTATE_H
#define GAMESTATE_H

// program headers
#include "audio/GameAudio.h"
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
extern GameAudio* gameAudio;
extern GameSystem* gameSystem;

// class definitions
class Fortress {
public:
	float health;
	float ammunition;
	float emp; // > 1 = charging, > 0 = firing, 0 = at rest

	Vector3 position;
	float rotation;
	float tilt;

	Fortress() :
			health(1.0f),
			ammunition(1.0f),
			emp(0.0f),
			position(Vector3(0.0f, 0.0f, 0.0f)),
			rotation(0.0f),
			tilt(0.0f) { };

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
public:
	Mesh island;
	Fortress fortress;
	std::vector<Shell> shells;
	std::vector<Ship> ships;
	std::vector<Missile> missiles;
	unsigned int score;

	Vector3 shipMissileOrigin;
	Vector3 turretOrigin;
	Vector3 shellOrigin;
	float shellRadius;
	float missileLength;
	float missileRadius;

	bool binoculars;
	float recoil; // > 1 = recoiling, > 0 = recovering, 0 = at rest
	bool empIsCharging;
	unsigned int lastStrikeTime;
	bool isPaused;
	unsigned int lastUpdateGameTime;
	int gameTimeMargin;

	unsigned int execute(bool unScheduled = false);

	GameState();
	~GameState();

private:
	unsigned int getGameMills();
public:
	void pause();
	void resume();
	void bumpStart();

	void fireShell();

	void playEffectAtDistance(std::string effect, float distance = 0.0f);
};

#endif // GAMESTATE_H
