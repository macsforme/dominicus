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
#include "graphics/2dgraphics/ui/components/DrawButton.h"
#include "graphics/2dgraphics/ui/components/DrawContainer.h"
#include "graphics/2dgraphics/ui/components/DrawLabel.h"
#include "graphics/2dgraphics/ui/components/DrawField.h"
#include "graphics/2dgraphics/ui/components/DrawTexture.h"
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
private:
	static void addQuitButton();
	static void addConsole();
	static void addRadar();

public:
	static void mainMenuScheme();
	static void settingsScheme();
	static void helpScheme();
	static void highScoresScheme();
	
	static void welcomeScheme(
			std::string callsignText
		);
	static void playingScheme(
			bool showHUD,
			std::string callsignText,
//			std::string team,
			std::string scoreText,
			std::string powerText,
			bool showDebugInfo
		);
	static void dashboardScheme();
};

#endif // SCHEMES_H
