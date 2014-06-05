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
class GameLogic;
class MouseButtonListener;
class MouseMotionListener;
class MouseZoneListener;

// program headers
#include "audio/GameAudio.h"
#include "core/GameSystem.h"
#include "core/MainLoopMember.h"
#include "graphics/GameGraphics.h"
#include "graphics/2dgraphics/DrawCircle.h"
#include "graphics/2dgraphics/DrawGrayOut.h"
#include "graphics/2dgraphics/DrawRadar.h"
#include "graphics/2dgraphics/DrawLabel.h"
#include "graphics/2dgraphics/DrawTexture.h"
#include "graphics/3dgraphics/ShipRenderer.h"
#include "graphics/3dgraphics/SkyRenderer.h"
#include "graphics/3dgraphics/MissileRenderer.h"
#include "graphics/3dgraphics/ShellRenderer.h"
#include "graphics/3dgraphics/TerrainRenderer.h"
#include "graphics/3dgraphics/TowerRenderer.h"
#include "graphics/3dgraphics/WaterRenderer.h"
#include "graphics/texture/Texture.h"
#include "input/InputHandler.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "logic/Camera.h"
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
extern GameAudio* gameAudio;
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
	// UI elements
	UILayoutAuthority* uiLayoutAuthority;

	typedef std::map<std::string, void*> DrawStackArgList;
	typedef std::pair< std::string, DrawStackArgList > DrawStackEntry;
	typedef std::vector<DrawStackEntry> DrawStack;

	// main menu
	KeyListener* quitKeyListener;
	KeyListener* fullScreenKeyListener;
	KeyListener* mainMenuKeyListener;
	MouseMotionListener* mouseMotionListener;
	DrawStackEntry* activeMenuSelection;
	DrawStackEntry splashEntry;
	DrawStackEntry brandEntry;
	DrawStackEntry presentsEntry;
	DrawStackEntry titleEntry;
	DrawStackEntry logoEntry;
	UIMetrics* mainMenuSpacerMetrics;
	DrawStackEntry playButtonEntry;
	MouseZoneListener* playButtonZoneListener;
	MouseButtonListener* playButtonClickListener;
	DrawStackEntry settingsButtonEntry;
	MouseZoneListener* settingsButtonZoneListener;
	MouseButtonListener* settingsButtonClickListener;
	DrawStackEntry helpButtonEntry;
	MouseZoneListener* helpButtonZoneListener;
	MouseButtonListener* helpButtonClickListener;
	DrawStackEntry highScoresButtonEntry;
	MouseZoneListener* highScoresButtonZoneListener;
	MouseButtonListener* highScoresButtonClickListener;
	DrawStackEntry quitButtonEntry;
	MouseZoneListener* quitButtonZoneListener;
	MouseButtonListener* quitButtonClickListener;
	DrawStackEntry menuTip1Entry;
	DrawStackEntry menuTip2Entry;
	DrawStackEntry menuTip3Entry;

	// loading
	DrawStackEntry loadingEntry;

	// intro
	KeyListener* introKeyListener;
	MouseButtonListener* introMouseButtonListener;
	DrawStackEntry introHintEntry;

	// playing
	KeyListener* playingKeyListener;
	KeyAbsoluteListener* turretUpKeyListener;
	KeyAbsoluteListener* turretDownKeyListener;
	KeyAbsoluteListener* turretLeftKeyListener;
	KeyAbsoluteListener* turretRightKeyListener;
KeyAbsoluteListener* cameraAheadKeyListener;
	MouseButtonListener* primaryFireClickListener;
	MouseButtonListener* secondaryFireClickListener;
	DrawStackEntry controlSpotEntry;
	DrawStackEntry cursorEntry;
	DrawStackEntry clockLabel;
	DrawStackEntry scoreLabel;
	DrawStackEntry playingTipEntry;
	DrawStackEntry gaugePanelEntry;
	DrawStackEntry radarEntry;
	DrawStackEntry skyEntry;
	DrawStackEntry waterEntry;
	DrawStackEntry terrainEntry;
	DrawStackEntry shipEntry;
	DrawStackEntry towerEntry;
	DrawStackEntry missileEntry;
	DrawStackEntry shellEntry;

	// paused
	KeyListener* pausedMenuKeyListener;
	DrawStackEntry grayOutEntry;
	DrawStackEntry pausedEntry;
	DrawStackEntry controlsTitleEntry;
	DrawStackEntry controlsEntry;
	UIMetrics* pausedMenuSpacerMetrics;
	DrawStackEntry resumeButtonEntry;
	MouseZoneListener* resumeButtonZoneListener;
	MouseButtonListener* resumeButtonClickListener;
	DrawStackEntry endGameButtonEntry;
	MouseZoneListener* endGameButtonZoneListener;
	MouseButtonListener* endGameButtonClickListener;
	DrawStackEntry pausedMenuTipEntry;

	// gameover
	KeyListener* gameOverKeyListener;
	KeyAbsoluteListener* deleteKeyListener;
	DrawStackEntry gameOverTitleEntry;
	DrawStackEntry yourScoreTitleEntry;
	DrawStackEntry yourScoreEntry;
	DrawStackEntry highScoresSectionEntry;
	DrawStackEntry newHighScoreContainer;
	DrawStackEntry newHighScoreLabel;
	DrawStackEntry newHighScoreNameLabel;
	DrawStackEntry newHighScoreNameField;
	DrawStackEntry gameOverContinueButton;
	MouseZoneListener* gameOverContinueButtonZoneListener;
	MouseButtonListener* gameOverContinueButtonClickListener;

	// settings menu
	KeyListener* settingsMenuKeyListener;
	DrawStackEntry settingsMenuTitleEntry;
	DrawStackEntry levelSettingEntry;
	MouseZoneListener* levelButtonZoneListener;
	MouseButtonListener* levelButtonClickListener;
	DrawStackEntry musicSettingEntry;
	MouseZoneListener* musicButtonZoneListener;
	MouseButtonListener* musicButtonClickListener;
	DrawStackEntry audioEffectsSettingEntry;
	MouseZoneListener* audioEffectsButtonZoneListener;
	MouseButtonListener* audioEffectsButtonClickListener;
	DrawStackEntry fullscreenSettingEntry;
	MouseZoneListener* fullscreenButtonZoneListener;
	MouseButtonListener* fullscreenButtonClickListener;
	DrawStackEntry windowedScreenResolutionEntry;
	MouseZoneListener* windowedScreenResolutionButtonZoneListener;
	MouseButtonListener* windowedScreenResolutionButtonClickListener;
	DrawStackEntry resetHighScoresEntry;
	MouseZoneListener* resetHighScoresButtonZoneListener;
	MouseButtonListener* resetHighScoresButtonClickListener;

	// high scores menu
	KeyListener* highScoresMenuKeyListener;
	DrawStackEntry highScoresTitleEntry;
	DrawStackEntry highScoresLabelEntry;

	// help menu
	KeyListener* helpMenuKeyListener;
	DrawStackEntry helpTitleEntry;
	DrawStackEntry instructionsTitleEntry;
	DrawStackEntry instructionsEntry;
	DrawStackEntry aboutButtonEntry;
	DrawStackEntry backButtonEntry;
	MouseZoneListener* aboutButtonZoneListener;
	MouseButtonListener* aboutButtonClickListener;
	MouseZoneListener* backButtonZoneListener;
	MouseButtonListener* backButtonClickListener;

	// about menu
	KeyListener* aboutMenuKeyListener;
	DrawStackEntry aboutTitleEntry;
	DrawStackEntry versionTitleEntry;
	DrawStackEntry versionEntry;
	DrawStackEntry creditsTitleEntry;
	DrawStackEntry creditsEntry;

DrawStackEntry fpsEntry;

	// stack of stuff to render each frame
	DrawStack drawStack;

	// cameras
	IntroCamera introCamera;
	TowerCamera towerCamera;
	OrbitCamera orbitCamera;
	PresentationCamera presentationCamera;
	RoamingCamera roamingCamera;

	// general logic info
	bool mouseActive;
	std::string playerName;
	unsigned int deleteKeyPressTime;
	unsigned int lastCharacterDeletionTime;
	unsigned int leftArrowPressTime, rightArrowPressTime, upArrowPressTime, downArrowPressTime;

//FIXME probably delete this fugly functionality
	Vector2 keyboardCursorPosition;
//	unsigned int lastClockUpdate;
//	unsigned int lastFrameTime;

	// time management
	unsigned int lastUpdate;

	// object management
	GameLogic();
	~GameLogic();

private:
	enum {
			SCHEME_MAINMENU,
			SCHEME_LOADING,
			SCHEME_INTRO,
			SCHEME_PLAYING,
			SCHEME_PAUSED,
			SCHEME_GAMEOVER,
			SCHEME_SETTINGS,
			SCHEME_HIGHSCORES,
			SCHEME_HELP,
			SCHEME_ABOUT
		} currentScheme;

	void reScheme();

public:
	// looping
	unsigned int execute(bool unScheduled = false);
};

#endif // GAMELOGIC_H
