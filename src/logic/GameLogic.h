// GameLogic.h
// Crucible Island

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <string>

#include "core/MainLoopMember.h"
#include "graphics/DrawTypes.h"
#include "graphics/UILayoutAuthority.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "logic/Camera.h"
#include "math/VectorMath.h"

class GameLogic : public MainLoopMember {
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

public:
	// focus tracking
	DrawStackEntry* activeMenuSelection;

	// main menu
	KeyListener* quitKeyListener;
	KeyListener* fullScreenKeyListener;
	KeyListener* mainMenuKeyListener;
	MouseMotionListener* mouseMotionListener;
	DrawStackEntry splashEntry;
	UIMetrics* mainMenuSpacer1Metrics;
	DrawStackEntry titleEntry;
	DrawStackEntry logoEntry;
	UIMetrics* mainMenuSpacer2Metrics;
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
	DrawStackEntry develStatsTitleEntry;
	DrawStackEntry develStatsContentEntry;
	DrawStackEntry develStatsContainerEntry;

	// loading
	DrawStackEntry loadingEntry;

	// intro
	KeyListener* introKeyListener;
	MouseButtonListener* introMouseButtonListener;
	DrawStackEntry introHintEntry;

	// playing
	KeyListener* playingKeyListener;
	KeyListener* playingDevelopmentModeKeyListener;
	KeyAbsoluteListener* turretUpKeyListener;
	KeyAbsoluteListener* turretDownKeyListener;
	KeyAbsoluteListener* turretLeftKeyListener;
	KeyAbsoluteListener* turretRightKeyListener;
	KeyAbsoluteListener* cameraAheadKeyListener;
	MouseButtonListener* primaryFireClickListener1;
	MouseButtonListener* primaryFireClickListener2;
	MouseButtonListener* primaryFireClickListener3;
	MouseButtonListener* secondaryFireClickListener;
	MouseButtonListener* binocularsClickListener;
	DrawStackEntry controlSpotEntry;
	DrawStackEntry cursorEntry;
	DrawStackEntry missileIndicators;
	DrawStackEntry scoreLabel;
	DrawStackEntry playingTipEntry;
	DrawStackEntry gaugePanelEntry;
	DrawStackEntry radarEntry;
	DrawStackEntry skyEntry;
	DrawStackEntry waterEntry;
	DrawStackEntry terrainEntry;
	DrawStackEntry shipEntry;
	DrawStackEntry fortressEntry;
	DrawStackEntry missileTrailEntry;
	DrawStackEntry missileEntry;
	DrawStackEntry explosionEntry;
	DrawStackEntry shellEntry;
	DrawStackEntry strikeEffectEntry;
	DrawStackEntry develControlsTitleEntry;
	DrawStackEntry develControlsContentEntry;
	DrawStackEntry develControlsContainerEntry;

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
	DrawStackEntry framerateLimitingEntry;
	MouseZoneListener* framerateLimitingButtonZoneListener;
	MouseButtonListener* framerateLimitingButtonClickListener;
	DrawStackEntry multisamplingLevelEntry;
	MouseZoneListener* multisamplingButtonZoneListener;
	MouseButtonListener* multisamplingButtonClickListener;
	DrawStackEntry terrainDetailEntry;
	MouseZoneListener* terrainDetailButtonZoneListener;
	MouseButtonListener* terrainDetailButtonClickListener;
	DrawStackEntry developmentModeEntry;
	MouseZoneListener* developmentModeButtonZoneListener;
	MouseButtonListener* developmentModeButtonClickListener;
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

	// cameras
	IntroCamera introCamera;
	FortressCamera fortressCamera;
	OrbitCamera orbitCamera;
	PresentationCamera presentationCamera;
	RoamingCamera roamingCamera;

	// general logic info
	bool mouseActive;
	std::string playerName;
	unsigned int deleteKeyPressTime;
	unsigned int lastCharacterDeletionTime;
	unsigned int leftArrowPressTime, rightArrowPressTime, upArrowPressTime, downArrowPressTime;
	unsigned int lastDevelInfoUpdate;
	unsigned int lastGameTimeUpdate;

//FIXME probably delete this fugly functionality
	Vector2 keyboardCursorPosition;

	// time management
	unsigned int lastUpdate;

	// object management
	GameLogic();
	~GameLogic();

//FIXME make this private again once schemes is incorporated herein
public:
	void reScheme();

	unsigned int execute(bool unScheduled = false);
};

#endif // GAMELOGIC_H
