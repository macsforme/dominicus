// dominicusMain.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.


#ifndef DOMINICUSMAIN_H
#define DOMINICUSMAIN_H

// program headers
#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "input/InputHandler.h"
#include "input/Keyboard.h"
#include "logic/GameLogic.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "state/GameState.h"

// library headers
#include <map>
#include <SDL/SDL.h>
#include <vector>

// main game function declaration
int dominicusMain(int argc, char* argv[]);

#endif
