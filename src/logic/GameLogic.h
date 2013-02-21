// GameLogic.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

// forward declarations
class FollowCamera;
class GameLogic;
class MouseButtonListener;
class MouseMotionListener;
class MouseZoneListener;
class ShipRelativeCamera;
class TerrainRelativeCamera;
class FollowCamera;

// program headers
#include "core/GameSystem.h"
#include "core/MainLoopMember.h"
#include "graphics/GameGraphics.h"
#include "graphics/2dgraphics/cursor/DrawCursor.h"
#include "graphics/2dgraphics/ui/DrawGrayOut.h"
#include "graphics/2dgraphics/ui/DrawRadar.h"
#include "graphics/2dgraphics/ui/components/DrawLabel.h"
#include "graphics/2dgraphics/ui/components/DrawTexture.h"
#include "graphics/3dgraphics/renderers/ShipRenderer.h"
#include "graphics/3dgraphics/renderers/SkyRenderer.h"
#include "graphics/3dgraphics/renderers/WaterRenderer.h"
#include "graphics/3dgraphics/renderers/TerrainRenderer.h"
#include "graphics/texture/Texture.h"
#include "input/InputHandler.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "logic/cameras/Camera.h"
//#include "logic/cameras/FollowCamera.h"
#include "logic/cameras/TestCameras.h"
#include "logic/cameras/FollowCamera.h"
#include "logic/Schemes.h"
#include "logic/UILayoutAuthority.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "state/GameState.h"

// library headers
#include <map>
#include <SDL.h>
#include <sstream>
#include <string>
#include <vector>

// global variables
extern GameGraphics* gameGraphics;
extern GameLogic* gameLogic;
extern GameState* gameState;
extern GameSystem* gameSystem;
extern InputHandler* inputHandler;
extern bool keepDominicusAlive;
extern std::map<MainLoopMember*,unsigned int> mainLoopModules;

// class definition
class GameLogic : public MainLoopMember {
public:
	// key listeners
	KeyListener* quitKeyListener;
	KeyListener* deleteKeyListener;
	KeyListener* fullScreenKeyListener;
	KeyListener* dashboardKeyListener;

	KeyListener* testKeyListener;
	KeyAbsoluteListener* testKeyAbsoluteListener;

	// mouse listeners
	MouseMotionListener* cursorMovementListener;

	MouseButtonListener* quitButtonClickListener;
	MouseButtonListener* startButtonClickListener;

	MouseZoneListener* quitButtonZoneListener;
	MouseZoneListener* startButtonZoneListener;

	// UI
	UILayoutAuthority* uiLayoutAuthority;

	// draw stack types
	typedef std::map<std::string, void*> DrawStackArgList;
	typedef std::pair< std::string, DrawStackArgList > DrawStackEntry;
	typedef std::vector<DrawStackEntry> DrawStack;

	// drawing stack entries for regenerating draw list
	DrawStackEntry splashEntry;

	DrawStackEntry titleEntry;

	DrawStackEntry quitButtonEntry;

	DrawStackEntry logoEntry;

	DrawStackEntry joinContainerEntry;
	DrawStackEntry joinHeaderLabelEntry;
	DrawStackEntry joinCallsignLabelEntry;
	DrawStackEntry joinCallsignFieldEntry;
	DrawStackEntry joinTeamLabelEntry;
	DrawStackEntry joinTeamMenuEntry;
	DrawStackEntry joinButtonEntry;
	DrawStackEntry joinStatusLabelEntry;

	DrawStackEntry welcomeHelpEntry;

	DrawStackEntry consoleEntry;

	DrawStackEntry playerInfoEntry;
	DrawStackEntry debugInfoEntry;

	DrawStackEntry grayOutEntry;
	DrawStackEntry controlsHelpEntry;
	DrawStackEntry scoreboardEntry;

	DrawStackEntry controlBoxEntry;
	DrawStackEntry cursorEntry;

	DrawStackEntry hintEntry;

	DrawStackEntry radarContainerEntry;
	DrawStackEntry radarEntry;

	DrawStackEntry ship1Entry;
	DrawStackEntry ship2Entry;

	DrawStackEntry skyEntry;

	DrawStackEntry waterEntry;

	DrawStackEntry terrain1Entry;
	DrawStackEntry terrain2Entry;


	// stack of stuff to render each frame
	DrawStack drawStack;

	// cameras
	ShipRelativeCamera* shipRelativeCamera;
	TerrainRelativeCamera* terrainRelativeCamera;
	FollowCamera* followCamera;

	// general logic info
	unsigned int lastClockUpdate;
	std::string myCallsign;
	unsigned int lastFrameTime;
	unsigned int hintExpiration;

	// object management
	GameLogic();
	~GameLogic();

private:
	enum {
			SCHEME_WELCOME,
			SCHEME_PLAYING,
			SCHEME_DASHBOARD
		} currentScheme;

	void reScheme();

public:
	// looping
	unsigned int execute();
};

#endif // GAMELOGIC_H
