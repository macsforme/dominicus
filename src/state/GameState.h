// GameState.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef GAMESTATE_H
#define GAMESTATE_H

// forward declarations
class Ship;
class ShipControl;

// program headers
#include "core/GameSystem.h"
#include "core/MainLoopMember.h"
#include "geometry/DiamondSquare.h"
#include "geometry/Mesh.h"
#include "state/Ship.h"
#include "state/World.h"
#include "graphics/texture/Texture.h"

// library headers
#include <vector>

// global variables
extern GameSystem* gameSystem;

// class definition
class GameState : public MainLoopMember {
public:
	World* world;

	std::vector<Ship*> ships;

	Texture* noiseTexture;
	Texture* fourDepthNoiseTexture;

	GameState();
	~GameState();

	unsigned int execute();
};

#endif // GAMESTATE_H
