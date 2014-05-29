// GameLogic.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "logic/GameLogic.h"

GameLogic::GameLogic() :
		MainLoopMember((unsigned int) gameSystem->getFloat("logicUpdateFrequency")),
		mouseActive(false),
		keyboardCursorPosition(Vector2(0.0f, 0.0f)),
		playerName(""),
		deleteKeyPressTime(-1),
		lastCharacterDeletionTime(0) {
	// instantiate HUD layout authority
	uiLayoutAuthority = new UILayoutAuthority(
			Vector2(gameSystem->getFloat("hudElementMargin") / gameGraphics->resolutionX,
					gameSystem->getFloat("hudElementMargin") / gameGraphics->resolutionY)
		);

	// independent key listeners
	std::vector<SDLKey> keysVector;

	keysVector.push_back(SDLK_F12);
	quitKeyListener = new KeyListener(keysVector);
	keysVector.clear();

	keysVector.push_back(SDLK_F1);
	fullScreenKeyListener = new KeyListener(keysVector);
	keysVector.clear();

	// initialize draw info/listeners for all schemes
	mouseMotionListener = new MouseMotionListener();

	std::vector<SDLKey> mainMenuKeys;
	mainMenuKeys.push_back(SDLK_UP);
	mainMenuKeys.push_back(SDLK_DOWN);
	mainMenuKeys.push_back(SDLK_RETURN);
	mainMenuKeyListener = new KeyListener(mainMenuKeys);

	splashEntry.first = "splash";

	brandEntry.first = "texture";
	brandEntry.second["metrics"] = (void*) new UIMetrics;
	brandEntry.second["texture"] = (void*) new std::string;

	presentsEntry.first = "label";
	presentsEntry.second["metrics"] = (void*) new UIMetrics;
	presentsEntry.second["fontSize"] = (void*) new float;
	presentsEntry.second["fontColor"] = (void*) new Vector4;
	presentsEntry.second["text"] = (void*) new std::string;

	titleEntry.first = "label";
	titleEntry.second["metrics"] = (void*) new UIMetrics;
	titleEntry.second["fontSize"] = (void*) new float;
	titleEntry.second["fontColor"] = (void*) new Vector4;
	titleEntry.second["text"] = (void*) new std::string;

	logoEntry.first = "texture";
	logoEntry.second["metrics"] = (void*) new UIMetrics;
	logoEntry.second["texture"] = (void*) new std::string;

	mainMenuSpacerMetrics = new UIMetrics;

	playButtonEntry.first = "button";
	playButtonEntry.second["metrics"] = (void*) new UIMetrics;
	playButtonEntry.second["fontSize"] = (void*) new float;
	playButtonEntry.second["fontColor"] = (void*) new Vector4;
	playButtonEntry.second["text"] = (void*) new std::string;
	playButtonEntry.second["padding"] = (void*) new float;
	playButtonEntry.second["border"] = (void*) new float;
	playButtonEntry.second["softEdge"] = (void*) new float;
	playButtonEntry.second["insideColor"] = (void*) new Vector4;
	playButtonEntry.second["borderColor"] = (void*) new Vector4;
	playButtonEntry.second["outsideColor"] = (void*) new Vector4;
	playButtonZoneListener = new MouseZoneListener();
	playButtonClickListener = new MouseButtonListener();

	settingsButtonEntry.first = "button";
	settingsButtonEntry.second["metrics"] = (void*) new UIMetrics;
	settingsButtonEntry.second["fontSize"] = (void*) new float;
	settingsButtonEntry.second["fontColor"] = (void*) new Vector4;
	settingsButtonEntry.second["text"] = (void*) new std::string;
	settingsButtonEntry.second["padding"] = (void*) new float;
	settingsButtonEntry.second["border"] = (void*) new float;
	settingsButtonEntry.second["softEdge"] = (void*) new float;
	settingsButtonEntry.second["insideColor"] = (void*) new Vector4;
	settingsButtonEntry.second["borderColor"] = (void*) new Vector4;
	settingsButtonEntry.second["outsideColor"] = (void*) new Vector4;
	settingsButtonZoneListener = new MouseZoneListener();
	settingsButtonClickListener = new MouseButtonListener();

	helpButtonEntry.first = "button";
	helpButtonEntry.second["metrics"] = (void*) new UIMetrics;
	helpButtonEntry.second["fontSize"] = (void*) new float;
	helpButtonEntry.second["fontColor"] = (void*) new Vector4;
	helpButtonEntry.second["text"] = (void*) new std::string;
	helpButtonEntry.second["padding"] = (void*) new float;
	helpButtonEntry.second["border"] = (void*) new float;
	helpButtonEntry.second["softEdge"] = (void*) new float;
	helpButtonEntry.second["insideColor"] = (void*) new Vector4;
	helpButtonEntry.second["borderColor"] = (void*) new Vector4;
	helpButtonEntry.second["outsideColor"] = (void*) new Vector4;
	helpButtonZoneListener = new MouseZoneListener();
	helpButtonClickListener = new MouseButtonListener();

	highScoresButtonEntry.first = "button";
	highScoresButtonEntry.second["metrics"] = (void*) new UIMetrics;
	highScoresButtonEntry.second["fontSize"] = (void*) new float;
	highScoresButtonEntry.second["fontColor"] = (void*) new Vector4;
	highScoresButtonEntry.second["text"] = (void*) new std::string;
	highScoresButtonEntry.second["padding"] = (void*) new float;
	highScoresButtonEntry.second["border"] = (void*) new float;
	highScoresButtonEntry.second["softEdge"] = (void*) new float;
	highScoresButtonEntry.second["insideColor"] = (void*) new Vector4;
	highScoresButtonEntry.second["borderColor"] = (void*) new Vector4;
	highScoresButtonEntry.second["outsideColor"] = (void*) new Vector4;
	highScoresButtonZoneListener = new MouseZoneListener();
	highScoresButtonClickListener = new MouseButtonListener();

	quitButtonEntry.first = "button";
	quitButtonEntry.second["metrics"] = (void*) new UIMetrics;
	quitButtonEntry.second["fontSize"] = (void*) new float;
	quitButtonEntry.second["fontColor"] = (void*) new Vector4;
	quitButtonEntry.second["text"] = (void*) new std::string;
	quitButtonEntry.second["padding"] = (void*) new float;
	quitButtonEntry.second["border"] = (void*) new float;
	quitButtonEntry.second["softEdge"] = (void*) new float;
	quitButtonEntry.second["insideColor"] = (void*) new Vector4;
	quitButtonEntry.second["borderColor"] = (void*) new Vector4;
	quitButtonEntry.second["outsideColor"] = (void*) new Vector4;
	quitButtonZoneListener = new MouseZoneListener();
	quitButtonClickListener = new MouseButtonListener();

	menuTip1Entry.first = "label";
	menuTip1Entry.second["metrics"] = (void*) new UIMetrics;
	menuTip1Entry.second["fontSize"] = (void*) new float;
	menuTip1Entry.second["fontColor"] = (void*) new Vector4;
	menuTip1Entry.second["text"] = (void*) new std::string;

	menuTip2Entry.first = "label";
	menuTip2Entry.second["metrics"] = (void*) new UIMetrics;
	menuTip2Entry.second["fontSize"] = (void*) new float;
	menuTip2Entry.second["fontColor"] = (void*) new Vector4;
	menuTip2Entry.second["text"] = (void*) new std::string;

	menuTip3Entry.first = "label";
	menuTip3Entry.second["metrics"] = (void*) new UIMetrics;
	menuTip3Entry.second["fontSize"] = (void*) new float;
	menuTip3Entry.second["fontColor"] = (void*) new Vector4;
	menuTip3Entry.second["text"] = (void*) new std::string;

	std::vector<SDLKey> helpMenuKeys;
	helpMenuKeys.push_back(SDLK_UP);
	helpMenuKeys.push_back(SDLK_DOWN);
	helpMenuKeys.push_back(SDLK_ESCAPE);
	helpMenuKeys.push_back(SDLK_RETURN);
	helpMenuKeyListener = new KeyListener(helpMenuKeys);

	helpTitleEntry.first = "label";
	helpTitleEntry.second["metrics"] = (void*) new UIMetrics;
	helpTitleEntry.second["fontSize"] = (void*) new float;
	helpTitleEntry.second["fontColor"] = (void*) new Vector4;
	helpTitleEntry.second["text"] = (void*) new std::string;

	controlsTitleEntry.first = "label";
	controlsTitleEntry.second["metrics"] = (void*) new UIMetrics;
	controlsTitleEntry.second["fontSize"] = (void*) new float;
	controlsTitleEntry.second["fontColor"] = (void*) new Vector4;
	controlsTitleEntry.second["text"] = (void*) new std::string;

	controlsEntry.first = "label";
	controlsEntry.second["metrics"] = (void*) new UIMetrics;
	controlsEntry.second["fontSize"] = (void*) new float;
	controlsEntry.second["fontColor"] = (void*) new Vector4;
	controlsEntry.second["text"] = (void*) new std::string;

	instructionsTitleEntry.first = "label";
	instructionsTitleEntry.second["metrics"] = (void*) new UIMetrics;
	instructionsTitleEntry.second["fontSize"] = (void*) new float;
	instructionsTitleEntry.second["fontColor"] = (void*) new Vector4;
	instructionsTitleEntry.second["text"] = (void*) new std::string;

	instructionsEntry.first = "label";
	instructionsEntry.second["metrics"] = (void*) new UIMetrics;
	instructionsEntry.second["fontSize"] = (void*) new float;
	instructionsEntry.second["fontColor"] = (void*) new Vector4;
	instructionsEntry.second["wrap"] = (void*) new float;
	instructionsEntry.second["text"] = (void*) new std::string;

	aboutButtonEntry.first = "button";
	aboutButtonEntry.second["metrics"] = (void*) new UIMetrics;
	aboutButtonEntry.second["fontSize"] = (void*) new float;
	aboutButtonEntry.second["fontColor"] = (void*) new Vector4;
	aboutButtonEntry.second["text"] = (void*) new std::string;
	aboutButtonEntry.second["padding"] = (void*) new float;
	aboutButtonEntry.second["border"] = (void*) new float;
	aboutButtonEntry.second["softEdge"] = (void*) new float;
	aboutButtonEntry.second["insideColor"] = (void*) new Vector4;
	aboutButtonEntry.second["borderColor"] = (void*) new Vector4;
	aboutButtonEntry.second["outsideColor"] = (void*) new Vector4;
	aboutButtonZoneListener = new MouseZoneListener();
	aboutButtonClickListener = new MouseButtonListener();

	backButtonEntry.first = "button";
	backButtonEntry.second["metrics"] = (void*) new UIMetrics;
	backButtonEntry.second["fontSize"] = (void*) new float;
	backButtonEntry.second["fontColor"] = (void*) new Vector4;
	backButtonEntry.second["text"] = (void*) new std::string;
	backButtonEntry.second["padding"] = (void*) new float;
	backButtonEntry.second["border"] = (void*) new float;
	backButtonEntry.second["softEdge"] = (void*) new float;
	backButtonEntry.second["insideColor"] = (void*) new Vector4;
	backButtonEntry.second["borderColor"] = (void*) new Vector4;
	backButtonEntry.second["outsideColor"] = (void*) new Vector4;
	backButtonZoneListener = new MouseZoneListener();
	backButtonClickListener = new MouseButtonListener();

	std::vector<SDLKey> aboutMenuKeys;
	aboutMenuKeys.push_back(SDLK_UP);
	aboutMenuKeys.push_back(SDLK_DOWN);
	aboutMenuKeys.push_back(SDLK_ESCAPE);
	aboutMenuKeys.push_back(SDLK_RETURN);
	aboutMenuKeyListener = new KeyListener(aboutMenuKeys);

	aboutTitleEntry.first = "label";
	aboutTitleEntry.second["metrics"] = (void*) new UIMetrics;
	aboutTitleEntry.second["fontSize"] = (void*) new float;
	aboutTitleEntry.second["fontColor"] = (void*) new Vector4;
	aboutTitleEntry.second["text"] = (void*) new std::string;

	versionTitleEntry.first = "label";
	versionTitleEntry.second["metrics"] = (void*) new UIMetrics;
	versionTitleEntry.second["fontSize"] = (void*) new float;
	versionTitleEntry.second["fontColor"] = (void*) new Vector4;
	versionTitleEntry.second["text"] = (void*) new std::string;

	versionEntry.first = "label";
	versionEntry.second["metrics"] = (void*) new UIMetrics;
	versionEntry.second["fontSize"] = (void*) new float;
	versionEntry.second["fontColor"] = (void*) new Vector4;
	versionEntry.second["wrap"] = (void*) new float;
	versionEntry.second["text"] = (void*) new std::string;

	creditsTitleEntry.first = "label";
	creditsTitleEntry.second["metrics"] = (void*) new UIMetrics;
	creditsTitleEntry.second["fontSize"] = (void*) new float;
	creditsTitleEntry.second["fontColor"] = (void*) new Vector4;
	creditsTitleEntry.second["text"] = (void*) new std::string;

	creditsEntry.first = "label";
	creditsEntry.second["metrics"] = (void*) new UIMetrics;
	creditsEntry.second["fontSize"] = (void*) new float;
	creditsEntry.second["fontColor"] = (void*) new Vector4;
	creditsEntry.second["wrap"] = (void*) new float;
	creditsEntry.second["text"] = (void*) new std::string;
	
	std::vector<SDLKey> settingsMenuKeys;
	settingsMenuKeys.push_back(SDLK_UP);
	settingsMenuKeys.push_back(SDLK_DOWN);
	settingsMenuKeys.push_back(SDLK_RIGHT);
	settingsMenuKeys.push_back(SDLK_LEFT);
	settingsMenuKeys.push_back(SDLK_ESCAPE);
	settingsMenuKeys.push_back(SDLK_RETURN);
	settingsMenuKeyListener = new KeyListener(settingsMenuKeys);

	settingsMenuTitleEntry.first = "label";
	settingsMenuTitleEntry.second["metrics"] = (void*) new UIMetrics;
	settingsMenuTitleEntry.second["fontSize"] = (void*) new float;
	settingsMenuTitleEntry.second["fontColor"] = (void*) new Vector4;
	settingsMenuTitleEntry.second["wrap"] = (void*) new float;
	settingsMenuTitleEntry.second["text"] = (void*) new std::string;

	levelSettingEntry.first = "label";
	levelSettingEntry.second["metrics"] = (void*) new UIMetrics;
	levelSettingEntry.second["fontSize"] = (void*) new float;
	levelSettingEntry.second["fontColor"] = (void*) new Vector4;
	levelSettingEntry.second["wrap"] = (void*) new float;
	levelSettingEntry.second["text"] = (void*) new std::string;
	levelButtonZoneListener = new MouseZoneListener();
	levelButtonClickListener = new MouseButtonListener();

	musicSettingEntry.first = "label";
	musicSettingEntry.second["metrics"] = (void*) new UIMetrics;
	musicSettingEntry.second["fontSize"] = (void*) new float;
	musicSettingEntry.second["fontColor"] = (void*) new Vector4;
	musicSettingEntry.second["wrap"] = (void*) new float;
	musicSettingEntry.second["text"] = (void*) new std::string;
	musicButtonZoneListener = new MouseZoneListener();
	musicButtonClickListener = new MouseButtonListener();

	audioEffectsSettingEntry.first = "label";
	audioEffectsSettingEntry.second["metrics"] = (void*) new UIMetrics;
	audioEffectsSettingEntry.second["fontSize"] = (void*) new float;
	audioEffectsSettingEntry.second["fontColor"] = (void*) new Vector4;
	audioEffectsSettingEntry.second["wrap"] = (void*) new float;
	audioEffectsSettingEntry.second["text"] = (void*) new std::string;
	audioEffectsButtonZoneListener = new MouseZoneListener();
	audioEffectsButtonClickListener = new MouseButtonListener();

	fullscreenSettingEntry.first = "label";
	fullscreenSettingEntry.second["metrics"] = (void*) new UIMetrics;
	fullscreenSettingEntry.second["fontSize"] = (void*) new float;
	fullscreenSettingEntry.second["fontColor"] = (void*) new Vector4;
	fullscreenSettingEntry.second["wrap"] = (void*) new float;
	fullscreenSettingEntry.second["text"] = (void*) new std::string;
	fullscreenButtonZoneListener = new MouseZoneListener();
	fullscreenButtonClickListener = new MouseButtonListener();
	
	resetHighScoresEntry.first = "button";
	resetHighScoresEntry.second["metrics"] = (void*) new UIMetrics;
	resetHighScoresEntry.second["fontSize"] = (void*) new float;
	resetHighScoresEntry.second["fontColor"] = (void*) new Vector4;
	resetHighScoresEntry.second["text"] = (void*) new std::string;
	resetHighScoresEntry.second["padding"] = (void*) new float;
	resetHighScoresEntry.second["border"] = (void*) new float;
	resetHighScoresEntry.second["softEdge"] = (void*) new float;
	resetHighScoresEntry.second["insideColor"] = (void*) new Vector4;
	resetHighScoresEntry.second["borderColor"] = (void*) new Vector4;
	resetHighScoresEntry.second["outsideColor"] = (void*) new Vector4;
	resetHighScoresButtonZoneListener = new MouseZoneListener();
	resetHighScoresButtonClickListener = new MouseButtonListener();

	std::vector<SDLKey> highScoresMenuKeys;
	highScoresMenuKeys.push_back(SDLK_UP);
	highScoresMenuKeys.push_back(SDLK_DOWN);
	highScoresMenuKeys.push_back(SDLK_ESCAPE);
	highScoresMenuKeys.push_back(SDLK_RETURN);
	highScoresMenuKeyListener = new KeyListener(highScoresMenuKeys);

	deleteKeyListener = new KeyAbsoluteListener(SDLK_BACKSPACE);

	highScoresTitleEntry.first = "label";
	highScoresTitleEntry.second["metrics"] = (void*) new UIMetrics;
	highScoresTitleEntry.second["fontSize"] = (void*) new float;
	highScoresTitleEntry.second["fontColor"] = (void*) new Vector4;
	highScoresTitleEntry.second["text"] = (void*) new std::string;

	highScoresLabelEntry.first = "label";
	highScoresLabelEntry.second["metrics"] = (void*) new UIMetrics;
	highScoresLabelEntry.second["fontSize"] = (void*) new float;
	highScoresLabelEntry.second["fontColor"] = (void*) new Vector4;
	highScoresLabelEntry.second["text"] = (void*) new std::string;

	loadingEntry.first = "label";
	loadingEntry.second["metrics"] = (void*) new UIMetrics;
	loadingEntry.second["fontSize"] = (void*) new float;
	loadingEntry.second["fontColor"] = (void*) new Vector4;
	loadingEntry.second["text"] = (void*) new std::string;

	std::vector<SDLKey> playingKeys;
	playingKeys.push_back(SDLK_RETURN);
	playingKeys.push_back(SDLK_ESCAPE);
	playingKeys.push_back(SDLK_SPACE);
	playingKeys.push_back(SDLK_BACKQUOTE);
	playingKeys.push_back(SDLK_BACKSLASH);
	playingKeyListener = new KeyListener(playingKeys);

	fireEMPKeyListener = new KeyAbsoluteListener(SDLK_TAB);
	turretUpKeyListener = new KeyAbsoluteListener(SDLK_UP);
	turretDownKeyListener = new KeyAbsoluteListener(SDLK_DOWN);
	turretLeftKeyListener = new KeyAbsoluteListener(SDLK_LEFT);
	turretRightKeyListener = new KeyAbsoluteListener(SDLK_RIGHT);
	cameraAheadKeyListener = new KeyAbsoluteListener(SDLK_SPACE);

	cursorEntry.first = "cursor";
	cursorEntry.second["size"] = (void*) new float;
	cursorEntry.second["position"] = NULL;
	cursorEntry.second["border"] = (void*) new float;
	cursorEntry.second["softEdge"] = (void*) new float;
	cursorEntry.second["insideColor"] = (void*) new Vector4;
	cursorEntry.second["borderColor"] = (void*) new Vector4;
	cursorEntry.second["outsideColor"] = (void*) new Vector4;

	clockLabel.first = "label";
	clockLabel.second["metrics"] = (void*) new UIMetrics;
	clockLabel.second["fontSize"] = (void*) new float;
	clockLabel.second["fontColor"] = (void*) new Vector4;
	clockLabel.second["text"] = (void*) new std::string;

	scoreLabel.first = "label";
	scoreLabel.second["metrics"] = (void*) new UIMetrics;
	scoreLabel.second["fontSize"] = (void*) new float;
	scoreLabel.second["fontColor"] = (void*) new Vector4;
	scoreLabel.second["text"] = (void*) new std::string;

	playingTipEntry.first = "label";
	playingTipEntry.second["metrics"] = (void*) new UIMetrics;
	playingTipEntry.second["fontSize"] = (void*) new float;
	playingTipEntry.second["fontColor"] = (void*) new Vector4;
	playingTipEntry.second["text"] = (void*) new std::string;

	controlBoxEntry.first = "controlBox";
	controlBoxEntry.second["drawCorners"] = (void*) new bool;

	gaugePanelEntry.first = "gaugePanel";
	gaugePanelEntry.second["metrics"] = (void*) new UIMetrics;
	gaugePanelEntry.second["padding"] = (void*) new Vector2;
	gaugePanelEntry.second["border"] = (void*) new float;
	gaugePanelEntry.second["softEdge"] = (void*) new float;
	gaugePanelEntry.second["insideColor"] = (void*) new Vector4;
	gaugePanelEntry.second["borderColor"] = (void*) new Vector4;
	gaugePanelEntry.second["outsideColor"] = (void*) new Vector4;
	gaugePanelEntry.second["elements"] = (void*) new size_t;
	gaugePanelEntry.second["textureNames"] = (void*) new std::vector<std::string>;
	((std::vector<std::string>*) gaugePanelEntry.second["textureNames"])->push_back("");
	((std::vector<std::string>*) gaugePanelEntry.second["textureNames"])->push_back("");
	((std::vector<std::string>*) gaugePanelEntry.second["textureNames"])->push_back("");
	gaugePanelEntry.second["progressBarSize"] = (void*) new Vector2;
	gaugePanelEntry.second["progressions"] = (void*) new std::vector<float>;
	((std::vector<float>*) gaugePanelEntry.second["progressions"])->push_back(0.0f);
	((std::vector<float>*) gaugePanelEntry.second["progressions"])->push_back(0.0f);
	((std::vector<float>*) gaugePanelEntry.second["progressions"])->push_back(0.0f);
	gaugePanelEntry.second["backgroundColorTop"] = (void*) new Vector4;
	gaugePanelEntry.second["backgroundColorBottom"] = (void*) new Vector4;
	gaugePanelEntry.second["progressBarColorsTop"] = (void*) new std::vector<Vector4>;
	((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsTop"])->push_back(Vector4());
	((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsTop"])->push_back(Vector4());
	((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsTop"])->push_back(Vector4());
	gaugePanelEntry.second["progressBarColorsBottom"] = (void*) new std::vector<Vector4>;
	((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsBottom"])->push_back(Vector4());
	((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsBottom"])->push_back(Vector4());
	((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsBottom"])->push_back(Vector4());

	radarEntry.first = "radar";
	radarEntry.second["metrics"] = (void*) new UIMetrics;
	radarEntry.second["padding"] = (void*) new Vector2;
	radarEntry.second["border"] = (void*) new float;
	radarEntry.second["softEdge"] = (void*) new float;
	radarEntry.second["insideColor"] = (void*) new Vector4;
	radarEntry.second["borderColor"] = (void*) new Vector4;
	radarEntry.second["outsideColor"] = (void*) new Vector4;

	skyEntry.first = "skyRenderer";

	waterEntry.first = "waterRenderer";

	terrainEntry.first = "terrainRenderer";

	shipEntry.first = "shipRenderer";

	towerEntry.first = "towerRenderer";

	missileEntry.first = "missileRenderer";

	shellEntry.first = "shellRenderer";

	std::vector<SDLKey> introKeys;
	introKeys.push_back(SDLK_SPACE);
	introKeys.push_back(SDLK_ESCAPE);
	introKeyListener = new KeyListener(introKeys);

	introMouseButtonListener = new MouseButtonListener();

	introHintEntry.first = "label";
	introHintEntry.second["metrics"] = (void*) new UIMetrics;
	introHintEntry.second["fontSize"] = (void*) new float;
	introHintEntry.second["fontColor"] = (void*) new Vector4;
	introHintEntry.second["text"] = (void*) new std::string;

	std::vector<SDLKey> pausedKeys;
	pausedKeys.push_back(SDLK_UP);
	pausedKeys.push_back(SDLK_DOWN);
	pausedKeys.push_back(SDLK_RETURN);
	pausedKeys.push_back(SDLK_ESCAPE);
	pausedMenuKeyListener = new KeyListener(pausedKeys);

	grayOutEntry.first = "grayOut";
	grayOutEntry.second["color"] = (void*) new Vector4;

	pausedEntry.first = "label";
	pausedEntry.second["metrics"] = (void*) new UIMetrics;
	pausedEntry.second["fontSize"] = (void*) new float;
	pausedEntry.second["fontColor"] = (void*) new Vector4;
	pausedEntry.second["text"] = (void*) new std::string;

	pausedMenuSpacerMetrics = new UIMetrics;

	resumeButtonEntry.first = "button";
	resumeButtonEntry.second["metrics"] = (void*) new UIMetrics;
	resumeButtonEntry.second["fontSize"] = (void*) new float;
	resumeButtonEntry.second["fontColor"] = (void*) new Vector4;
	resumeButtonEntry.second["text"] = (void*) new std::string;
	resumeButtonEntry.second["padding"] = (void*) new float;
	resumeButtonEntry.second["border"] = (void*) new float;
	resumeButtonEntry.second["softEdge"] = (void*) new float;
	resumeButtonEntry.second["insideColor"] = (void*) new Vector4;
	resumeButtonEntry.second["borderColor"] = (void*) new Vector4;
	resumeButtonEntry.second["outsideColor"] = (void*) new Vector4;
	resumeButtonZoneListener = new MouseZoneListener();
	resumeButtonClickListener = new MouseButtonListener();

	endGameButtonEntry.first = "button";
	endGameButtonEntry.second["metrics"] = (void*) new UIMetrics;
	endGameButtonEntry.second["fontSize"] = (void*) new float;
	endGameButtonEntry.second["fontColor"] = (void*) new Vector4;
	endGameButtonEntry.second["text"] = (void*) new std::string;
	endGameButtonEntry.second["padding"] = (void*) new float;
	endGameButtonEntry.second["border"] = (void*) new float;
	endGameButtonEntry.second["softEdge"] = (void*) new float;
	endGameButtonEntry.second["insideColor"] = (void*) new Vector4;
	endGameButtonEntry.second["borderColor"] = (void*) new Vector4;
	endGameButtonEntry.second["outsideColor"] = (void*) new Vector4;
	endGameButtonZoneListener = new MouseZoneListener();
	endGameButtonClickListener = new MouseButtonListener();

	pausedMenuTipEntry.first = "label";
	pausedMenuTipEntry.second["metrics"] = (void*) new UIMetrics;
	pausedMenuTipEntry.second["fontSize"] = (void*) new float;
	pausedMenuTipEntry.second["fontColor"] = (void*) new Vector4;
	pausedMenuTipEntry.second["text"] = (void*) new std::string;

	std::vector<SDLKey> gameOverKeys;
	gameOverKeys.push_back(SDLK_UP);
	gameOverKeys.push_back(SDLK_DOWN);
	gameOverKeys.push_back(SDLK_RETURN);
	gameOverKeys.push_back(SDLK_ESCAPE);
	gameOverKeyListener = new KeyListener(gameOverKeys);

	gameOverTitleEntry.first = "label";
	gameOverTitleEntry.second["metrics"] = (void*) new UIMetrics;
	gameOverTitleEntry.second["fontSize"] = (void*) new float;
	gameOverTitleEntry.second["fontColor"] = (void*) new Vector4;
	gameOverTitleEntry.second["text"] = (void*) new std::string;

	yourScoreTitleEntry.first = "label";
	yourScoreTitleEntry.second["metrics"] = (void*) new UIMetrics;
	yourScoreTitleEntry.second["fontSize"] = (void*) new float;
	yourScoreTitleEntry.second["fontColor"] = (void*) new Vector4;
	yourScoreTitleEntry.second["text"] = (void*) new std::string;

	yourScoreEntry.first = "label";
	yourScoreEntry.second["metrics"] = (void*) new UIMetrics;
	yourScoreEntry.second["fontSize"] = (void*) new float;
	yourScoreEntry.second["fontColor"] = (void*) new Vector4;
	yourScoreEntry.second["text"] = (void*) new std::string;

	highScoresSectionEntry.first = "label";
	highScoresSectionEntry.second["metrics"] = (void*) new UIMetrics;
	highScoresSectionEntry.second["fontSize"] = (void*) new float;
	highScoresSectionEntry.second["fontColor"] = (void*) new Vector4;
	highScoresSectionEntry.second["text"] = (void*) new std::string;

	newHighScoreLabel.first = "label";
	newHighScoreLabel.second["metrics"] = (void*) new UIMetrics;
	newHighScoreLabel.second["fontSize"] = (void*) new float;
	newHighScoreLabel.second["fontColor"] = (void*) new Vector4;
	newHighScoreLabel.second["text"] = (void*) new std::string;

	newHighScoreContainer.first = "container";
	newHighScoreContainer.second["metrics"] = (void*) new UIMetrics;
	newHighScoreContainer.second["padding"] = (void*) new float;
	newHighScoreContainer.second["border"] = (void*) new float;
	newHighScoreContainer.second["softEdge"] = (void*) new float;
	newHighScoreContainer.second["insideColor"] = (void*) new Vector4;
	newHighScoreContainer.second["borderColor"] = (void*) new Vector4;
	newHighScoreContainer.second["outsideColor"] = (void*) new Vector4;

	newHighScoreNameLabel.first = "label";
	newHighScoreNameLabel.second["metrics"] = (void*) new UIMetrics;
	newHighScoreNameLabel.second["fontSize"] = (void*) new float;
	newHighScoreNameLabel.second["fontColor"] = (void*) new Vector4;
	newHighScoreNameLabel.second["text"] = (void*) new std::string;

	newHighScoreNameField.first = "field";
	newHighScoreNameField.second["metrics"] = (void*) new UIMetrics;
	newHighScoreNameField.second["fontSize"] = (void*) new float;
	newHighScoreNameField.second["fontColor"] = (void*) new Vector4;
	newHighScoreNameField.second["boxColor"] = (void*) new Vector4;
	newHighScoreNameField.second["text"] = (void*) new std::string;

	gameOverContinueButton.first = "button";
	gameOverContinueButton.second["metrics"] = (void*) new UIMetrics;
	gameOverContinueButton.second["fontSize"] = (void*) new float;
	gameOverContinueButton.second["fontColor"] = (void*) new Vector4;
	gameOverContinueButton.second["text"] = (void*) new std::string;
	gameOverContinueButton.second["padding"] = (void*) new float;
	gameOverContinueButton.second["border"] = (void*) new float;
	gameOverContinueButton.second["softEdge"] = (void*) new float;
	gameOverContinueButton.second["insideColor"] = (void*) new Vector4;
	gameOverContinueButton.second["borderColor"] = (void*) new Vector4;
	gameOverContinueButton.second["outsideColor"] = (void*) new Vector4;
	gameOverContinueButtonZoneListener = new MouseZoneListener();
	gameOverContinueButtonClickListener = new MouseButtonListener();

//FIXME reorder all those according to header (well, reorder header too if necessary)

fpsEntry.first = "label";
fpsEntry.second["metrics"] = (void*) new UIMetrics;
((UIMetrics*) fpsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
((UIMetrics*) fpsEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_RIGHT;
fpsEntry.second["fontSize"] = (void*) new float;
*((float*) fpsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
fpsEntry.second["fontColor"] = (void*) new Vector4;
*((Vector4*) fpsEntry.second["fontColor"]) = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
fpsEntry.second["text"] = (void*) new std::string;
*((std::string*) fpsEntry.second["text"]) = "FPS: 0";

	// set global variable (we only exist once, right?)
	gameLogic = this;

	// build the initial draw stack
	currentScheme = SCHEME_MAINMENU;
	activeMenuSelection = &playButtonEntry;
	reScheme();

	// start audio
	SDL_LockAudio();
	gameAudio->setBackgroundMusic("menuSong");
	SDL_UnlockAudio();

	// draw the initial frame
	gameGraphics->execute(true);

	// clear the motion listener
	inputHandler->execute();
	mouseMotionListener->wasMoved();

	// zero the lastUpdate time
	lastUpdate = 0;
}

GameLogic::~GameLogic() {
// mega fixme here
}

void GameLogic::reScheme() {
	// always clear the current stacks
	drawStack.clear();
	uiLayoutAuthority->metrics.clear();
	inputHandler->keyboard->clearListeners();
	inputHandler->mouse->clearListeners();

	// call the appropriate scheme function and do any associated logic
	switch (currentScheme) {
	case SCHEME_MAINMENU:
		Schemes::mainMenuScheme();
		break;

	case SCHEME_LOADING:
		Schemes::loadingScheme();
		break;

	case SCHEME_INTRO:
		Schemes::introScheme();
		break;

	case SCHEME_PLAYING:
		Schemes::playingScheme();
		break;

	case SCHEME_PAUSED:
		Schemes::pausedScheme();
		break;

	case SCHEME_GAMEOVER:
		Schemes::gameOverScheme();
		break;

	case SCHEME_SETTINGS:
		Schemes::settingsScheme();
		break;
		
	case SCHEME_HIGHSCORES:
		Schemes::highScoresScheme();
		break;

	case SCHEME_HELP:
		Schemes::helpScheme();
		break;

	case SCHEME_ABOUT:
		Schemes::aboutScheme();
		break;
	}

std::stringstream ss; ss << "FPS: " << gameGraphics->runRate;
*((std::string*) fpsEntry.second["text"]) = ss.str().c_str();
((UIMetrics*) fpsEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(fpsEntry.second);
uiLayoutAuthority->metrics.push_back((UIMetrics*) fpsEntry.second["metrics"]);
drawStack.push_back(fpsEntry);
uiLayoutAuthority->rearrange();
}

unsigned int GameLogic::execute(bool unScheduled) {
	bool needRedraw = false;

static unsigned int lastFPSUpdate = 0;
if(lastFPSUpdate + 1000 < platform->getExecMills()) {
reScheme();
needRedraw = true;
lastFPSUpdate = platform->getExecMills();
}
	// get a delta time for schemes that need it
	float deltaTime = 0.0f;
	if(gameState != NULL && ! gameState->isPaused && currentScheme == SCHEME_PLAYING) {
		unsigned int currentGameTime = gameState->getGameMills();
		deltaTime = (float) (currentGameTime - lastUpdate) / 1000.0f;
		lastUpdate = currentGameTime;
	}

	// universal logic
	if(quitKeyListener->popKey() != SDLK_UNKNOWN )
		keepDominicusAlive = false;
	if(fullScreenKeyListener->popKey() != SDLK_UNKNOWN) {
		bool fullScreenGraphics = ! gameGraphics->fullScreen;

		bool isInMainLoopModules = (mainLoopModules.find(gameGraphics) != mainLoopModules.end());
		if(isInMainLoopModules)
			mainLoopModules.erase(mainLoopModules.find(gameGraphics));

		Camera* currentCamera = gameGraphics->currentCamera;

		delete(gameGraphics);

		gameGraphics = new GameGraphics(fullScreenGraphics);

		gameGraphics->currentCamera = currentCamera;

		inputHandler->execute();
		mouseMotionListener->wasMoved();

		if(isInMainLoopModules)
			mainLoopModules[gameGraphics] = 0;

		if(currentScheme == SCHEME_PLAYING || currentScheme == SCHEME_INTRO || currentScheme == SCHEME_PAUSED) {
			((TerrainRenderer*) gameGraphics->drawers["terrainRenderer"])->reloadGraphics();
			((DrawRadar*) gameGraphics->drawers["radar"])->reloadGraphics();
		}

		delete(uiLayoutAuthority);
		uiLayoutAuthority = new UILayoutAuthority(
				Vector2(gameSystem->getFloat("hudElementMargin") / (float) gameGraphics->resolutionX,
						gameSystem->getFloat("hudElementMargin") / (float) gameGraphics->resolutionY)
			);

		reScheme();
		needRedraw = true;
	}

	// scheme-specific logic
	if(currentScheme == SCHEME_MAINMENU) {
		// button highlight
		if(mouseMotionListener->wasMoved()) {
			if(playButtonZoneListener->isEntered) {
				if(activeMenuSelection != &playButtonEntry) {
					activeMenuSelection = &playButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(settingsButtonZoneListener->isEntered) {
				if(activeMenuSelection != &settingsButtonEntry) {
					activeMenuSelection = &settingsButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(helpButtonZoneListener->isEntered) {
				if(activeMenuSelection != &helpButtonEntry) {
					activeMenuSelection = &helpButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(highScoresButtonZoneListener->isEntered) {
				if(activeMenuSelection != &highScoresButtonEntry) {
					activeMenuSelection = &highScoresButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(quitButtonZoneListener->isEntered) {
				if(activeMenuSelection != &quitButtonEntry) {
					activeMenuSelection = &quitButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				reScheme();
				needRedraw = true;
			}
		}

		// button clicks
		if(playButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_LOADING;
			reScheme();
			gameGraphics->execute(true);

			gameState = new GameState();
			mainLoopModules[gameState] = 0;

			mainLoopModules[gameGraphics] = 0;
			((TerrainRenderer*) gameGraphics->drawers["terrainRenderer"])->reloadGraphics();
			((DrawRadar*) gameGraphics->drawers["radar"])->reloadGraphics();
			gameGraphics->currentCamera = &introCamera;

			currentScheme = SCHEME_INTRO;
			activeMenuSelection = NULL;
			reScheme();
			SDL_LockAudio();
			gameAudio->setBackgroundMusic("playingSong");
			SDL_UnlockAudio();
		} else if(settingsButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_SETTINGS;
			activeMenuSelection = &levelSettingEntry;
			reScheme();
			needRedraw = true;
		} else if(helpButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_HELP;
			activeMenuSelection = &backButtonEntry;
			reScheme();
			needRedraw = true;
		} else if(highScoresButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_HIGHSCORES;
			activeMenuSelection = &backButtonEntry;
			reScheme();
			needRedraw = true;
		} else if(quitButtonClickListener->wasClicked()) {
			keepDominicusAlive = false;
		}

		// key hits
		for(SDLKey key = mainMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = mainMenuKeyListener->popKey()) {
			if(key == SDLK_UP) {
				if(activeMenuSelection == NULL || activeMenuSelection == &playButtonEntry) {
					activeMenuSelection = &quitButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &settingsButtonEntry) {
					activeMenuSelection = &playButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &highScoresButtonEntry) {
					activeMenuSelection = &settingsButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &helpButtonEntry) {
					activeMenuSelection = &highScoresButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &quitButtonEntry) {
					activeMenuSelection = &helpButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(key == SDLK_DOWN) {
				if(activeMenuSelection == NULL || activeMenuSelection == &quitButtonEntry) {
					activeMenuSelection = &playButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &playButtonEntry) {
					activeMenuSelection = &settingsButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &settingsButtonEntry) {
					activeMenuSelection = &highScoresButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &highScoresButtonEntry) {
					activeMenuSelection = &helpButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &helpButtonEntry) {
					activeMenuSelection = &quitButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(key == SDLK_RETURN) {
				if(activeMenuSelection == &playButtonEntry) {
					currentScheme = SCHEME_LOADING;
					reScheme();
					gameGraphics->execute(true);

					gameState = new GameState();
					mainLoopModules[gameState] = 0;

					mainLoopModules[gameGraphics] = 0;
					((TerrainRenderer*) gameGraphics->drawers["terrainRenderer"])->reloadGraphics();
					((DrawRadar*) gameGraphics->drawers["radar"])->reloadGraphics();
					gameGraphics->currentCamera = &introCamera;

					currentScheme = SCHEME_INTRO;
					activeMenuSelection = NULL;
					reScheme();

					SDL_LockAudio();
					gameAudio->setBackgroundMusic("playingSong");
					SDL_UnlockAudio();
				} else if(activeMenuSelection == &settingsButtonEntry) {
					currentScheme = SCHEME_SETTINGS;
					activeMenuSelection = &levelSettingEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &helpButtonEntry) {
					currentScheme = SCHEME_HELP;
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &highScoresButtonEntry) {
					currentScheme = SCHEME_HIGHSCORES;
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &quitButtonEntry) {
					keepDominicusAlive = false;
				}
			}
		}
	} else if(currentScheme == SCHEME_INTRO) {
		// button clicks
		if(introMouseButtonListener->wasClicked()) {
			gameState->bumpStart();
			fireEMPKeyListener->isDown = false;
			currentScheme = SCHEME_PLAYING;
			reScheme();
			SDL_ShowCursor(0);
			SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
			inputHandler->execute();
			mouseMotionListener->wasMoved();
			gameGraphics->currentCamera = &towerCamera;
		}

		// key hits
		for(SDLKey key = introKeyListener->popKey(); key != SDLK_UNKNOWN; key = introKeyListener->popKey()) {
			if(key == SDLK_SPACE) {
				gameState->bumpStart();
				fireEMPKeyListener->isDown = false;
				currentScheme = SCHEME_PLAYING;
				reScheme();
				SDL_ShowCursor(0);
				SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
				inputHandler->execute();
				mouseMotionListener->wasMoved();
				gameGraphics->currentCamera = &towerCamera;
			} else if(key == SDLK_ESCAPE) {
				gameState->pause();

				mainLoopModules.erase(mainLoopModules.find(gameGraphics));

				currentScheme = SCHEME_PAUSED;
				activeMenuSelection = &resumeButtonEntry;
				reScheme();
				SDL_ShowCursor(1);

				needRedraw = true;
			}
		}

		// expiration of intro time
		if((float) gameState->lastUpdateGameTime / 1000.0f > gameSystem->getFloat("stateShipEntryTime")) {
			fireEMPKeyListener->isDown = false;
			currentScheme = SCHEME_PLAYING;
			reScheme();
			SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
			inputHandler->execute();
			mouseMotionListener->wasMoved();
			SDL_ShowCursor(0);
			gameGraphics->currentCamera = &towerCamera;
		}
	} else if(currentScheme == SCHEME_PLAYING) {
		// see if we're dead
		if(gameState->fortress.health == 0.0f) {
			gameState->pause();

			mainLoopModules.erase(mainLoopModules.find(gameGraphics));

			if(gameSystem->highScores.size() > 0)
				playerName = gameSystem->highScores[0].second;
			else
				playerName = "";

			currentScheme = SCHEME_GAMEOVER;
			activeMenuSelection = &gameOverContinueButton;
			inputHandler->keyboard->listenUnicode = true;
			inputHandler->keyboard->unicodeChars = "";

			reScheme();
			SDL_ShowCursor(1);

			needRedraw = true;

			SDL_LockAudio();
			gameAudio->setBackgroundMusic("menuSong");
			SDL_UnlockAudio();
		}

		// check clock
/*
		time_t rawTime;
		struct tm* timeInfo;
		time(&rawTime);
		timeInfo = localtime(&rawTime);
		char timeString[6];
		strftime(timeString, 6, "%H:%M", timeInfo);

		if(*((std::string*) clockLabel.second["text"]) != timeString) {
			reScheme();
			needRedraw = true;
		}
*/

		// check score
		if(atoi(((std::string*) scoreLabel.second["text"])->c_str()) != gameState->score)
			reScheme();

		// check gauges
		if(
				(*((std::vector<float>*) gaugePanelEntry.second["progressions"]))[0] != gameState->fortress.health ||
				(*((std::vector<float>*) gaugePanelEntry.second["progressions"]))[1] != gameState->fortress.ammunition ||
				(*((std::vector<float>*) gaugePanelEntry.second["progressions"]))[2] != gameState->fortress.shock
			) {
			reScheme();
		}

		// mouse motion
		if(mouseMotionListener->wasMoved()) {
			if(! mouseActive) {
				SDL_WarpMouse(
						gameGraphics->resolutionX / 2,
						gameGraphics->resolutionY / 2
					);

				inputHandler->execute();

				mouseActive = true;
				reScheme();
			}

			// constrain to control box
			if(inputHandler->mouse->position.x > gameSystem->getFloat("hudControlBoxSize") / 100.0f / gameGraphics->aspectRatio) {
				SDL_WarpMouse(
						(gameSystem->getFloat("hudControlBoxSize") / 100.0f / gameGraphics->aspectRatio + 1.0f) / 2.0f * gameGraphics->resolutionX,
						(-inputHandler->mouse->position.y + 1.0f) / 2.0f * gameGraphics->resolutionY
					);
				inputHandler->execute();
			}
			if(inputHandler->mouse->position.x < -gameSystem->getFloat("hudControlBoxSize") / 100.0f / gameGraphics->aspectRatio) {
				SDL_WarpMouse(
						(-gameSystem->getFloat("hudControlBoxSize") / 100.0f / gameGraphics->aspectRatio + 1.0f) / 2.0f * gameGraphics->resolutionX,
						(-inputHandler->mouse->position.y + 1.0f) / 2.0f * gameGraphics->resolutionY
					);
				inputHandler->execute();
			}
			if(inputHandler->mouse->position.y > gameSystem->getFloat("hudControlBoxSize") / 100.0f) {
				SDL_WarpMouse(
						(inputHandler->mouse->position.x + 1.0f) / 2.0f * gameGraphics->resolutionX,
						(-gameSystem->getFloat("hudControlBoxSize") / 100.0f + 1.0f) / 2.0f * gameGraphics->resolutionY
					);
				inputHandler->execute();
			}
			if(inputHandler->mouse->position.y < -gameSystem->getFloat("hudControlBoxSize") / 100.0f) {
				SDL_WarpMouse(
						(inputHandler->mouse->position.x + 1.0f) / 2.0f * gameGraphics->resolutionX,
						(gameSystem->getFloat("hudControlBoxSize") / 100.0f + 1.0f) / 2.0f * gameGraphics->resolutionY
					);
				inputHandler->execute();
			}

			mouseMotionListener->wasMoved();
		}

		// do movement
		if(mouseActive) {
			Vector2 effectiveMousePosition = Vector2(
				inputHandler->mouse->position.x * gameGraphics->aspectRatio * 100.0f / gameSystem->getFloat("hudControlBoxSize"),
				inputHandler->mouse->position.y * 100.0f / gameSystem->getFloat("hudControlBoxSize")
				);
			float effectiveDeadAreaRadius = gameSystem->getFloat("hudControlBoxSpotSize") / gameSystem->getFloat("hudControlBoxSize");

			if(distance(Vector2(0.0f, 0.0f), effectiveMousePosition) > effectiveDeadAreaRadius) {
				gameState->fortress.addRotation(gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime * effectiveMousePosition.x);
				gameState->fortress.addTilt(gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime * effectiveMousePosition.y);
			}
		}

		// key hits
		for(SDLKey key = playingKeyListener->popKey(); key != SDLK_UNKNOWN; key = playingKeyListener->popKey()) {
			if(key == SDLK_SPACE) {
				gameState->fireShell();
			} else if(key == SDLK_RETURN) {
				mainLoopModules.erase(mainLoopModules.find(gameState));
				delete(gameState);
				gameState = new GameState();
				mainLoopModules[gameState] = 0;

				((TerrainRenderer*) gameGraphics->drawers["terrainRenderer"])->reloadGraphics();
				((DrawRadar*) gameGraphics->drawers["radar"])->reloadGraphics();
			} else if(key == SDLK_BACKSLASH) {
				if(gameGraphics->currentCamera == &towerCamera)
					gameGraphics->currentCamera = &orbitCamera;
				else if(gameGraphics->currentCamera == &orbitCamera)
					gameGraphics->currentCamera = &presentationCamera;
				else if(gameGraphics->currentCamera == &presentationCamera)
					gameGraphics->currentCamera = &roamingCamera;
				else if(gameGraphics->currentCamera == &roamingCamera)
					gameGraphics->currentCamera = &towerCamera;
			} else if(key == SDLK_ESCAPE) {
				gameState->pause();

				mainLoopModules.erase(mainLoopModules.find(gameGraphics));

				currentScheme = SCHEME_PAUSED;
				activeMenuSelection = &resumeButtonEntry;
				reScheme();
				SDL_ShowCursor(1);

				needRedraw = true;
			} else if(key == SDLK_BACKQUOTE) {
				if(gameState->isPaused)
					gameState->resume();
				else
					gameState->pause();
			}
		}

		// keys down
		gameState->shockIsCharging = fireEMPKeyListener->isDown;

		if(gameGraphics->currentCamera == &roamingCamera) {
			if(turretLeftKeyListener->isDown)
				roamingCamera.rotationX += gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime;
			if(turretRightKeyListener->isDown)
				roamingCamera.rotationX -= gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime;
			if(turretUpKeyListener->isDown)
				roamingCamera.rotationY += gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime;
			if(turretDownKeyListener->isDown)
				roamingCamera.rotationY -= gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime;
			if(cameraAheadKeyListener->isDown) {
				Matrix3 directionMatrix; directionMatrix.identity();
				rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(roamingCamera.rotationX), directionMatrix);
				rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(roamingCamera.rotationY), directionMatrix);
				directionMatrix.transpose();
				Vector3 movement(0.0f, 0.0f, 200.0f * deltaTime);
				roamingCamera.position = roamingCamera.position + movement * directionMatrix;
			}
		} else {
			if(turretLeftKeyListener->isDown) {
				gameState->fortress.addRotation(-gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime);
				mouseActive = false;
			}
			if(turretRightKeyListener->isDown) {
				gameState->fortress.addRotation(gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime);
				mouseActive = false;
			}
			if(turretUpKeyListener->isDown) {
				gameState->fortress.addTilt(gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime);
				mouseActive = false;
			}
			if(turretDownKeyListener->isDown) {
				gameState->fortress.addTilt(-gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime);
				mouseActive = false;
			}
		}

		if(! mouseActive) {
			Vector2 newKeyboardCursorPosition(0.0f, 0.0f);
			if(turretLeftKeyListener->isDown)
				newKeyboardCursorPosition.x -= 1.0f;
			if(turretRightKeyListener->isDown)
				newKeyboardCursorPosition.x += 1.0f;
			if(turretUpKeyListener->isDown)
				newKeyboardCursorPosition.y += 1.0f;
			if(turretDownKeyListener->isDown)
				newKeyboardCursorPosition.y -= 1.0f;

			newKeyboardCursorPosition.x *= gameSystem->getFloat("hudControlBoxSize") / 100.0f / gameGraphics->aspectRatio;
			newKeyboardCursorPosition.y *= gameSystem->getFloat("hudControlBoxSize") / 100.0f;

			if(! mouseActive && newKeyboardCursorPosition != keyboardCursorPosition) {
				keyboardCursorPosition = newKeyboardCursorPosition;
				reScheme();
			}
		}
	} else if(currentScheme == SCHEME_PAUSED) {
		// button highlight
		if(mouseMotionListener->wasMoved()) {
			if(resumeButtonZoneListener->isEntered) {
				if(activeMenuSelection != &resumeButtonEntry) {
					activeMenuSelection = &resumeButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(endGameButtonZoneListener->isEntered) {
				if(activeMenuSelection != &endGameButtonEntry) {
					activeMenuSelection = &endGameButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				reScheme();
				needRedraw = true;
			}
		}

		// button clicks
		if(resumeButtonClickListener->wasClicked()) {
			if(gameGraphics->currentCamera == &introCamera) {
				currentScheme = SCHEME_INTRO;
			} else {
				fireEMPKeyListener->isDown = false;
				currentScheme = SCHEME_PLAYING;
				SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
				SDL_ShowCursor(0);

				inputHandler->execute();
				mouseMotionListener->wasMoved();
			}

			activeMenuSelection = NULL;
			reScheme();

			mainLoopModules[gameGraphics] = 0;

			gameState->resume();
		} else if(endGameButtonClickListener->wasClicked()) {
				inputHandler->keyboard->listenUnicode = true;
				inputHandler->keyboard->unicodeChars = "";

				if(gameSystem->highScores.size() > 0)
					playerName = gameSystem->highScores[0].second;
				else
					playerName = "";

				currentScheme = SCHEME_GAMEOVER;
				activeMenuSelection = &gameOverContinueButton;
				reScheme();
				needRedraw = true;

				SDL_LockAudio();
				gameAudio->setBackgroundMusic("menuSong");
				SDL_UnlockAudio();
		}

		// key hits
		for(SDLKey key = pausedMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = pausedMenuKeyListener->popKey()) {
			if(key == SDLK_UP) {
				if(activeMenuSelection == NULL || activeMenuSelection == &resumeButtonEntry) {
					activeMenuSelection = &endGameButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &endGameButtonEntry) {
					activeMenuSelection = &resumeButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(key == SDLK_DOWN) {
				if(activeMenuSelection == NULL || activeMenuSelection == &endGameButtonEntry) {
					activeMenuSelection = &resumeButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &resumeButtonEntry) {
					activeMenuSelection = &endGameButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(key == SDLK_RETURN || key == SDLK_ESCAPE) {
				if(activeMenuSelection == &resumeButtonEntry || key == SDLK_ESCAPE) {
					if(gameGraphics->currentCamera == &introCamera) {
						currentScheme = SCHEME_INTRO;
					} else {
						fireEMPKeyListener->isDown = false;
						currentScheme = SCHEME_PLAYING;
						SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
						SDL_ShowCursor(0);

						inputHandler->execute();
						mouseMotionListener->wasMoved();
					}

					activeMenuSelection = NULL;
					reScheme();

					mainLoopModules[gameGraphics] = 0;

					gameState->resume();
				} else if(activeMenuSelection == &endGameButtonEntry) {
					inputHandler->keyboard->listenUnicode = true;
					inputHandler->keyboard->unicodeChars = "";

					if(gameSystem->highScores.size() > 0)
						playerName = gameSystem->highScores[0].second;
					else
						playerName = "";

					currentScheme = SCHEME_GAMEOVER;
					activeMenuSelection = &gameOverContinueButton;
					reScheme();
					needRedraw = true;

					SDL_LockAudio();
					gameAudio->setBackgroundMusic("menuSong");
					SDL_UnlockAudio();
				}
			} else if (key == SDLK_ESCAPE) {
				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				reScheme();
				needRedraw = true;
			}
		}
	} else if(currentScheme == SCHEME_GAMEOVER) {
		// typing
		if(inputHandler->keyboard->unicodeChars != "") {
			if(playerName.length() < gameSystem->getFloat("hudFieldWidth")) {
				for(size_t i = 0; i < inputHandler->keyboard->unicodeChars.length(); ++i) {
					if(strstr(gameSystem->getString("inputAllowedNameChars").c_str(), inputHandler->keyboard->unicodeChars.substr(i, 1).c_str()) != NULL) {
						playerName += inputHandler->keyboard->unicodeChars.substr(i, 1);

						reScheme();
						needRedraw = true;
					}
				}
			}

			inputHandler->keyboard->unicodeChars = "";
		}

		if(deleteKeyListener->isDown && playerName.length() > 0) {
			if(deleteKeyPressTime > platform->getExecMills()) {
				playerName.erase(playerName.length() - 1);
				deleteKeyPressTime = platform->getExecMills();

				reScheme();
				needRedraw = true;
			} else if(
					deleteKeyPressTime + (unsigned int) (gameSystem->getFloat("inputDeleteKeyRepeatWait") * 1000.0f) <
							platform->getExecMills() &&
					lastCharacterDeletionTime + (unsigned int) (gameSystem->getFloat("inputDeleteKeyRepeatRate") * 1000.0f) <
							platform->getExecMills()
				) {
				playerName.erase(playerName.length() - 1);
				lastCharacterDeletionTime = platform->getExecMills();

				reScheme();
				needRedraw = true;
			}
		} else {
			deleteKeyPressTime = -1;
			lastCharacterDeletionTime = 0;
		}

		// button highlight
		if(mouseMotionListener->wasMoved()) {
			if(gameOverContinueButtonZoneListener->isEntered) {
				if(activeMenuSelection != &gameOverContinueButton) {
					activeMenuSelection = &gameOverContinueButton;
					reScheme();
					needRedraw = true;
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				reScheme();
				needRedraw = true;
			}
		}

		// button clicks
		if(gameOverContinueButtonClickListener->wasClicked()) {
			if(gameState->score > 0 && (gameSystem->highScores.size() == 0 || gameState->score > gameSystem->highScores[0].first)) {
				if(playerName != "") {
					gameSystem->highScores.insert(gameSystem->highScores.begin(), std::make_pair(gameState->score, gameLogic->playerName));
					while(gameSystem->highScores.size() > (size_t) gameSystem->getFloat("gameMaximumHighScores"))
						gameSystem->highScores.erase(gameSystem->highScores.end());

					gameSystem->flushPreferences();
				}
			}

			mainLoopModules.erase(mainLoopModules.find(gameState));
			delete(gameState);
			gameState = NULL;

			currentScheme = SCHEME_MAINMENU;
			activeMenuSelection = &playButtonEntry;
			reScheme();
			needRedraw = true;
		}

		// key hits
		for(SDLKey key = gameOverKeyListener->popKey(); key != SDLK_UNKNOWN; key = gameOverKeyListener->popKey()) {
			if(key == SDLK_UP || key == SDLK_DOWN) {
				activeMenuSelection = &gameOverContinueButton;
				reScheme();
				needRedraw = true;
			} else if(key == SDLK_RETURN) {
				if(activeMenuSelection == &gameOverContinueButton) {
					if(gameSystem->highScores.size() == 0 || gameState->score > gameSystem->highScores[0].first) {
						if(playerName != "") {
							gameSystem->highScores.insert(gameSystem->highScores.begin(), std::make_pair(gameState->score, gameLogic->playerName));
							while(gameSystem->highScores.size() > (size_t) gameSystem->getFloat("gameMaximumHighScores"))
								gameSystem->highScores.erase(gameSystem->highScores.end());

							gameSystem->flushPreferences();
						}
					}

					mainLoopModules.erase(mainLoopModules.find(gameState));
					delete(gameState);
					gameState = NULL;

					currentScheme = SCHEME_MAINMENU;
					activeMenuSelection = &playButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(key == SDLK_ESCAPE) {
				mainLoopModules.erase(mainLoopModules.find(gameState));
				delete(gameState);
				gameState = NULL;

				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				reScheme();
				needRedraw = true;
			}
		}
	} else if(currentScheme == SCHEME_SETTINGS) {
		// button highlight
		if(mouseMotionListener->wasMoved()) {
			if(levelButtonZoneListener->isEntered) {
				if(activeMenuSelection != &levelSettingEntry) {
					activeMenuSelection = &levelSettingEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(musicButtonZoneListener->isEntered) {
				if(activeMenuSelection != &musicSettingEntry) {
					activeMenuSelection = &musicSettingEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(audioEffectsButtonZoneListener->isEntered) {
				if(activeMenuSelection != &audioEffectsSettingEntry) {
					activeMenuSelection = &audioEffectsSettingEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(fullscreenButtonZoneListener->isEntered) {
				if(activeMenuSelection != &fullscreenSettingEntry) {
					activeMenuSelection = &fullscreenSettingEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(backButtonZoneListener->isEntered) {
				if(activeMenuSelection != &backButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(resetHighScoresButtonZoneListener->isEntered) {
				if(activeMenuSelection != &resetHighScoresEntry) {
					activeMenuSelection = &resetHighScoresEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				reScheme();
				needRedraw = true;
			}
		}

		// button clicks
		if(levelButtonClickListener->wasClicked()) {
			if(gameSystem->getString("gameStartingLevel") == "Easy") {
				gameSystem->setStandard("gameStartingLevel", "Medium", "");
			} else if(gameSystem->getString("gameStartingLevel") == "Medium") {
				gameSystem->setStandard("gameStartingLevel", "Hard", "");
			} else if(gameSystem->getString("gameStartingLevel") == "Hard") {
				gameSystem->setStandard("gameStartingLevel", "Easy", "");
			}
			gameSystem->flushPreferences();
			
			reScheme();
			needRedraw = true;
		} else if(musicButtonClickListener->wasClicked()) {
			float value = gameSystem->getFloat("audioMusicVolume");
			value += 0.1f;
			if(value > 1.0f) value = 0.0f;
			SDL_LockAudio();
			gameSystem->setStandard("audioMusicVolume", value, "");
			SDL_UnlockAudio();
			gameSystem->flushPreferences();

			reScheme();
			needRedraw = true;
		} else if(audioEffectsButtonClickListener->wasClicked()) {
			float value = gameSystem->getFloat("audioEffectsVolume");
			value += 0.1f;
			if(value > 1.0f) value = 0.0f;
			gameSystem->setStandard("audioEffectsVolume", value, "");
			gameSystem->flushPreferences();

			reScheme();
			needRedraw = true;
		} else if(fullscreenButtonClickListener->wasClicked()) {
			gameSystem->setStandard("displayStartFullscreen", ! gameSystem->getBool("displayStartFullscreen"), "");
			gameSystem->flushPreferences();

			reScheme();
			needRedraw = true;
		} else if(backButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_MAINMENU;
			activeMenuSelection = NULL;
			reScheme();
			needRedraw = true;
		} else if(resetHighScoresButtonClickListener->wasClicked()) {
			gameSystem->highScores.clear();
			gameSystem->flushPreferences();
		}

		// key hits
		for(SDLKey key = settingsMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = settingsMenuKeyListener->popKey()) {
			if(key == SDLK_UP) {
				if(activeMenuSelection == NULL || activeMenuSelection == &levelSettingEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &backButtonEntry) {
					activeMenuSelection = &resetHighScoresEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &resetHighScoresEntry) {
					activeMenuSelection = &fullscreenSettingEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &fullscreenSettingEntry) {
					activeMenuSelection = &audioEffectsSettingEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &audioEffectsSettingEntry) {
					activeMenuSelection = &musicSettingEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &musicSettingEntry) {
					activeMenuSelection = &levelSettingEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(key == SDLK_DOWN) {
				if(activeMenuSelection == NULL || activeMenuSelection == &backButtonEntry) {
					activeMenuSelection = &levelSettingEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &levelSettingEntry) {
					activeMenuSelection = &musicSettingEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &musicSettingEntry) {
					activeMenuSelection = &audioEffectsSettingEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &audioEffectsSettingEntry) {
					activeMenuSelection = &fullscreenSettingEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &fullscreenSettingEntry) {
					activeMenuSelection = &resetHighScoresEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &resetHighScoresEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(key == SDLK_LEFT || key == SDLK_RIGHT) {
				if(activeMenuSelection == &levelSettingEntry) {
					if(gameSystem->getString("gameStartingLevel") == "Easy") {
						gameSystem->setStandard("gameStartingLevel", (key == SDLK_RIGHT ? "Medium" : "Hard"), "");
					} else if(gameSystem->getString("gameStartingLevel") == "Medium") {
						gameSystem->setStandard("gameStartingLevel", (key == SDLK_RIGHT ? "Hard" : "Easy"), "");
					} else if(gameSystem->getString("gameStartingLevel") == "Hard") {
						gameSystem->setStandard("gameStartingLevel", (key == SDLK_RIGHT ? "Easy" : "Medium"), "");
					}
					gameSystem->flushPreferences();
			
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &musicSettingEntry) {
					float value = gameSystem->getFloat("audioMusicVolume");
					value += (key == SDLK_RIGHT ? 0.1f : -0.1f);
					if(value > 1.0f) value = 0.0f;
					if(value < 0.0f) value = 1.0f;
					SDL_LockAudio();
					gameSystem->setStandard("audioMusicVolume", value, "");
					SDL_UnlockAudio();
					gameSystem->flushPreferences();

					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &audioEffectsSettingEntry) {
					float value = gameSystem->getFloat("audioEffectsVolume");
					value += (key == SDLK_RIGHT ? 0.1f : -0.1f);
					if(value > 1.0f) value = 0.0f;
					if(value < 0.0f) value = 1.0f;
					gameSystem->setStandard("audioEffectsVolume", value, "");
					gameSystem->flushPreferences();

					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &fullscreenSettingEntry) {
					gameSystem->setStandard("displayStartFullscreen", ! gameSystem->getBool("displayStartFullscreen"), "");
					gameSystem->flushPreferences();

					reScheme();
					needRedraw = true;
				}
			} else if(key == SDLK_RETURN) {
				if(activeMenuSelection == &resetHighScoresEntry) {
					gameSystem->highScores.clear();
					gameSystem->flushPreferences();
				} else if(activeMenuSelection == &backButtonEntry) {
					currentScheme = SCHEME_MAINMENU;
					activeMenuSelection = &playButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if (key == SDLK_ESCAPE) {
				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				reScheme();
				needRedraw = true;
			}
		}
	} else if(currentScheme == SCHEME_HIGHSCORES) {
		// button highlight
		if(mouseMotionListener->wasMoved()) {
			if(backButtonZoneListener->isEntered) {
				if(activeMenuSelection != &backButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				reScheme();
				needRedraw = true;
			}
		}

		// button clicks
		if(backButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_MAINMENU;
			activeMenuSelection = &playButtonEntry;
			reScheme();
			needRedraw = true;
		}

		// key hits
		for(SDLKey key = highScoresMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = highScoresMenuKeyListener->popKey()) {
			if((key == SDLK_UP || key == SDLK_DOWN) && activeMenuSelection != &backButtonEntry) {
				activeMenuSelection = &backButtonEntry;
				reScheme();
				needRedraw = true;
			} else if((key == SDLK_RETURN && activeMenuSelection == &backButtonEntry) || key == SDLK_ESCAPE) {
				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				reScheme();
				needRedraw = true;
			}
		}
	} else if(currentScheme == SCHEME_HELP) {
		// button highlight
		if(mouseMotionListener->wasMoved()) {
			if(aboutButtonZoneListener->isEntered) {
				if(activeMenuSelection != &aboutButtonEntry) {
					activeMenuSelection = &aboutButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(backButtonZoneListener->isEntered) {
				if(activeMenuSelection != &backButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				reScheme();
				needRedraw = true;
			}
		}

		// button clicks
		if(aboutButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_ABOUT;
			activeMenuSelection = &backButtonEntry;
			reScheme();
			needRedraw = true;
		} else if(backButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_MAINMENU;
			activeMenuSelection = &playButtonEntry;
			reScheme();
			needRedraw = true;
		}

		// key hits
		for(SDLKey key = helpMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = helpMenuKeyListener->popKey()) {
			if(key == SDLK_UP) {
				if(activeMenuSelection == NULL || activeMenuSelection == &aboutButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &backButtonEntry) {
					activeMenuSelection = &aboutButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(key == SDLK_DOWN) {
				if(activeMenuSelection == NULL || activeMenuSelection == &backButtonEntry) {
					activeMenuSelection = &aboutButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &aboutButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(key == SDLK_RETURN) {
				if(activeMenuSelection == &aboutButtonEntry) {
					currentScheme = SCHEME_ABOUT;
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
				} else if(activeMenuSelection == &backButtonEntry) {
					currentScheme = SCHEME_MAINMENU;
					activeMenuSelection = &playButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if (key == SDLK_ESCAPE) {
				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				reScheme();
				needRedraw = true;
			}
		}
	} else if(currentScheme == SCHEME_ABOUT) {
		// button highlight
		if(mouseMotionListener->wasMoved()) {
			if(backButtonZoneListener->isEntered) {
				if(activeMenuSelection != &backButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				reScheme();
				needRedraw = true;
			}
		}

		// button clicks
		if(backButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_HELP;
			activeMenuSelection = &backButtonEntry;
			reScheme();
			needRedraw = true;
		}

		// key hits
		for(SDLKey key = helpMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = helpMenuKeyListener->popKey()) {
			if((key == SDLK_UP || key == SDLK_DOWN) && activeMenuSelection != &backButtonEntry) {
				activeMenuSelection = &backButtonEntry;
				reScheme();
				needRedraw = true;
			} else if((key == SDLK_RETURN && activeMenuSelection == &backButtonEntry) || key == SDLK_ESCAPE) {
				currentScheme = SCHEME_HELP;
				activeMenuSelection = &backButtonEntry;
				reScheme();
				needRedraw = true;
			}
		}
	}

	// see if we need to redraw graphics, if we're still in the menus
	if(needRedraw && mainLoopModules.find(gameGraphics) == mainLoopModules.end())
		gameGraphics->execute(true);

	// track runcount
	trackRunCount();

	// calculate and return sleep time from superclass
	if(unScheduled)
		return 0;
	else
		return getSleepTime();
}
