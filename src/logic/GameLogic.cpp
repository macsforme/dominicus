// GameLogic.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "logic/GameLogic.h"

GameLogic::GameLogic() {
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
	playButtonEntry.second["insideColor"] = (void*) new Vector4;
	playButtonEntry.second["highlightColor"] = (void*) new Vector4;
	playButtonEntry.second["borderColor"] = (void*) new Vector4;
	playButtonEntry.second["outsideColor"] = (void*) new Vector4;
	playButtonEntry.second["padding"] = (void*) new float;
	playButtonEntry.second["border"] = (void*) new float;
	playButtonEntry.second["containerTimer"] = NULL;
	playButtonEntry.second["textTimer"] = NULL;
	playButtonZoneListener = new MouseZoneListener();
	playButtonClickListener = new MouseButtonListener();

	settingsButtonEntry.first = "button";
	settingsButtonEntry.second["metrics"] = (void*) new UIMetrics;
	settingsButtonEntry.second["fontSize"] = (void*) new float;
	settingsButtonEntry.second["fontColor"] = (void*) new Vector4;
	settingsButtonEntry.second["text"] = (void*) new std::string;
	settingsButtonEntry.second["insideColor"] = (void*) new Vector4;
	settingsButtonEntry.second["highlightColor"] = (void*) new Vector4;
	settingsButtonEntry.second["borderColor"] = (void*) new Vector4;
	settingsButtonEntry.second["outsideColor"] = (void*) new Vector4;
	settingsButtonEntry.second["padding"] = (void*) new float;
	settingsButtonEntry.second["border"] = (void*) new float;
	settingsButtonEntry.second["containerTimer"] = NULL;
	settingsButtonEntry.second["textTimer"] = NULL;
	settingsButtonZoneListener = new MouseZoneListener();
	settingsButtonClickListener = new MouseButtonListener();

	helpButtonEntry.first = "button";
	helpButtonEntry.second["metrics"] = (void*) new UIMetrics;
	helpButtonEntry.second["fontSize"] = (void*) new float;
	helpButtonEntry.second["fontColor"] = (void*) new Vector4;
	helpButtonEntry.second["text"] = (void*) new std::string;
	helpButtonEntry.second["insideColor"] = (void*) new Vector4;
	helpButtonEntry.second["highlightColor"] = (void*) new Vector4;
	helpButtonEntry.second["borderColor"] = (void*) new Vector4;
	helpButtonEntry.second["outsideColor"] = (void*) new Vector4;
	helpButtonEntry.second["padding"] = (void*) new float;
	helpButtonEntry.second["border"] = (void*) new float;
	helpButtonEntry.second["containerTimer"] = NULL;
	helpButtonEntry.second["textTimer"] = NULL;
	helpButtonZoneListener = new MouseZoneListener();
	helpButtonClickListener = new MouseButtonListener();

	highScoresButtonEntry.first = "button";
	highScoresButtonEntry.second["metrics"] = (void*) new UIMetrics;
	highScoresButtonEntry.second["fontSize"] = (void*) new float;
	highScoresButtonEntry.second["fontColor"] = (void*) new Vector4;
	highScoresButtonEntry.second["text"] = (void*) new std::string;
	highScoresButtonEntry.second["insideColor"] = (void*) new Vector4;
	highScoresButtonEntry.second["highlightColor"] = (void*) new Vector4;
	highScoresButtonEntry.second["borderColor"] = (void*) new Vector4;
	highScoresButtonEntry.second["outsideColor"] = (void*) new Vector4;
	highScoresButtonEntry.second["padding"] = (void*) new float;
	highScoresButtonEntry.second["border"] = (void*) new float;
	highScoresButtonEntry.second["containerTimer"] = NULL;
	highScoresButtonEntry.second["textTimer"] = NULL;
	highScoresButtonZoneListener = new MouseZoneListener();
	highScoresButtonClickListener = new MouseButtonListener();

	quitButtonEntry.first = "button";
	quitButtonEntry.second["metrics"] = (void*) new UIMetrics;
	quitButtonEntry.second["fontSize"] = (void*) new float;
	quitButtonEntry.second["fontColor"] = (void*) new Vector4;
	quitButtonEntry.second["text"] = (void*) new std::string;
	quitButtonEntry.second["insideColor"] = (void*) new Vector4;
	quitButtonEntry.second["highlightColor"] = (void*) new Vector4;
	quitButtonEntry.second["borderColor"] = (void*) new Vector4;
	quitButtonEntry.second["outsideColor"] = (void*) new Vector4;
	quitButtonEntry.second["padding"] = (void*) new float;
	quitButtonEntry.second["border"] = (void*) new float;
	quitButtonEntry.second["containerTimer"] = NULL;
	quitButtonEntry.second["textTimer"] = NULL;
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
	
	backButtonEntry.first = "button";
	backButtonEntry.second["metrics"] = (void*) new UIMetrics;
	backButtonEntry.second["fontSize"] = (void*) new float;
	backButtonEntry.second["fontColor"] = (void*) new Vector4;
	backButtonEntry.second["text"] = (void*) new std::string;
	backButtonEntry.second["insideColor"] = (void*) new Vector4;
	backButtonEntry.second["highlightColor"] = (void*) new Vector4;
	backButtonEntry.second["borderColor"] = (void*) new Vector4;
	backButtonEntry.second["outsideColor"] = (void*) new Vector4;
	backButtonEntry.second["padding"] = (void*) new float;
	backButtonEntry.second["border"] = (void*) new float;
	backButtonEntry.second["containerTimer"] = NULL;
	backButtonEntry.second["textTimer"] = NULL;
	backButtonZoneListener = new MouseZoneListener();
	backButtonClickListener = new MouseButtonListener();

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
	resetHighScoresEntry.second["insideColor"] = (void*) new Vector4;
	resetHighScoresEntry.second["highlightColor"] = (void*) new Vector4;
	resetHighScoresEntry.second["borderColor"] = (void*) new Vector4;
	resetHighScoresEntry.second["outsideColor"] = (void*) new Vector4;
	resetHighScoresEntry.second["padding"] = (void*) new float;
	resetHighScoresEntry.second["border"] = (void*) new float;
	resetHighScoresEntry.second["containerTimer"] = NULL;
	resetHighScoresEntry.second["textTimer"] = NULL;
	resetHighScoresButtonZoneListener = new MouseZoneListener();
	resetHighScoresButtonClickListener = new MouseButtonListener();

	std::vector<SDLKey> highScoresMenuKeys;
	highScoresMenuKeys.push_back(SDLK_UP);
	highScoresMenuKeys.push_back(SDLK_DOWN);
	highScoresMenuKeys.push_back(SDLK_ESCAPE);
	highScoresMenuKeys.push_back(SDLK_RETURN);
	highScoresMenuKeyListener = new KeyListener(highScoresMenuKeys);

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
	playingKeys.push_back(SDLK_TAB);
	playingKeys.push_back(SDLK_ESCAPE);
	playingKeys.push_back(SDLK_BACKQUOTE);
	playingKeyListener = new KeyListener(playingKeys);

	cameraAheadKeyListener = new KeyAbsoluteListener(SDLK_SPACE);
	cameraUpKeyListener = new KeyAbsoluteListener(SDLK_UP);
	cameraDownKeyListener = new KeyAbsoluteListener(SDLK_DOWN);
	cameraLeftKeyListener = new KeyAbsoluteListener(SDLK_LEFT);
	cameraRightKeyListener = new KeyAbsoluteListener(SDLK_RIGHT);

	scoreLabel.first = "label";
	scoreLabel.second["metrics"] = (void*) new UIMetrics;
	scoreLabel.second["fontSize"] = (void*) new float;
	scoreLabel.second["fontColor"] = (void*) new Vector4;
	scoreLabel.second["text"] = (void*) new std::string;

	healthGaugeImage.first = "texture";
	healthGaugeImage.second["metrics"] = (void*) new UIMetrics;
	healthGaugeImage.second["texture"] = (void*) new std::string;

	healthGaugeBar.first = "progressBar";
	healthGaugeBar.second["metrics"] = (void*) new UIMetrics;
	healthGaugeBar.second["color1Top"] = (void*) new Vector4;
	healthGaugeBar.second["color1Bottom"] = (void*) new Vector4;
	healthGaugeBar.second["color2Top"] = (void*) new Vector4;
	healthGaugeBar.second["color2Bottom"] = (void*) new Vector4;
	healthGaugeBar.second["progression"] = (void*) new float;
	healthGaugeBar.second["size"] = (void*) new Vector2;

	ammoGaugeImage.first = "texture";
	ammoGaugeImage.second["metrics"] = (void*) new UIMetrics;
	ammoGaugeImage.second["texture"] = (void*) new std::string;

	ammoGaugeBar.first = "progressBar";
	ammoGaugeBar.second["metrics"] = (void*) new UIMetrics;
	ammoGaugeBar.second["color1Top"] = (void*) new Vector4;
	ammoGaugeBar.second["color1Bottom"] = (void*) new Vector4;
	ammoGaugeBar.second["color2Top"] = (void*) new Vector4;
	ammoGaugeBar.second["color2Bottom"] = (void*) new Vector4;
	ammoGaugeBar.second["progression"] = (void*) new float;
	ammoGaugeBar.second["size"] = (void*) new Vector2;

	shockGaugeImage.first = "texture";
	shockGaugeImage.second["metrics"] = (void*) new UIMetrics;
	shockGaugeImage.second["texture"] = (void*) new std::string;

	shockGaugeBar.first = "progressBar";
	shockGaugeBar.second["metrics"] = (void*) new UIMetrics;
	shockGaugeBar.second["color1Top"] = (void*) new Vector4;
	shockGaugeBar.second["color1Bottom"] = (void*) new Vector4;
	shockGaugeBar.second["color2Top"] = (void*) new Vector4;
	shockGaugeBar.second["color2Bottom"] = (void*) new Vector4;
	shockGaugeBar.second["progression"] = (void*) new float;
	shockGaugeBar.second["size"] = (void*) new Vector2;

	skyEntry.first = "skyRenderer";

	waterEntry.first = "waterRenderer";

	terrainEntry.first = "terrainRenderer";

	shipEntry.first = "shipRenderer";

	towerEntry.first = "towerRenderer";

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
	gameGraphics->execute();

	// clear the motion listener
	inputHandler->execute();
	mouseMotionListener->wasMoved();

currentScheme = SCHEME_LOADING;
reScheme();
gameGraphics->execute();
gameState = new GameState();
mainLoopModules[gameState] = 0;
gameGraphics->currentCamera = &orbitCamera;
mainLoopModules[gameGraphics] = 0;
((TerrainRenderer*) gameGraphics->drawers["terrainRenderer"])->reloadGraphics();
SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
currentScheme = SCHEME_PLAYING;
activeMenuSelection = NULL;
reScheme();
SDL_LockAudio();
gameAudio->setBackgroundMusic("playingSong");
SDL_UnlockAudio();

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

	case SCHEME_SETTINGS:
		Schemes::settingsScheme();
		break;
		
	case SCHEME_HELP:
		Schemes::helpScheme();
		break;

	case SCHEME_HIGHSCORES:
		Schemes::highScoresScheme();
		break;

	case SCHEME_LOADING:
		Schemes::loadingScheme();
		break;

	case SCHEME_PLAYING:
		Schemes::playingScheme();
		break;

	case SCHEME_WELCOME:
		SDL_ShowCursor(1);

		Schemes::welcomeScheme(
				*((std::string*) joinCallsignFieldEntry.second["text"])
			);

		break;
	case SCHEME_DASHBOARD:
		SDL_ShowCursor(1);

		Schemes::dashboardScheme();

		break;
	}
((UIMetrics*) fpsEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(fpsEntry.second);
uiLayoutAuthority->metrics.push_back((UIMetrics*) fpsEntry.second["metrics"]);
drawStack.push_back(fpsEntry);
uiLayoutAuthority->rearrange();
}

unsigned int GameLogic::execute() {
	bool needRedraw = false;

	// independent logic
	if(quitKeyListener->popKey() != SDLK_UNKNOWN )
		keepDominicusAlive = false;
	if(fullScreenKeyListener->popKey() != SDLK_UNKNOWN) {
		bool fullScreenGraphics = ! gameGraphics->fullScreen;

		bool isInMainLoopModules = (mainLoopModules.find(gameGraphics) != mainLoopModules.end());
		if(isInMainLoopModules)
			mainLoopModules.erase(mainLoopModules.find(gameGraphics));

		delete(gameGraphics);

		Camera* currentCamera = gameGraphics->currentCamera;
		gameGraphics = new GameGraphics(fullScreenGraphics);
		gameGraphics->currentCamera = currentCamera;

		if(isInMainLoopModules)
			mainLoopModules[gameGraphics] = 0;

		if(currentScheme == SCHEME_PLAYING) {
			((TerrainRenderer*) gameGraphics->drawers["terrainRenderer"])->reloadGraphics();
//			((DrawRadar*) gameGraphics->drawers["radar"])->reloadGraphics();
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
			gameGraphics->execute();

			gameState = new GameState();
			mainLoopModules[gameState] = 0;

			mainLoopModules[gameGraphics] = 0;
			((TerrainRenderer*) gameGraphics->drawers["terrainRenderer"])->reloadGraphics();
//			((DrawRadar*) gameGraphics->drawers["radar"])->reloadGraphics();
			gameGraphics->currentCamera = &orbitCamera;

			SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);

			currentScheme = SCHEME_PLAYING;
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
					gameGraphics->execute();

					gameState = new GameState();
					mainLoopModules[gameState] = 0;

					mainLoopModules[gameGraphics] = 0;
					((TerrainRenderer*) gameGraphics->drawers["terrainRenderer"])->reloadGraphics();
//					((DrawRadar*) gameGraphics->drawers["radar"])->reloadGraphics();
					gameGraphics->currentCamera = &orbitCamera;
					SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);

					currentScheme = SCHEME_PLAYING;
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
	} else if(currentScheme == SCHEME_PLAYING) {
		// key hits
		for(SDLKey key = playingKeyListener->popKey(); key != SDLK_UNKNOWN; key = playingKeyListener->popKey()) {
			if(key == SDLK_RETURN) {
				mainLoopModules.erase(mainLoopModules.find(gameState));
				delete(gameState);
				gameState = new GameState();
				mainLoopModules[gameState] = 0;

				((TerrainRenderer*) gameGraphics->drawers["terrainRenderer"])->reloadGraphics();
			} else if(key == SDLK_TAB) {
				if(gameGraphics->currentCamera == &orbitCamera)
					gameGraphics->currentCamera = &presentationCamera;
				else if(gameGraphics->currentCamera == &presentationCamera)
					gameGraphics->currentCamera = &roamingCamera;
				else if(gameGraphics->currentCamera == &roamingCamera)
					gameGraphics->currentCamera = &orbitCamera;
			} else if(key == SDLK_ESCAPE) {
				mainLoopModules.erase(mainLoopModules.find(gameState));
				mainLoopModules.erase(mainLoopModules.find(gameGraphics));

				delete(gameState);

				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				reScheme();
				needRedraw = true;

				SDL_LockAudio();
				gameAudio->setBackgroundMusic("menuSong");
				SDL_UnlockAudio();
			} else if(key == SDLK_BACKQUOTE) {
				if(gameState->isPaused)
					gameState->resume();
				else
					gameState->pause();
			}
		}

		// keys down
		if(cameraLeftKeyListener->isDown)
			roamingCamera.rotationX += 0.5f;
		if(cameraRightKeyListener->isDown)
			roamingCamera.rotationX -= 0.5f;
		if(cameraUpKeyListener->isDown)
			roamingCamera.rotationY += 0.5f;
		if(cameraDownKeyListener->isDown)
			roamingCamera.rotationY -= 0.5f;
		if(cameraAheadKeyListener->isDown) {
			Matrix3 directionMatrix; directionMatrix.identity();
			rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(roamingCamera.rotationX), directionMatrix);
			rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(roamingCamera.rotationY), directionMatrix);
			directionMatrix.transpose();
			Vector3 movement(0.0f, 0.0f, 3.0f);
			roamingCamera.position = roamingCamera.position + movement * directionMatrix;
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
	} else if(currentScheme == SCHEME_HELP) {
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
		for(SDLKey key = helpMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = helpMenuKeyListener->popKey()) {
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
	}
/*
	// see if we need to toggle the dashboard
	if(dashboardKeyListener->popKey() != SDLK_UNKNOWN) {
		if(currentScheme == SCHEME_PLAYING)
			currentScheme = SCHEME_DASHBOARD;
		else
			currentScheme = SCHEME_PLAYING;

		hintExpiration = platform->getExecMills() + 10000;

		reScheme();
	}
	if (currentScheme == SCHEME_MAINMENU) {
	} else if(currentScheme == SCHEME_WELCOME) {
		// update text field
		if(inputHandler->keyboard->unicodeChars.length() > 0) {
			*((std::string*) joinCallsignFieldEntry.second["text"]) += inputHandler->keyboard->unicodeChars;
			inputHandler->keyboard->unicodeChars.erase();

			reScheme();
			needRedraw = true;
		}

		// see if we need to delete text
		if(deleteKeyListener->popKey() != SDLK_UNKNOWN) {
			if(((std::string*) joinCallsignFieldEntry.second["text"])->length() > 0) {
				*((std::string*) joinCallsignFieldEntry.second["text"]) =
						((std::string*) joinCallsignFieldEntry.second["text"])->substr(0,
								((std::string*) joinCallsignFieldEntry.second["text"])->length() - 1);

				reScheme();
				needRedraw = true;
			}
		}

		}
	} else if (currentScheme == SCHEME_PLAYING) {
		// cursor movement and ship steering
		if(cursorMovementListener->wasMoved()) {
			Vector2 zeroVec(0.0f, 0.0f);
			Vector2 posVec(
					inputHandler->mouse->position.x * gameGraphics->aspectRatio,
					inputHandler->mouse->position.y
				);

			posVec.x /= gameSystem->getFloat("hudControlBoxSize") / 100.0f;
			posVec.y /= gameSystem->getFloat("hudControlBoxSize") / 100.0f;

			if(posVec.x > 1.0f) posVec.x = 1.0f;
			else if(posVec.x < -1.0f) posVec.x = -1.0f;
			if(posVec.y > 1.0f) posVec.y = 1.0f;
			else if(posVec.y < -1.0f) posVec.y = -1.0f;

			if(distance(zeroVec, posVec) > gameSystem->getFloat("hudControlBoxSpotSize") / 100.0f) {
				gameState->ships[0]->steering.x = posVec.x;
				gameState->ships[0]->steering.y = -posVec.y;
			} else {
				gameState->ships[0]->steering = Vector2(0.0f, 0.0f);
			}
		}

		// test key
		if(testKeyListener->popKey() != SDLK_UNKNOWN) {
//			rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(-3.0f), *((Matrix4*) terrainEntry.second["mvpMatrix"]));
//			printf("T key press\n");
//while(1) ;
			delete(gameState->world);
			gameState->world = new World();
			((TerrainRenderer*) gameGraphics->drawers["terrainRenderer"])->reloadGraphics();
			((DrawRadar*) gameGraphics->drawers["radar"])->reloadGraphics();

			reScheme();
		}
		if(testKeyAbsoluteListener->isDown) {
//			printf("R key down\n");
			if((gameState->ships[0])->speed + 0.1f < gameSystem->getFloat("shipMaxSpeed"))
//				(gameState->ships[0])->speed = gameSystem->getFloat("shipMaxSpeed");
				(gameState->ships[0])->speed += 0.1f;
		} else {
			if((gameState->ships[0])->speed - 0.1f > 0.0f)
			(gameState->ships[0])->speed -= 0.1f;
		}


		// update buttons
		if(
				quitButtonZoneListener->wasChanged()
			) {
			reScheme();
		}
	}

*/

	// see if we need to redraw graphics, if we're still in the menus
	if(needRedraw && mainLoopModules.find(gameGraphics) == mainLoopModules.end())
		gameGraphics->execute();

	// calculate and return sleep time from superclass
	unsigned int frequency = (unsigned int) gameSystem->getFloat("inputPollingFrequency");
	static const unsigned int idealSleepTime = (
			frequency  != 0 ?
			1000 / frequency : 0
		);
	return getSleepTime(idealSleepTime);
}
