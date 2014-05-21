// Schemes.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef SCHEMES_H
#define SCHEMES_H

// program headers
#include "graphics/GameGraphics.h"
#include "graphics/text/TextBlock.h"
#include "graphics/2dgraphics/DrawProgressBar.h"
#include "graphics/2dgraphics/DrawButton.h"
#include "graphics/2dgraphics/DrawContainer.h"
#include "graphics/2dgraphics/DrawLabel.h"
#include "graphics/2dgraphics/DrawField.h"
#include "graphics/2dgraphics/DrawTexture.h"
#include "logic/GameLogic.h"
#include "logic/UILayoutAuthority.h"
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "state/GameState.h"

// library headers
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>

// global variables
extern GameGraphics* gameGraphics;
extern GameLogic* gameLogic;
extern GameState* gameState;

// class definition
class Schemes {
public:
	static void mainMenuScheme();
	static void loadingScheme();
	static void introScheme();
	static void playingScheme();
	static void pausedScheme();
	static void gameOverScheme();
	static void settingsScheme();
	static void highScoresScheme();
	static void helpScheme();
	static void aboutScheme();
};

#endif // SCHEMES_H
