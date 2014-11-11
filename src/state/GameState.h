// GameState.h
// Crucible Island

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <cstdlib>
#include <vector>

#include "core/MainLoopMember.h"
#include "geometry/Mesh.h"
#include "math/VectorMath.h"

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

	void addRotation(float value);
	void addTilt(float value);

	void missileStrike();
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
	unsigned int originTime;
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
	unsigned int getGameMills();

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

	GameState();
	~GameState();

	unsigned int execute(bool unScheduled = false);

	void pause();
	void resume();
	void bumpStart();

	void fireShell();

	unsigned int getFiringInterval();
	unsigned int getShipAdditionInterval();
	unsigned int getCriticalTime();
	unsigned int getNumberOfShipsAtTime(unsigned int time);
	unsigned int getShipOriginTime(size_t ship);
};

#endif // GAMESTATE_H
