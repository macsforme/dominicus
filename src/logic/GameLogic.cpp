// GameLogic.cpp
// Crucible Island

#include "logic/GameLogic.h"

GameLogic::GameLogic() :
		MainLoopMember((unsigned int) gameSystem->getFloat("logicUpdateFrequency")),
		mouseActive(false),
		keyboardCursorPosition(Vector2(0.0f, 0.0f)),
		playerName(""),
		deleteKeyPressTime(-1),
		lastCharacterDeletionTime(0),
		leftArrowPressTime(0),
		rightArrowPressTime(0),
		upArrowPressTime(0),
		downArrowPressTime(0),
		lastFPSUpdate(0) {
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

	mainMenuSpacer1Metrics = new UIMetrics;

	titleEntry.first = "label";
	titleEntry.second["metrics"] = (void*) new UIMetrics;
	titleEntry.second["fontSize"] = (void*) new float;
	titleEntry.second["fontColor"] = (void*) new Vector4;
	titleEntry.second["text"] = (void*) new std::string;

	logoEntry.first = "texture";
	logoEntry.second = ((DrawTexture*) drawingMaster->drawers["texture"])->instantiateArgList();
/*
	logoEntry.second["metrics"] = (void*) new UIMetrics;
	logoEntry.second["texture"] = (void*) new std::string;
*/

	mainMenuSpacer2Metrics = new UIMetrics;

	playButtonEntry.first = "button";
	playButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
/*
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
*/
	playButtonZoneListener = new MouseZoneListener();
	playButtonClickListener = new MouseButtonListener();

	settingsButtonEntry.first = "button";
	settingsButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
/*
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
*/
	settingsButtonZoneListener = new MouseZoneListener();
	settingsButtonClickListener = new MouseButtonListener();

	helpButtonEntry.first = "button";
	helpButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
/*
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
*/
	helpButtonZoneListener = new MouseZoneListener();
	helpButtonClickListener = new MouseButtonListener();

	highScoresButtonEntry.first = "button";
	highScoresButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
/*
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
*/
	highScoresButtonZoneListener = new MouseZoneListener();
	highScoresButtonClickListener = new MouseButtonListener();

	quitButtonEntry.first = "button";
	quitButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
/*
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
*/
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
	
	windowedScreenResolutionEntry.first = "label";
	windowedScreenResolutionEntry.second["metrics"] = (void*) new UIMetrics;
	windowedScreenResolutionEntry.second["fontSize"] = (void*) new float;
	windowedScreenResolutionEntry.second["fontColor"] = (void*) new Vector4;
	windowedScreenResolutionEntry.second["wrap"] = (void*) new float;
	windowedScreenResolutionEntry.second["text"] = (void*) new std::string;
	windowedScreenResolutionButtonZoneListener = new MouseZoneListener();
	windowedScreenResolutionButtonClickListener = new MouseButtonListener();

	framerateLimitingEntry.first = "label";
	framerateLimitingEntry.second["metrics"] = (void*) new UIMetrics;
	framerateLimitingEntry.second["fontSize"] = (void*) new float;
	framerateLimitingEntry.second["fontColor"] = (void*) new Vector4;
	framerateLimitingEntry.second["wrap"] = (void*) new float;
	framerateLimitingEntry.second["text"] = (void*) new std::string;
	framerateLimitingButtonZoneListener = new MouseZoneListener();
	framerateLimitingButtonClickListener = new MouseButtonListener();

	multisamplingLevelEntry.first = "label";
	multisamplingLevelEntry.second["metrics"] = (void*) new UIMetrics;
	multisamplingLevelEntry.second["fontSize"] = (void*) new float;
	multisamplingLevelEntry.second["fontColor"] = (void*) new Vector4;
	multisamplingLevelEntry.second["wrap"] = (void*) new float;
	multisamplingLevelEntry.second["text"] = (void*) new std::string;
	multisamplingButtonZoneListener = new MouseZoneListener();
	multisamplingButtonClickListener = new MouseButtonListener();

	terrainDetailEntry.first = "label";
	terrainDetailEntry.second["metrics"] = (void*) new UIMetrics;
	terrainDetailEntry.second["fontSize"] = (void*) new float;
	terrainDetailEntry.second["fontColor"] = (void*) new Vector4;
	terrainDetailEntry.second["wrap"] = (void*) new float;
	terrainDetailEntry.second["text"] = (void*) new std::string;
	terrainDetailButtonZoneListener = new MouseZoneListener();
	terrainDetailButtonClickListener = new MouseButtonListener();

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
	playingKeys.push_back(SDLK_TAB);
	playingKeys.push_back(SDLK_BACKQUOTE);
	playingKeys.push_back(SDLK_BACKSLASH);
	playingKeys.push_back(SDLK_LSHIFT);
	playingKeys.push_back(SDLK_RSHIFT);
	playingKeyListener = new KeyListener(playingKeys);

	turretUpKeyListener = new KeyAbsoluteListener(SDLK_UP);
	turretDownKeyListener = new KeyAbsoluteListener(SDLK_DOWN);
	turretLeftKeyListener = new KeyAbsoluteListener(SDLK_LEFT);
	turretRightKeyListener = new KeyAbsoluteListener(SDLK_RIGHT);
	cameraAheadKeyListener = new KeyAbsoluteListener(SDLK_SPACE);

	primaryFireClickListener1 = new MouseButtonListener();
	primaryFireClickListener2 = new MouseButtonListener(SDL_BUTTON_WHEELUP);
	primaryFireClickListener3 = new MouseButtonListener(SDL_BUTTON_WHEELDOWN);
	secondaryFireClickListener = new MouseButtonListener(SDL_BUTTON_RIGHT);
	binocularsClickListener = new MouseButtonListener(SDL_BUTTON_MIDDLE);

	controlSpotEntry.first = "circle";
	controlSpotEntry.second["size"] = (void*) new Vector2;
	controlSpotEntry.second["position"] = (void*) new Vector2;
	controlSpotEntry.second["border"] = (void*) new float;
	controlSpotEntry.second["softEdge"] = (void*) new float;
	controlSpotEntry.second["insideColor"] = (void*) new Vector4;
	controlSpotEntry.second["borderColor"] = (void*) new Vector4;
	controlSpotEntry.second["outsideColor"] = (void*) new Vector4;

	cursorEntry.first = "circle";
	cursorEntry.second["size"] = (void*) new Vector2;
	cursorEntry.second["position"] = NULL;
	cursorEntry.second["border"] = (void*) new float;
	cursorEntry.second["softEdge"] = (void*) new float;
	cursorEntry.second["insideColor"] = (void*) new Vector4;
	cursorEntry.second["borderColor"] = (void*) new Vector4;
	cursorEntry.second["outsideColor"] = (void*) new Vector4;

	missileIndicators.first = "missileIndicators";
	missileIndicators.second = ((DrawMissileIndicators*) drawingMaster->drawers["missileIndicators"])->instantiateArgList();

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

	gaugePanelEntry.first = "gaugePanel";
	gaugePanelEntry.second = ((DrawGaugePanel*) drawingMaster->drawers["gaugePanel"])->instantiateArgList();
	((std::vector<std::string>*) gaugePanelEntry.second["textureNames"])->push_back("");
	((std::vector<std::string>*) gaugePanelEntry.second["textureNames"])->push_back("");
	((std::vector<std::string>*) gaugePanelEntry.second["textureNames"])->push_back("");
	((std::vector<Vector2>*) gaugePanelEntry.second["textureSizes"])->push_back(Vector2());
	((std::vector<Vector2>*) gaugePanelEntry.second["textureSizes"])->push_back(Vector2());
	((std::vector<Vector2>*) gaugePanelEntry.second["textureSizes"])->push_back(Vector2());
	((std::vector<float>*) gaugePanelEntry.second["progressions"])->push_back(0.0f);
	((std::vector<float>*) gaugePanelEntry.second["progressions"])->push_back(0.0f);
	((std::vector<float>*) gaugePanelEntry.second["progressions"])->push_back(0.0f);
	((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsTop"])->push_back(Vector4());
	((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsTop"])->push_back(Vector4());
	((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsTop"])->push_back(Vector4());
	((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsBottom"])->push_back(Vector4());
	((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsBottom"])->push_back(Vector4());
	((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsBottom"])->push_back(Vector4());
/*
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
*/
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

	fortressEntry.first = "fortressRenderer";

	missileTrailEntry.first = "missileTrailRenderer";

	missileEntry.first = "missileRenderer";

	explosionEntry.first = "explosionRenderer";

	shellEntry.first = "shellRenderer";

	strikeEffectEntry.first = "strikeEffect";

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
	newHighScoreNameField.second = ((DrawField*) drawingMaster->drawers["field"])->instantiateArgList();
/*
	newHighScoreNameField.second["metrics"] = (void*) new UIMetrics;
	newHighScoreNameField.second["fontSize"] = (void*) new float;
	newHighScoreNameField.second["fontColor"] = (void*) new Vector4;
	newHighScoreNameField.second["boxColor"] = (void*) new Vector4;
	newHighScoreNameField.second["text"] = (void*) new std::string;
*/
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
*((Vector4*) fpsEntry.second["fontColor"]) = gameSystem->getColor("fontColorDark");
fpsEntry.second["text"] = (void*) new std::string;
*((std::string*) fpsEntry.second["text"]) = "FPS: 0";

	// build the initial draw stack
	currentScheme = SCHEME_MAINMENU;
	activeMenuSelection = &playButtonEntry;
//FIXME after schemes is gone we can put this back in because we can refer to our own data
//	reScheme();

	// start audio
	gameAudio->setBackgroundMusic("menuSong");

	// draw the initial frame
//	drawingMaster->execute(true);

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
	drawingMaster->drawStack.clear();
	drawingMaster->uiLayoutAuthority->metrics.clear();
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

std::stringstream ss; ss << "FPS: " << drawingMaster->runRate;
*((std::string*) fpsEntry.second["text"]) = ss.str().c_str();
*((float*) fpsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
((UIMetrics*) fpsEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(fpsEntry.second);
drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) fpsEntry.second["metrics"]);
drawingMaster->uiLayoutAuthority->rearrange();
drawingMaster->drawStack.push_back(fpsEntry);
}

unsigned int GameLogic::execute(bool unScheduled) {
	bool needRedraw = false;

if(lastFPSUpdate + 1000 < platform->getExecMills()) {
reScheme();
needRedraw = true;
lastFPSUpdate = platform->getExecMills();
}
	// get a delta time for schemes that need it
	float deltaTime = 0.0f;
	if(gameState != NULL && ! gameState->isPaused && currentScheme == SCHEME_PLAYING) {
		unsigned int currentGameTime = gameState->lastUpdateGameTime;
		deltaTime = (float) (currentGameTime - lastUpdate) / 1000.0f;
		lastUpdate = currentGameTime;
	}

	// universal logic
	if(quitKeyListener->popKey() != SDLK_UNKNOWN )
		keepProgramAlive = false;

	if(fullScreenKeyListener->popKey() != SDLK_UNKNOWN) {
		bool fullScreenGraphics = ! gameGraphics->fullScreen;

		Camera* currentCamera = gameGraphics->currentCamera;

		delete gameGraphics;

		gameGraphics = new GameGraphics(fullScreenGraphics);

		gameGraphics->currentCamera = currentCamera;

		drawingMaster->newGraphics();

		inputHandler->execute();
		mouseMotionListener->wasMoved();

		if(currentScheme == SCHEME_PLAYING || currentScheme == SCHEME_INTRO || currentScheme == SCHEME_PAUSED) {
			((DrawRadar*) drawingMaster->drawers["radar"])->reloadState();
			((ExplosionRenderer*) drawingMaster->drawers["explosionRenderer"])->reloadState();
			((TerrainRenderer*) drawingMaster->drawers["terrainRenderer"])->reloadState();
		}

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
					gameAudio->playSound("alterDownEffect");
				}
			} else if(settingsButtonZoneListener->isEntered) {
				if(activeMenuSelection != &settingsButtonEntry) {
					activeMenuSelection = &settingsButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(helpButtonZoneListener->isEntered) {
				if(activeMenuSelection != &helpButtonEntry) {
					activeMenuSelection = &helpButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(highScoresButtonZoneListener->isEntered) {
				if(activeMenuSelection != &highScoresButtonEntry) {
					activeMenuSelection = &highScoresButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(quitButtonZoneListener->isEntered) {
				if(activeMenuSelection != &quitButtonEntry) {
					activeMenuSelection = &quitButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
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
			drawingMaster->execute(true);

			gameAudio->playSound("selectEffect");

			gameState = new GameState();
			mainLoopModules[gameState] = 0;

			mainLoopModules[drawingMaster] = 0;
			((DrawRadar*) drawingMaster->drawers["radar"])->reloadState();
			((ExplosionRenderer*) drawingMaster->drawers["explosionRenderer"])->reloadState();
			((TerrainRenderer*) drawingMaster->drawers["terrainRenderer"])->reloadState();
			gameGraphics->currentCamera = &introCamera;

			currentScheme = SCHEME_INTRO;
			activeMenuSelection = NULL;
			reScheme();
			gameAudio->setBackgroundMusic("playingSong");
		} else if(settingsButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_SETTINGS;
			activeMenuSelection = &levelSettingEntry;
			reScheme();
			needRedraw = true;
			gameAudio->playSound("selectEffect");
		} else if(helpButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_HELP;
			activeMenuSelection = &backButtonEntry;
			reScheme();
			needRedraw = true;
			gameAudio->playSound("selectEffect");
		} else if(highScoresButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_HIGHSCORES;
			activeMenuSelection = &backButtonEntry;
			reScheme();
			needRedraw = true;
			gameAudio->playSound("selectEffect");
		} else if(quitButtonClickListener->wasClicked()) {
			keepProgramAlive = false;
		}

		// key hits
		for(SDLKey key = mainMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = mainMenuKeyListener->popKey()) {
			if(key == SDLK_UP) {
				if(activeMenuSelection == NULL || activeMenuSelection == &playButtonEntry) {
					activeMenuSelection = &quitButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &settingsButtonEntry) {
					activeMenuSelection = &playButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &highScoresButtonEntry) {
					activeMenuSelection = &settingsButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &helpButtonEntry) {
					activeMenuSelection = &highScoresButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &quitButtonEntry) {
					activeMenuSelection = &helpButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				}
			} else if(key == SDLK_DOWN) {
				if(activeMenuSelection == NULL || activeMenuSelection == &quitButtonEntry) {
					activeMenuSelection = &playButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &playButtonEntry) {
					activeMenuSelection = &settingsButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &settingsButtonEntry) {
					activeMenuSelection = &highScoresButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &highScoresButtonEntry) {
					activeMenuSelection = &helpButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &helpButtonEntry) {
					activeMenuSelection = &quitButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(key == SDLK_RETURN) {
				if(activeMenuSelection == &playButtonEntry) {
					currentScheme = SCHEME_LOADING;
					reScheme();
					drawingMaster->execute(true);

					gameAudio->playSound("selectEffect");

					gameState = new GameState();
					mainLoopModules[gameState] = 0;

					mainLoopModules[drawingMaster] = 0;
					((DrawRadar*) drawingMaster->drawers["radar"])->reloadState();
					((ExplosionRenderer*) drawingMaster->drawers["explosionRenderer"])->reloadState();
					((TerrainRenderer*) drawingMaster->drawers["terrainRenderer"])->reloadState();
					gameGraphics->currentCamera = &introCamera;

					currentScheme = SCHEME_INTRO;
					activeMenuSelection = NULL;
					reScheme();

					gameAudio->setBackgroundMusic("playingSong");
				} else if(activeMenuSelection == &settingsButtonEntry) {
					currentScheme = SCHEME_SETTINGS;
					activeMenuSelection = &levelSettingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("selectEffect");
				} else if(activeMenuSelection == &helpButtonEntry) {
					currentScheme = SCHEME_HELP;
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("selectEffect");
				} else if(activeMenuSelection == &highScoresButtonEntry) {
					currentScheme = SCHEME_HIGHSCORES;
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("selectEffect");
				} else if(activeMenuSelection == &quitButtonEntry) {
					keepProgramAlive = false;
				}
			}
		}
	} else if(currentScheme == SCHEME_INTRO) {
		// button clicks
		if(introMouseButtonListener->wasClicked()) {
			gameState->bumpStart();
			currentScheme = SCHEME_PLAYING;
			reScheme();
			SDL_WM_GrabInput(SDL_GRAB_ON);
			SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
			SDL_ShowCursor(0);
			inputHandler->execute();
			mouseMotionListener->wasMoved();
			gameGraphics->currentCamera = &fortressCamera;
		}

		// key hits
		for(SDLKey key = introKeyListener->popKey(); key != SDLK_UNKNOWN; key = introKeyListener->popKey()) {
			if(key == SDLK_SPACE) {
				gameState->bumpStart();
				currentScheme = SCHEME_PLAYING;
				reScheme();
				SDL_WM_GrabInput(SDL_GRAB_ON);
				SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
				SDL_ShowCursor(0);
				inputHandler->execute();
				mouseMotionListener->wasMoved();
				gameGraphics->currentCamera = &fortressCamera;
			} else if(key == SDLK_ESCAPE) {
				gameState->pause();

				mainLoopModules.erase(mainLoopModules.find(drawingMaster));

				currentScheme = SCHEME_PAUSED;
				activeMenuSelection = &resumeButtonEntry;
				reScheme();
				SDL_WM_GrabInput(SDL_GRAB_OFF);
				SDL_ShowCursor(1);

				needRedraw = true;

				gameAudio->playSound("backEffect");
			}
		}

		// expiration of intro time
		if((float) gameState->lastUpdateGameTime / 1000.0f > gameSystem->getFloat("stateShipEntryTime")) {
			currentScheme = SCHEME_PLAYING;
			reScheme();
			SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
			inputHandler->execute();
			mouseMotionListener->wasMoved();
			SDL_WM_GrabInput(SDL_GRAB_ON);
			SDL_ShowCursor(0);
			gameGraphics->currentCamera = &fortressCamera;
		}
	} else if(currentScheme == SCHEME_PLAYING) {
		// see if we're dead
		if(gameState->fortress.health == 0.0f) {
			gameState->pause();

			mainLoopModules.erase(mainLoopModules.find(drawingMaster));

			playerName = "";

			currentScheme = SCHEME_GAMEOVER;
			activeMenuSelection = &gameOverContinueButton;
			inputHandler->keyboard->listenUnicode = true;
			inputHandler->keyboard->unicodeChars = "";

			reScheme();
			SDL_WM_GrabInput(SDL_GRAB_OFF);
			SDL_ShowCursor(1);

			needRedraw = true;

			gameAudio->setBackgroundMusic("menuSong");
		}

		// check score
		if(atoi(((std::string*) scoreLabel.second["text"])->c_str()) != gameState->score)
			reScheme();

		// check gauges
		if(
				(*((std::vector<float>*) gaugePanelEntry.second["progressions"]))[0] != gameState->fortress.health ||
				(*((std::vector<float>*) gaugePanelEntry.second["progressions"]))[1] != gameState->fortress.ammunition ||
				(*((std::vector<float>*) gaugePanelEntry.second["progressions"]))[2] != gameState->fortress.emp
			) {
			reScheme();
		}

		// see if changing to mouse motion
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

			// constrain to control radius
			Vector2 correctAspectMousePosition(inputHandler->mouse->position.x * gameGraphics->aspectRatio, inputHandler->mouse->position.y);
			if(mag(correctAspectMousePosition) > gameSystem->getFloat("hudControlAreaRadius") * 2.0f / gameGraphics->resolutionY) {
				Vector2 mousePositionVector = correctAspectMousePosition * (gameSystem->getFloat("hudControlAreaRadius") * 2.0f / gameGraphics->resolutionY) / mag(correctAspectMousePosition);

				SDL_WarpMouse(
						gameGraphics->resolutionX / 2 + (unsigned short int) (mousePositionVector.x * (float) (gameGraphics->resolutionX / 2) / gameGraphics->aspectRatio),
						gameGraphics->resolutionY / 2 - (unsigned short int) (mousePositionVector.y * (float) (gameGraphics->resolutionY / 2))
					);

				inputHandler->execute();

				mouseMotionListener->wasMoved();
			}
		}

		// do movement
		if(mouseActive) {
			Vector2 correctAspectMousePosition(inputHandler->mouse->position.x * gameGraphics->aspectRatio, inputHandler->mouse->position.y);
			float effectiveDeadAreaRadius = gameSystem->getFloat("hudControlSpotSize") / (float) gameGraphics->resolutionY;

			if(mag(correctAspectMousePosition) > effectiveDeadAreaRadius) {
				// also raise fraction to exponent for more precise movement near center
				Vector2 movementVector =
						correctAspectMousePosition * pow(
								(mag(correctAspectMousePosition) - effectiveDeadAreaRadius) /
								(gameSystem->getFloat("hudControlAreaRadius") * 2.0f / gameGraphics->resolutionY - effectiveDeadAreaRadius),
								gameSystem->getFloat("hudCursorPositionExponent")
							) / (gameSystem->getFloat("hudControlAreaRadius") * 2.0f / gameGraphics->resolutionY);

				gameState->fortress.addRotation(gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime * movementVector.x);
				gameState->fortress.addTilt(gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime * movementVector.y);
			}
		}

		// button clicks
		if(
				primaryFireClickListener1->wasClicked() ||
				primaryFireClickListener2->wasClicked() ||
				primaryFireClickListener3->wasClicked()
			) {
			gameState->fireShell();
		}
		if(secondaryFireClickListener->wasClicked()) {
			gameState->empIsCharging = ! gameState->empIsCharging;
		}

		if(binocularsClickListener->wasClicked()) {
			gameState->binoculars = ! gameState->binoculars;
			reScheme();
		}

		// key hits
		for(SDLKey key = playingKeyListener->popKey(); key != SDLK_UNKNOWN; key = playingKeyListener->popKey()) {
			if(key == SDLK_SPACE) {
				gameState->fireShell();
			} else if(key == SDLK_TAB) {
				gameState->empIsCharging = ! gameState->empIsCharging;
			} else if(key == SDLK_RETURN) {
				mainLoopModules.erase(mainLoopModules.find(gameState));
				delete gameState;
				gameState = new GameState();
				mainLoopModules[gameState] = 0;

				((DrawRadar*) drawingMaster->drawers["radar"])->reloadState();
				((ExplosionRenderer*) drawingMaster->drawers["explosionRenderer"])->reloadState();
				((TerrainRenderer*) drawingMaster->drawers["terrainRenderer"])->reloadState();
			} else if(key == SDLK_BACKSLASH) {
				if(gameGraphics->currentCamera == &fortressCamera)
					gameGraphics->currentCamera = &orbitCamera;
				else if(gameGraphics->currentCamera == &orbitCamera)
					gameGraphics->currentCamera = &presentationCamera;
				else if(gameGraphics->currentCamera == &presentationCamera)
					gameGraphics->currentCamera = &roamingCamera;
				else if(gameGraphics->currentCamera == &roamingCamera)
					gameGraphics->currentCamera = &fortressCamera;
			} else if(key == SDLK_ESCAPE) {
				gameState->pause();

				mainLoopModules.erase(mainLoopModules.find(drawingMaster));

				currentScheme = SCHEME_PAUSED;
				activeMenuSelection = &resumeButtonEntry;
				reScheme();
				SDL_WM_GrabInput(SDL_GRAB_OFF);
				SDL_ShowCursor(1);

				needRedraw = true;

				gameAudio->playSound("backEffect");
			} else if(key == SDLK_BACKQUOTE) {
				if(gameState->isPaused)
					gameState->resume();
				else
					gameState->pause();
			} else if(key == SDLK_LSHIFT || key == SDLK_RSHIFT) {
				gameState->binoculars = ! gameState->binoculars;
				reScheme();
			}
		}

		// arrow key dampening; this will actually put the time behind by one frame
		if(! turretLeftKeyListener->isDown) leftArrowPressTime = gameState->lastUpdateGameTime;
		if(! turretRightKeyListener->isDown) rightArrowPressTime = gameState->lastUpdateGameTime;
		if(! turretUpKeyListener->isDown) upArrowPressTime = gameState->lastUpdateGameTime;
		if(! turretDownKeyListener->isDown) downArrowPressTime = gameState->lastUpdateGameTime;

		// keys down
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
				float dampeningFactor = gameSystem->getFloat("stateKeyDampeningBasePortion") +
						(1.0f - gameSystem->getFloat("stateKeyDampeningBasePortion")) *
						pow(
								(float) (gameState->lastUpdateGameTime - leftArrowPressTime) /
								1000.0f /
								gameSystem->getFloat("stateKeyDampeningTime"),
								gameSystem->getFloat("stateKeyDampeningExponent")
							);
				gameState->fortress.addRotation(-gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime * (dampeningFactor < 1.0f ? dampeningFactor : 1.0f));
				mouseActive = false;
			}
			if(turretRightKeyListener->isDown) {
				float dampeningFactor = gameSystem->getFloat("stateKeyDampeningBasePortion") +
						(1.0f - gameSystem->getFloat("stateKeyDampeningBasePortion")) *
						pow(
								(float) (gameState->lastUpdateGameTime - rightArrowPressTime) /
								1000.0f /
								gameSystem->getFloat("stateKeyDampeningTime"),
								gameSystem->getFloat("stateKeyDampeningExponent")
							);
				gameState->fortress.addRotation(gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime * (dampeningFactor < 1.0f ? dampeningFactor : 1.0f));
				mouseActive = false;
			}
			if(turretUpKeyListener->isDown) {
				float dampeningFactor = gameSystem->getFloat("stateKeyDampeningBasePortion") +
						(1.0f - gameSystem->getFloat("stateKeyDampeningBasePortion")) *
						pow(
								(float) (gameState->lastUpdateGameTime - upArrowPressTime) /
								1000.0f /
								gameSystem->getFloat("stateKeyDampeningTime"),
								gameSystem->getFloat("stateKeyDampeningExponent")
							);
				gameState->fortress.addTilt(gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime * (dampeningFactor < 1.0f ? dampeningFactor : 1.0f));
				mouseActive = false;
			}
			if(turretDownKeyListener->isDown) {
				float dampeningFactor = gameSystem->getFloat("stateKeyDampeningBasePortion") +
						(1.0f - gameSystem->getFloat("stateKeyDampeningBasePortion")) *
						pow(
								(float) (gameState->lastUpdateGameTime - downArrowPressTime) /
								1000.0f /
								gameSystem->getFloat("stateKeyDampeningTime"),
								gameSystem->getFloat("stateKeyDampeningExponent")
							);
				gameState->fortress.addTilt(-gameSystem->getFloat("stateTurretTurnSpeed") * deltaTime * (dampeningFactor < 1.0f ? dampeningFactor : 1.0f));
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

//			newKeyboardCursorPosition.x *= gameSystem->getFloat("hudControlBoxSize") / 100.0f / gameGraphics->aspectRatio;
//			newKeyboardCursorPosition.y *= gameSystem->getFloat("hudControlBoxSize") / 100.0f;

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
					gameAudio->playSound("alterDownEffect");
				}
			} else if(endGameButtonZoneListener->isEntered) {
				if(activeMenuSelection != &endGameButtonEntry) {
					activeMenuSelection = &endGameButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
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
				currentScheme = SCHEME_PLAYING;
				SDL_WM_GrabInput(SDL_GRAB_ON);
				SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
				SDL_ShowCursor(0);

				inputHandler->execute();
				mouseMotionListener->wasMoved();
			}

			activeMenuSelection = NULL;
			reScheme();

			mainLoopModules[drawingMaster] = 0;

			gameState->resume();

			gameAudio->playSound("selectEffect");
		} else if(endGameButtonClickListener->wasClicked()) {
				inputHandler->keyboard->listenUnicode = true;
				inputHandler->keyboard->unicodeChars = "";

				playerName = "";

				currentScheme = SCHEME_GAMEOVER;
				activeMenuSelection = &gameOverContinueButton;
				reScheme();
				needRedraw = true;

				gameAudio->playSound("selectEffect");

				gameAudio->setBackgroundMusic("menuSong");
		}

		// key hits
		for(SDLKey key = pausedMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = pausedMenuKeyListener->popKey()) {
			if(key == SDLK_UP) {
				if(activeMenuSelection == NULL || activeMenuSelection == &resumeButtonEntry) {
					activeMenuSelection = &endGameButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &endGameButtonEntry) {
					activeMenuSelection = &resumeButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				}
			} else if(key == SDLK_DOWN) {
				if(activeMenuSelection == NULL || activeMenuSelection == &endGameButtonEntry) {
					activeMenuSelection = &resumeButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &resumeButtonEntry) {
					activeMenuSelection = &endGameButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(key == SDLK_RETURN || key == SDLK_ESCAPE) {
				if(activeMenuSelection == &resumeButtonEntry || key == SDLK_ESCAPE) {
					if(gameGraphics->currentCamera == &introCamera) {
						currentScheme = SCHEME_INTRO;
					} else {
						currentScheme = SCHEME_PLAYING;
						SDL_WM_GrabInput(SDL_GRAB_ON);
						SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
						SDL_ShowCursor(0);

						inputHandler->execute();
						mouseMotionListener->wasMoved();
					}

					activeMenuSelection = NULL;
					reScheme();

					mainLoopModules[drawingMaster] = 0;

					gameState->resume();

					if(key == SDLK_RETURN)
						gameAudio->playSound("selectEffect");
					else
						gameAudio->playSound("backEffect");
				} else if(activeMenuSelection == &endGameButtonEntry) {
					inputHandler->keyboard->listenUnicode = true;
					inputHandler->keyboard->unicodeChars = "";

					playerName = "";

					currentScheme = SCHEME_GAMEOVER;
					activeMenuSelection = &gameOverContinueButton;
					reScheme();
					needRedraw = true;

					gameAudio->playSound("selectEffect");

					gameAudio->setBackgroundMusic("menuSong");
				}
			}
		}
	} else if(currentScheme == SCHEME_GAMEOVER) {
		// typing
		if(inputHandler->keyboard->unicodeChars != "") {
			if(playerName.length() < gameSystem->getFloat("hudFieldWidth")) {
				for(size_t i = 0; i < inputHandler->keyboard->unicodeChars.length(); ++i) {
					if(gameSystem->getString("inputAllowedNameChars").find(inputHandler->keyboard->unicodeChars.substr(i, 1)) != std::string::npos) {
						playerName += inputHandler->keyboard->unicodeChars.substr(i, 1);

						activeMenuSelection = &gameOverContinueButton;

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

				activeMenuSelection = &gameOverContinueButton;

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

				activeMenuSelection = &gameOverContinueButton;

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
					gameAudio->playSound("alterDownEffect");
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				reScheme();
				needRedraw = true;
			}
		}

		// button clicks
		if(gameOverContinueButtonClickListener->wasClicked()) {
			if(gameState->score > 0 && (gameSystem->highScores.size() == 0 || gameState->score > gameSystem->highScores.back().first)) {
				if(playerName != "") {
					size_t position = 0;
					while(position < gameSystem->highScores.size() && gameSystem->highScores[position].first >= gameState->score)
						++position;

					if(position == gameSystem->highScores.size())
						gameSystem->highScores.push_back(std::make_pair(gameState->score, playerName));
					else
						gameSystem->highScores.insert(gameSystem->highScores.begin() + position, std::make_pair(gameState->score, playerName));
					while(gameSystem->highScores.size() > (size_t) gameSystem->getFloat("gameMaximumHighScores"))
						gameSystem->highScores.erase(gameSystem->highScores.end());

					gameSystem->flushPreferences();
				}
			}

			mainLoopModules.erase(mainLoopModules.find(gameState));
			delete gameState;
			gameState = NULL;

			currentScheme = SCHEME_MAINMENU;
			activeMenuSelection = &playButtonEntry;
			reScheme();
			needRedraw = true;

			gameAudio->playSound("selectEffect");
		}

		// key hits
		for(SDLKey key = gameOverKeyListener->popKey(); key != SDLK_UNKNOWN; key = gameOverKeyListener->popKey()) {
			if(key == SDLK_UP || key == SDLK_DOWN) {
				if(activeMenuSelection != &gameOverContinueButton) {
					activeMenuSelection = &gameOverContinueButton;
					reScheme();
					needRedraw = true;
					if(key == SDLK_UP)
						gameAudio->playSound("alterUpEffect");
					else
						gameAudio->playSound("alterDownEffect");
				}
			} else if(key == SDLK_RETURN) {
				if(activeMenuSelection == &gameOverContinueButton) {
					if(gameState->score > 0 && (gameSystem->highScores.size() == 0 || gameState->score > gameSystem->highScores.back().first)) {
						if(playerName != "") {
							size_t position = 0;
							while(position < gameSystem->highScores.size() && gameSystem->highScores[position].first >= gameState->score)
								++position;

							if(position == gameSystem->highScores.size())
								gameSystem->highScores.push_back(std::make_pair(gameState->score, playerName));
							else
								gameSystem->highScores.insert(gameSystem->highScores.begin() + position, std::make_pair(gameState->score, playerName));
							while(gameSystem->highScores.size() > (size_t) gameSystem->getFloat("gameMaximumHighScores"))
								gameSystem->highScores.erase(gameSystem->highScores.end());

							gameSystem->flushPreferences();
						}
					}

					mainLoopModules.erase(mainLoopModules.find(gameState));
					delete gameState;
					gameState = NULL;

					currentScheme = SCHEME_MAINMENU;
					activeMenuSelection = &playButtonEntry;
					reScheme();
					needRedraw = true;

					gameAudio->playSound("selectEffect");
				}
			} else if(key == SDLK_ESCAPE) {
				mainLoopModules.erase(mainLoopModules.find(gameState));
				delete gameState;
				gameState = NULL;

				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				reScheme();
				needRedraw = true;

				gameAudio->playSound("backEffect");
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
					gameAudio->playSound("alterDownEffect");
				}
			} else if(musicButtonZoneListener->isEntered) {
				if(activeMenuSelection != &musicSettingEntry) {
					activeMenuSelection = &musicSettingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(audioEffectsButtonZoneListener->isEntered) {
				if(activeMenuSelection != &audioEffectsSettingEntry) {
					activeMenuSelection = &audioEffectsSettingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(fullscreenButtonZoneListener->isEntered) {
				if(activeMenuSelection != &fullscreenSettingEntry) {
					activeMenuSelection = &fullscreenSettingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(windowedScreenResolutionButtonZoneListener->isEntered) {
				if(activeMenuSelection != &windowedScreenResolutionEntry) {
					activeMenuSelection = &windowedScreenResolutionEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(framerateLimitingButtonZoneListener->isEntered) {
				if(activeMenuSelection != &framerateLimitingEntry) {
					activeMenuSelection = &framerateLimitingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(multisamplingButtonZoneListener->isEntered) {
				if(activeMenuSelection != &multisamplingLevelEntry) {
					activeMenuSelection = &multisamplingLevelEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(terrainDetailButtonZoneListener->isEntered) {
				if(activeMenuSelection != &terrainDetailEntry) {
					activeMenuSelection = &terrainDetailEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(backButtonZoneListener->isEntered) {
				if(activeMenuSelection != &backButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(resetHighScoresButtonZoneListener->isEntered) {
				if(activeMenuSelection != &resetHighScoresEntry) {
					activeMenuSelection = &resetHighScoresEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
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

			gameAudio->playSound("alterDownEffect");
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

			gameAudio->playSound("alterDownEffect");
		} else if(audioEffectsButtonClickListener->wasClicked()) {
			float value = gameSystem->getFloat("audioEffectsVolume");
			value += 0.1f;
			if(value > 1.0f) value = 0.0f;
			gameSystem->setStandard("audioEffectsVolume", value, "");
			gameSystem->flushPreferences();

			reScheme();
			needRedraw = true;

			gameAudio->playSound("alterDownEffect");
		} else if(fullscreenButtonClickListener->wasClicked()) {
			gameSystem->setStandard("displayStartFullscreen", ! gameSystem->getBool("displayStartFullscreen"), "");
			gameSystem->flushPreferences();

			reScheme();
			needRedraw = true;

			gameAudio->playSound("alterDownEffect");
		} else if(windowedScreenResolutionButtonClickListener->wasClicked()) {
			std::vector< std::pair<unsigned int, unsigned int> > allowedResolutions = gameSystem->getAllowedWindowResolutions();

			size_t i = 0;
			while(i < allowedResolutions.size()) {
				std::stringstream resolutionText;
				resolutionText << allowedResolutions[i].first << "x" << allowedResolutions[i].second;

				if(resolutionText.str() == gameSystem->getString("displayWindowedResolution"))
					break;

				++i;
			}

			++i;
			if(i >= allowedResolutions.size())
				i = 0;

			std::stringstream resolutionText;
			resolutionText << allowedResolutions[i].first << "x" << allowedResolutions[i].second;

			// only update it if more than one resolution is allowed
			if(resolutionText.str() != gameSystem->getString("displayWindowedResolution")) {
				gameSystem->setStandard("displayWindowedResolution", resolutionText.str().c_str());
				gameSystem->flushPreferences();

				if(! gameGraphics->fullScreen) {
					// recreate the window
					delete gameGraphics;

					gameGraphics = new GameGraphics(false);

					drawingMaster->newGraphics();

					inputHandler->execute();
					mouseMotionListener->wasMoved();
				}

				reScheme();
				needRedraw = true;
			}

			gameAudio->playSound("alterDownEffect");
		} else if(framerateLimitingButtonClickListener->wasClicked()) {
			if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_VSYNC) {
				gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_30);
			} else if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_30) {
				gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_60);
			} else if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_60) {
				gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_120);
			} else if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_120) {
				gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_OFF);
			} else {
				gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_VSYNC);
			}

			gameSystem->flushPreferences();

			bool fullScreen = gameGraphics->fullScreen;
			delete gameGraphics;
			gameGraphics = new GameGraphics(fullScreen);
			drawingMaster->newGraphics();
			inputHandler->execute();
			mouseMotionListener->wasMoved();

			reScheme();
			needRedraw = true;

			gameAudio->playSound("alterDownEffect");
		} else if(multisamplingButtonClickListener->wasClicked()) {
			if(gameSystem->getFloat("displayMultisamplingLevel") == 0.0f) {
				gameSystem->setStandard("displayMultisamplingLevel", 2.0f);
			} else if(gameSystem->getFloat("displayMultisamplingLevel") == 2.0f) {
				gameSystem->setStandard("displayMultisamplingLevel", 4.0f);
			} else {
				gameSystem->setStandard("displayMultisamplingLevel", 0.0f);
			}

			gameSystem->flushPreferences();

			bool fullScreen = gameGraphics->fullScreen;
			delete gameGraphics;
			gameGraphics = new GameGraphics(fullScreen);
			drawingMaster->newGraphics();
			inputHandler->execute();
			mouseMotionListener->wasMoved();

			reScheme();
			needRedraw = true;

			gameAudio->playSound("alterDownEffect");
		} else if(terrainDetailButtonClickListener->wasClicked()) {
			if(gameSystem->getFloat("islandTerrainDetail") == 1.0f) {
				gameSystem->setStandard("islandTerrainDetail", 2.0f);
			} else if(gameSystem->getFloat("islandTerrainDetail") == 2.0f) {
				gameSystem->setStandard("islandTerrainDetail", 3.0f);
			} else {
				gameSystem->setStandard("islandTerrainDetail", 1.0f);
			}

			gameSystem->flushPreferences();

			reScheme();
			needRedraw = true;

			gameAudio->playSound("selectEffect");
		} else if(backButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_MAINMENU;
			activeMenuSelection = NULL;
			reScheme();
			needRedraw = true;

			gameAudio->playSound("selectEffect");
		} else if(resetHighScoresButtonClickListener->wasClicked()) {
			gameSystem->highScores.clear();
			gameSystem->flushPreferences();

			gameAudio->playSound("selectEffect");
		}

		// key hits
		for(SDLKey key = settingsMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = settingsMenuKeyListener->popKey()) {
			if(key == SDLK_UP) {
				if(activeMenuSelection == NULL || activeMenuSelection == &levelSettingEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &backButtonEntry) {
					activeMenuSelection = &resetHighScoresEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &resetHighScoresEntry) {
					activeMenuSelection = &terrainDetailEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &terrainDetailEntry) {
					if(gameGraphics->supportsMultisampling)
						activeMenuSelection = &multisamplingLevelEntry;
					else
						activeMenuSelection = &framerateLimitingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &multisamplingLevelEntry) {
					activeMenuSelection = &framerateLimitingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &framerateLimitingEntry) {
					activeMenuSelection = &windowedScreenResolutionEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &windowedScreenResolutionEntry) {
					activeMenuSelection = &fullscreenSettingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &fullscreenSettingEntry) {
					activeMenuSelection = &audioEffectsSettingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &audioEffectsSettingEntry) {
					activeMenuSelection = &musicSettingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &musicSettingEntry) {
					activeMenuSelection = &levelSettingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				}
			} else if(key == SDLK_DOWN) {
				if(activeMenuSelection == NULL || activeMenuSelection == &backButtonEntry) {
					activeMenuSelection = &levelSettingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &levelSettingEntry) {
					activeMenuSelection = &musicSettingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &musicSettingEntry) {
					activeMenuSelection = &audioEffectsSettingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &audioEffectsSettingEntry) {
					activeMenuSelection = &fullscreenSettingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &fullscreenSettingEntry) {
					activeMenuSelection = &windowedScreenResolutionEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &windowedScreenResolutionEntry) {
					activeMenuSelection = &framerateLimitingEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &framerateLimitingEntry) {
					if(gameGraphics->supportsMultisampling)
						activeMenuSelection = &multisamplingLevelEntry;
					else
						activeMenuSelection = &terrainDetailEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &multisamplingLevelEntry) {
					activeMenuSelection = &terrainDetailEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &terrainDetailEntry) {
					activeMenuSelection = &resetHighScoresEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &resetHighScoresEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
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

					if(key == SDLK_RIGHT)
						gameAudio->playSound("alterUpEffect");
					else
						gameAudio->playSound("alterDownEffect");
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

					if(key == SDLK_RIGHT)
						gameAudio->playSound("alterUpEffect");
					else
						gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &audioEffectsSettingEntry) {
					float value = gameSystem->getFloat("audioEffectsVolume");
					value += (key == SDLK_RIGHT ? 0.1f : -0.1f);
					if(value > 1.0f) value = 0.0f;
					if(value < 0.0f) value = 1.0f;
					gameSystem->setStandard("audioEffectsVolume", value, "");
					gameSystem->flushPreferences();

					reScheme();
					needRedraw = true;

					if(key == SDLK_RIGHT)
						gameAudio->playSound("alterUpEffect");
					else
						gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &fullscreenSettingEntry) {
					gameSystem->setStandard("displayStartFullscreen", ! gameSystem->getBool("displayStartFullscreen"), "");
					gameSystem->flushPreferences();

					reScheme();
					needRedraw = true;

					if(key == SDLK_RIGHT)
						gameAudio->playSound("alterUpEffect");
					else
						gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &windowedScreenResolutionEntry) {
					std::vector< std::pair<unsigned int, unsigned int> > allowedResolutions = gameSystem->getAllowedWindowResolutions();

					size_t i = 0;
					while(i < allowedResolutions.size()) {
						std::stringstream resolutionText;
						resolutionText << allowedResolutions[i].first << "x" << allowedResolutions[i].second;

						if(resolutionText.str() == gameSystem->getString("displayWindowedResolution"))
							break;

						++i;
					}

					if(key == SDLK_RIGHT) {
						++i;
						if(i >= allowedResolutions.size())
							i = 0;
					} else {
						if(i == 0)
							i = allowedResolutions.size() - 1;
						else
							--i;
					}
					std::stringstream resolutionText;
					resolutionText << allowedResolutions[i].first << "x" << allowedResolutions[i].second;

					// only update it if more than one resolution is allowed
					if(resolutionText.str() != gameSystem->getString("displayWindowedResolution")) {
						gameSystem->setStandard("displayWindowedResolution", resolutionText.str().c_str());
						gameSystem->flushPreferences();

						if(! gameGraphics->fullScreen) {
							// recreate the window
							delete gameGraphics;

							gameGraphics = new GameGraphics(false);

							drawingMaster->newGraphics();

							inputHandler->execute();
							mouseMotionListener->wasMoved();
						}

						reScheme();
						needRedraw = true;
					}

					if(key == SDLK_RIGHT)
						gameAudio->playSound("alterUpEffect");
					else
						gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &framerateLimitingEntry) {
					if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_VSYNC) {
						if(key == SDLK_LEFT)
							gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_OFF);
						else
							gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_30);
					} else if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_30) {
						if(key == SDLK_LEFT)
							gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_VSYNC);
						else
							gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_60);
					} else if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_60) {
						if(key == SDLK_LEFT)
							gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_30);
						else
							gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_120);
					} else if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_120) {
						if(key == SDLK_LEFT)
							gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_60);
						else
							gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_OFF);
					} else {
						if(key == SDLK_LEFT)
							gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_120);
						else
							gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_VSYNC);
					}

					gameSystem->flushPreferences();

					bool fullScreen = gameGraphics->fullScreen;
					delete gameGraphics;
					gameGraphics = new GameGraphics(fullScreen);
					drawingMaster->newGraphics();
					inputHandler->execute();
					mouseMotionListener->wasMoved();

					reScheme();
					needRedraw = true;

					if(key == SDLK_RIGHT)
						gameAudio->playSound("alterUpEffect");
					else
						gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &multisamplingLevelEntry) {
					if(gameSystem->getFloat("displayMultisamplingLevel") == 0.0f) {
						if(key == SDLK_LEFT)
							gameSystem->setStandard("displayMultisamplingLevel", 4.0f);
						else
							gameSystem->setStandard("displayMultisamplingLevel", 2.0f);
					} else if(gameSystem->getFloat("displayMultisamplingLevel") == 2.0f) {
						if(key == SDLK_LEFT)
							gameSystem->setStandard("displayMultisamplingLevel", 0.0f);
						else
							gameSystem->setStandard("displayMultisamplingLevel", 4.0f);
					} else {
						if(key == SDLK_LEFT)
							gameSystem->setStandard("displayMultisamplingLevel", 2.0f);
						else
							gameSystem->setStandard("displayMultisamplingLevel", 0.0f);
					}

					gameSystem->flushPreferences();

					bool fullScreen = gameGraphics->fullScreen;
					delete gameGraphics;
					gameGraphics = new GameGraphics(fullScreen);
					drawingMaster->newGraphics();
					inputHandler->execute();
					mouseMotionListener->wasMoved();

					reScheme();
					needRedraw = true;

					if(key == SDLK_RIGHT)
						gameAudio->playSound("alterUpEffect");
					else
						gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &terrainDetailEntry) {
					if(gameSystem->getFloat("islandTerrainDetail") == 1.0f) {
						if(key == SDLK_LEFT)
							gameSystem->setStandard("islandTerrainDetail", 3.0f);
						else
							gameSystem->setStandard("islandTerrainDetail", 2.0f);
					} else if(gameSystem->getFloat("islandTerrainDetail") == 2.0f) {
						if(key == SDLK_LEFT)
							gameSystem->setStandard("islandTerrainDetail", 1.0f);
						else
							gameSystem->setStandard("islandTerrainDetail", 3.0f);
					} else {
						if(key == SDLK_LEFT)
							gameSystem->setStandard("islandTerrainDetail", 2.0f);
						else
							gameSystem->setStandard("islandTerrainDetail", 1.0f);
					}

					gameSystem->flushPreferences();

					reScheme();
					needRedraw = true;

					if(key == SDLK_RIGHT)
						gameAudio->playSound("alterUpEffect");
					else
						gameAudio->playSound("alterDownEffect");
				}
			} else if(key == SDLK_RETURN) {
				if(activeMenuSelection == &resetHighScoresEntry) {
					gameSystem->highScores.clear();
					gameSystem->flushPreferences();
					gameAudio->playSound("selectEffect");
				} else if(activeMenuSelection == &backButtonEntry) {
					currentScheme = SCHEME_MAINMENU;
					activeMenuSelection = &playButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("selectEffect");
				}
			} else if(key == SDLK_ESCAPE) {
				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				reScheme();
				needRedraw = true;
				gameAudio->playSound("backEffect");
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
					gameAudio->playSound("alterDownEffect");
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
			gameAudio->playSound("selectEffect");
		}

		// key hits
		for(SDLKey key = highScoresMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = highScoresMenuKeyListener->popKey()) {
			if((key == SDLK_UP || key == SDLK_DOWN) && activeMenuSelection != &backButtonEntry) {
				activeMenuSelection = &backButtonEntry;
				reScheme();
				needRedraw = true;
				gameAudio->playSound("alterDownEffect");
			} else if((key == SDLK_RETURN && activeMenuSelection == &backButtonEntry) || key == SDLK_ESCAPE) {
				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				reScheme();
				needRedraw = true;

				if(key == SDLK_RETURN)
					gameAudio->playSound("selectEffect");
				else
					gameAudio->playSound("backEffect");
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
					gameAudio->playSound("alterDownEffect");
				}
			} else if(backButtonZoneListener->isEntered) {
				if(activeMenuSelection != &backButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
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
			gameAudio->playSound("selectEffect");
		} else if(backButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_MAINMENU;
			activeMenuSelection = &playButtonEntry;
			reScheme();
			needRedraw = true;
			gameAudio->playSound("selectEffect");
		}

		// key hits
		for(SDLKey key = helpMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = helpMenuKeyListener->popKey()) {
			if(key == SDLK_UP) {
				if(activeMenuSelection == NULL || activeMenuSelection == &aboutButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &backButtonEntry) {
					activeMenuSelection = &aboutButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				}
			} else if(key == SDLK_DOWN) {
				if(activeMenuSelection == NULL || activeMenuSelection == &backButtonEntry) {
					activeMenuSelection = &aboutButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &aboutButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(key == SDLK_RETURN) {
				if(activeMenuSelection == &aboutButtonEntry) {
					currentScheme = SCHEME_ABOUT;
					activeMenuSelection = &backButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("selectEffect");
				} else if(activeMenuSelection == &backButtonEntry) {
					currentScheme = SCHEME_MAINMENU;
					activeMenuSelection = &playButtonEntry;
					reScheme();
					needRedraw = true;
					gameAudio->playSound("selectEffect");
				}
			} else if(key == SDLK_ESCAPE) {
				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				reScheme();
				needRedraw = true;
				gameAudio->playSound("backEffect");
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
					gameAudio->playSound("alterDownEffect");
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
			gameAudio->playSound("selectEffect");
		}

		// key hits
		for(SDLKey key = helpMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = helpMenuKeyListener->popKey()) {
			if((key == SDLK_UP || key == SDLK_DOWN) && activeMenuSelection != &backButtonEntry) {
				activeMenuSelection = &backButtonEntry;
				reScheme();
				needRedraw = true;
				if(key == SDLK_UP)
					gameAudio->playSound("alterUpEffect");
				else
					gameAudio->playSound("alterDownEffect");
			} else if((key == SDLK_RETURN && activeMenuSelection == &backButtonEntry) || key == SDLK_ESCAPE) {
				currentScheme = SCHEME_HELP;
				activeMenuSelection = &backButtonEntry;
				reScheme();
				needRedraw = true;
				if(key == SDLK_RETURN)
					gameAudio->playSound("selectEffect");
				else
					gameAudio->playSound("backEffect");
			}
		}
	}

	// see if we need to redraw graphics, if we're still in the menus
	if(needRedraw && mainLoopModules.find(drawingMaster) == mainLoopModules.end())
		drawingMaster->execute(true);

	// track runcount
	trackRunCount();

	// calculate and return sleep time from superclass
	if(unScheduled)
		return 0;
	else
		return getSleepTime();
}
