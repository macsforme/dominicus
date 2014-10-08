// GameLogic.h
// Crucible Island

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <cstdlib>
#include <string>
#include <vector>

#include "core/MainLoopMember.h"
#include "graphics/DrawTypes.h"
#include "graphics/UILayoutAuthority.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "logic/Camera.h"

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

	void syncButtonWidths(std::vector<DrawStackEntry*> buttons);

	void reScheme();

	// logic helper functions resulting from clicks or enter key
	void startNewGame();
	void bumpStartFromIntro();
	void pauseGame();
	void resumeGame();
	void endGameFromPause();
	void continueFromGameOver();
	void alterGameLevel(bool increase = true);
	void alterMusicLevel(bool increase = true);
	void alterAudioEffectsLevel(bool increase = true);
	void alterFullscreenSetting(bool increase = true);
	void alterWindowedResolution(bool increase = true);
	void alterFramerateLimiting(bool increase = true);
	void alterMultiSamplingLevel(bool increase = true);
	void alterTerrainDetail(bool increase = true);
	void alterDevelopmentMode(bool increase = true);
	void resetHighScores();

	void playEffectAtDistance(std::string effect, float distance);

public:
	// main menu
	KeyListener* quitKeyListener;
	KeyListener* fullScreenKeyListener;
	KeyListener* mainMenuKeyListener;
	DrawStackEntry* activeMenuSelection;
	MouseMotionListener* mouseMotionListener;
	DrawStackEntry splashEntry;
	DrawStackEntry mainMenuTitleEntry;
	DrawStackEntry logoEntry;
	UIMetrics* spacerMetrics;
	DrawStackEntry playButtonEntry;
	MouseZoneListener* playButtonZoneListener;
	MouseButtonListener* playButtonClickListener;
	DrawStackEntry settingsButtonEntry;
	MouseZoneListener* settingsButtonZoneListener;
	MouseButtonListener* settingsButtonClickListener;
	DrawStackEntry highScoresButtonEntry;
	MouseZoneListener* highScoresButtonZoneListener;
	MouseButtonListener* highScoresButtonClickListener;
	DrawStackEntry helpButtonEntry;
	MouseZoneListener* helpButtonZoneListener;
	MouseButtonListener* helpButtonClickListener;
	DrawStackEntry quitButtonEntry;
	MouseZoneListener* quitButtonZoneListener;
	MouseButtonListener* quitButtonClickListener;
	DrawStackEntry menuTip1Entry;
	DrawStackEntry menuTip2Entry;
	DrawStackEntry menuTip3Entry;
	DrawStackEntry develStatsContainerEntry;
	DrawStackEntry develStatsTitleEntry;
	DrawStackEntry develStatsEntry;

	// loading
	DrawStackEntry loadingEntry;

	// intro
	KeyListener* introKeyListener;
	DrawStackEntry skyEntry;
	DrawStackEntry waterEntry;
	DrawStackEntry shipEntry;
	DrawStackEntry missileEntry;
	DrawStackEntry missileTrailEntry;
	DrawStackEntry shellEntry;
	DrawStackEntry terrainEntry;
	DrawStackEntry explosionEntry;
	DrawStackEntry fortressEntry;

	// paused
	KeyListener* pausedMenuKeyListener;
	DrawStackEntry grayOutEntry;
	DrawStackEntry menuTitleEntry;
	DrawStackEntry controlsTitleEntry;
	DrawStackEntry controlsEntry;
	DrawStackEntry resumeButtonEntry;
	MouseZoneListener* resumeButtonZoneListener;
	MouseButtonListener* resumeButtonClickListener;
	DrawStackEntry endGameButtonEntry;
	MouseZoneListener* endGameButtonZoneListener;
	MouseButtonListener* endGameButtonClickListener;

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
	DrawStackEntry strikeEffectEntry;
	DrawStackEntry missileIndicators;
	DrawStackEntry scoreLabel;
	DrawStackEntry gaugePanelEntry;
	DrawStackEntry radarEntry;
	DrawStackEntry develControlsContainerEntry;
	DrawStackEntry develControlsTitleEntry;
	DrawStackEntry develControlsEntry;
	DrawStackEntry controlSpotEntry;
	DrawStackEntry cursorEntry;

	// gameover
	KeyListener* gameOverKeyListener;
	KeyAbsoluteListener* deleteKeyListener;
	DrawStackEntry yourScoreTitleEntry;
	DrawStackEntry yourScoreEntry;
	DrawStackEntry highScoresTitleEntry;
	DrawStackEntry highScoresEntry;
	DrawStackEntry newHighScoreTitleEntry;
	DrawStackEntry newHighScoreContainer;
	DrawStackEntry newHighScoreNameLabel;
	DrawStackEntry newHighScoreNameField;
	DrawStackEntry gameOverContinueButton;
	MouseZoneListener* gameOverContinueButtonZoneListener;
	MouseButtonListener* gameOverContinueButtonClickListener;

	// settings menu
	KeyListener* settingsMenuKeyListener;
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
	DrawStackEntry backButtonEntry;
	MouseZoneListener* backButtonZoneListener;
	MouseButtonListener* backButtonClickListener;

	// high scores menu
	KeyListener* highScoresMenuKeyListener;

	// help menu
	KeyListener* helpMenuKeyListener;
	DrawStackEntry instructionsTitleEntry;
	DrawStackEntry instructionsEntry;
	DrawStackEntry aboutButtonEntry;
	MouseZoneListener* aboutButtonZoneListener;
	MouseButtonListener* aboutButtonClickListener;

	// about menu
	KeyListener* aboutMenuKeyListener;
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

	// missile cache for audio effects
	std::vector<size_t> missileCache;

	// general logic info
	bool mouseActive;
	std::string playerName;
	unsigned int deleteKeyPressTime;
	unsigned int lastCharacterDeletionTime;
	unsigned int leftArrowPressTime, rightArrowPressTime, upArrowPressTime, downArrowPressTime;
	unsigned int lastDevelInfoUpdate;
	unsigned int lastGameTimeUpdate;

	// time management
	unsigned int lastUpdate;

	// object management
	GameLogic();
	~GameLogic();

	unsigned int execute(bool unScheduled = false);
};

#endif // GAMELOGIC_H
