/*
 * Ship.h
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
