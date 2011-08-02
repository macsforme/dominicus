/*
 * GameLogic.cpp
 * Dominicus
 *
 * Copyright 2010-2011, Joshua Bodine
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "logic/GameLogic.h"

GameLogic::GameLogic() {
	// instantiate HUD layout authority
	uiLayoutAuthority = new UILayoutAuthority(
			Vector2(gameSystem->getFloat("hudElementMargin") / gameGraphics->resolutionX,
					gameSystem->getFloat("hudElementMargin") / gameGraphics->resolutionY)
		);

	// add key listeners
	std::vector<SDLKey> keysVector;

	keysVector = gameSystem->getBindingKeys("quit");
	quitKeyListener = new KeyListener(keysVector);
	keysVector.clear();

	keysVector = gameSystem->getBindingKeys("fullScreenToggle");
	fullScreenKeyListener = new KeyListener(keysVector);
	keysVector.clear();

	keysVector = gameSystem->getBindingKeys("dashboardToggle");
	dashboardKeyListener = new KeyListener(keysVector);
	keysVector.clear();

	keysVector.push_back(SDLK_BACKSPACE);
	deleteKeyListener = new KeyListener(keysVector);
	keysVector.clear();

	keysVector.push_back(SDLK_t);
	testKeyListener = new KeyListener(keysVector);
	keysVector.clear();

	testKeyAbsoluteListener = new KeyAbsoluteListener(SDLK_SPACE);

	// add mouse listeners
	cursorMovementListener = new MouseMotionListener();
	quitButtonZoneListener = new MouseZoneListener();
	quitButtonClickListener = new MouseButtonListener();
	startButtonZoneListener = new MouseZoneListener();
	startButtonClickListener = new MouseButtonListener();

	// specify draw stack entry draw node types and initialize memory
	splashEntry.first = "splash";

	titleEntry.first = "label";
	titleEntry.second["metrics"] = (void*) new UIMetrics;
	titleEntry.second["fontSize"] = (void*) new float;
	titleEntry.second["fontColor"] = (void*) new Vector4;
	titleEntry.second["text"] = (void*) new std::string;

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

	logoEntry.first = "texture";
	logoEntry.second["metrics"] = (void*) new UIMetrics;
	logoEntry.second["texture"] = (void*) new std::string;

	joinContainerEntry.first = "container";
	joinContainerEntry.second["metrics"] = (void*) new UIMetrics;
	joinContainerEntry.second["insideColor"] = (void*) new Vector4;
	joinContainerEntry.second["highlightColor"] = (void*) new Vector4;
	joinContainerEntry.second["borderColor"] = (void*) new Vector4;
	joinContainerEntry.second["outsideColor"] = (void*) new Vector4;
	joinContainerEntry.second["padding"] = (void*) new float;
	joinContainerEntry.second["border"] = (void*) new float;
	joinContainerEntry.second["containerTimer"] = NULL;
	joinContainerEntry.second["textTimer"] = NULL;

	joinHeaderLabelEntry.first = "label";
	joinHeaderLabelEntry.second["metrics"] = (void*) new UIMetrics;
	joinHeaderLabelEntry.second["fontSize"] = (void*) new float;
	joinHeaderLabelEntry.second["fontColor"] = (void*) new Vector4;
	joinHeaderLabelEntry.second["text"] = (void*) new std::string;

	joinCallsignLabelEntry.first = "label";
	joinCallsignLabelEntry.second["metrics"] = (void*) new UIMetrics;
	joinCallsignLabelEntry.second["fontSize"] = (void*) new float;
	joinCallsignLabelEntry.second["fontColor"] = (void*) new Vector4;
	joinCallsignLabelEntry.second["text"] = (void*) new std::string;

	joinCallsignFieldEntry.first = "field";
	joinCallsignFieldEntry.second["metrics"] = (void*) new UIMetrics;
	joinCallsignFieldEntry.second["fontSize"] = (void*) new float;
	joinCallsignFieldEntry.second["fontColor"] = (void*) new Vector4;
	joinCallsignFieldEntry.second["text"] = (void*) new std::string;
	joinCallsignFieldEntry.second["boxColor"] = (void*) new Vector4;
	joinCallsignFieldEntry.second["boxPadding"] = (void*) new float;

	joinTeamLabelEntry.first = "label";
	joinTeamLabelEntry.second["metrics"] = (void*) new UIMetrics;
	joinTeamLabelEntry.second["fontSize"] = (void*) new float;
	joinTeamLabelEntry.second["fontColor"] = (void*) new Vector4;
	joinTeamLabelEntry.second["text"] = (void*) new std::string;

	joinTeamMenuEntry.first = "menu";

	joinButtonEntry.first = "button";
	joinButtonEntry.second["metrics"] = (void*) new UIMetrics;
	joinButtonEntry.second["fontSize"] = (void*) new float;
	joinButtonEntry.second["fontColor"] = (void*) new Vector4;
	joinButtonEntry.second["text"] = (void*) new std::string;
	joinButtonEntry.second["insideColor"] = (void*) new Vector4;
	joinButtonEntry.second["highlightColor"] = (void*) new Vector4;
	joinButtonEntry.second["borderColor"] = (void*) new Vector4;
	joinButtonEntry.second["outsideColor"] = (void*) new Vector4;
	joinButtonEntry.second["padding"] = (void*) new float;
	joinButtonEntry.second["border"] = (void*) new float;
	joinButtonEntry.second["containerTimer"] = NULL;
	joinButtonEntry.second["textTimer"] = NULL;

	joinStatusLabelEntry.first = "label";
	joinStatusLabelEntry.second["metrics"] = (void*) new UIMetrics;
	joinStatusLabelEntry.second["fontSize"] = (void*) new float;
	joinStatusLabelEntry.second["fontColor"] = (void*) new Vector4;
	joinStatusLabelEntry.second["text"] = (void*) new std::string;

	welcomeHelpEntry.first = "button";
	welcomeHelpEntry.second["metrics"] = (void*) new UIMetrics;
	welcomeHelpEntry.second["fontSize"] = (void*) new float;
	welcomeHelpEntry.second["fontColor"] = (void*) new Vector4;
	welcomeHelpEntry.second["text"] = (void*) new std::string;
	welcomeHelpEntry.second["wrap"] = (void*) new float;
	welcomeHelpEntry.second["insideColor"] = (void*) new Vector4;
	welcomeHelpEntry.second["highlightColor"] = (void*) new Vector4;
	welcomeHelpEntry.second["borderColor"] = (void*) new Vector4;
	welcomeHelpEntry.second["outsideColor"] = (void*) new Vector4;
	welcomeHelpEntry.second["padding"] = (void*) new float;
	welcomeHelpEntry.second["border"] = (void*) new float;
	welcomeHelpEntry.second["containerTimer"] = NULL;
	welcomeHelpEntry.second["textTimer"] = NULL;

	consoleEntry.first = "button";
	consoleEntry.second["metrics"] = (void*) new UIMetrics;
	consoleEntry.second["fontSize"] = (void*) new float;
	consoleEntry.second["fontColor"] = (void*) new Vector4;
	consoleEntry.second["text"] = (void*) new std::string;
	consoleEntry.second["wrap"] = (void*) new float;
	consoleEntry.second["insideColor"] = (void*) new Vector4;
	consoleEntry.second["highlightColor"] = (void*) new Vector4;
	consoleEntry.second["borderColor"] = (void*) new Vector4;
	consoleEntry.second["outsideColor"] = (void*) new Vector4;
	consoleEntry.second["padding"] = (void*) new float;
	consoleEntry.second["border"] = (void*) new float;
	consoleEntry.second["containerTimer"] = NULL;
	consoleEntry.second["textTimer"] = NULL;

	playerInfoEntry.first = "button";
	playerInfoEntry.second["metrics"] = (void*) new UIMetrics;
	playerInfoEntry.second["fontSize"] = (void*) new float;
	playerInfoEntry.second["fontColor"] = (void*) new Vector4;
	playerInfoEntry.second["text"] = (void*) new std::string;
	playerInfoEntry.second["wrap"] = (void*) new float;
	playerInfoEntry.second["insideColor"] = (void*) new Vector4;
	playerInfoEntry.second["highlightColor"] = (void*) new Vector4;
	playerInfoEntry.second["borderColor"] = (void*) new Vector4;
	playerInfoEntry.second["outsideColor"] = (void*) new Vector4;
	playerInfoEntry.second["padding"] = (void*) new float;
	playerInfoEntry.second["border"] = (void*) new float;
	playerInfoEntry.second["containerTimer"] = NULL;
	playerInfoEntry.second["textTimer"] = NULL;

	debugInfoEntry.first = "button";
	debugInfoEntry.second["metrics"] = (void*) new UIMetrics;
	debugInfoEntry.second["fontSize"] = (void*) new float;
	debugInfoEntry.second["fontColor"] = (void*) new Vector4;
	debugInfoEntry.second["text"] = (void*) new std::string;
	debugInfoEntry.second["wrap"] = (void*) new float;
	debugInfoEntry.second["insideColor"] = (void*) new Vector4;
	debugInfoEntry.second["highlightColor"] = (void*) new Vector4;
	debugInfoEntry.second["borderColor"] = (void*) new Vector4;
	debugInfoEntry.second["outsideColor"] = (void*) new Vector4;
	debugInfoEntry.second["padding"] = (void*) new float;
	debugInfoEntry.second["border"] = (void*) new float;
	debugInfoEntry.second["containerTimer"] = NULL;
	debugInfoEntry.second["textTimer"] = NULL;

	grayOutEntry.first = "grayOut";
	grayOutEntry.second["color"] = (void*) new Vector4;

	controlsHelpEntry.first = "button";
	controlsHelpEntry.second["metrics"] = (void*) new UIMetrics;
	controlsHelpEntry.second["fontSize"] = (void*) new float;
	controlsHelpEntry.second["fontColor"] = (void*) new Vector4;
	controlsHelpEntry.second["text"] = (void*) new std::string;
	controlsHelpEntry.second["wrap"] = (void*) new float;
	controlsHelpEntry.second["insideColor"] = (void*) new Vector4;
	controlsHelpEntry.second["highlightColor"] = (void*) new Vector4;
	controlsHelpEntry.second["borderColor"] = (void*) new Vector4;
	controlsHelpEntry.second["outsideColor"] = (void*) new Vector4;
	controlsHelpEntry.second["padding"] = (void*) new float;
	controlsHelpEntry.second["border"] = (void*) new float;
	controlsHelpEntry.second["containerTimer"] = NULL;
	controlsHelpEntry.second["textTimer"] = NULL;

	scoreboardEntry.first = "button";
	scoreboardEntry.second["metrics"] = (void*) new UIMetrics;
	scoreboardEntry.second["fontSize"] = (void*) new float;
	scoreboardEntry.second["fontColor"] = (void*) new Vector4;
	scoreboardEntry.second["text"] = (void*) new std::string;
	scoreboardEntry.second["wrap"] = (void*) new float;
	scoreboardEntry.second["insideColor"] = (void*) new Vector4;
	scoreboardEntry.second["highlightColor"] = (void*) new Vector4;
	scoreboardEntry.second["borderColor"] = (void*) new Vector4;
	scoreboardEntry.second["outsideColor"] = (void*) new Vector4;
	scoreboardEntry.second["padding"] = (void*) new float;
	scoreboardEntry.second["border"] = (void*) new float;
	scoreboardEntry.second["containerTimer"] = NULL;
	scoreboardEntry.second["textTimer"] = NULL;

	controlBoxEntry.first = "controlBox";
	controlBoxEntry.second["spotSize"] = (void*) new float;
	controlBoxEntry.second["boxSize"] = (void*) new float;
	controlBoxEntry.second["color"] = (void*) new Vector4;

	cursorEntry.first = "cursor";
	cursorEntry.second["size"] = (void*) new float;
	cursorEntry.second["thickness"] = (void*) new float;
	cursorEntry.second["color"] = (void*) new Vector4;
	cursorEntry.second["position"] = (void*) new Vector2*;

	hintEntry.first = "button";
	hintEntry.second["metrics"] = (void*) new UIMetrics;
	hintEntry.second["fontSize"] = (void*) new float;
	hintEntry.second["fontColor"] = (void*) new Vector4;
	hintEntry.second["text"] = (void*) new std::string;
	hintEntry.second["wrap"] = (void*) new float;
	hintEntry.second["insideColor"] = (void*) new Vector4;
	hintEntry.second["highlightColor"] = (void*) new Vector4;
	hintEntry.second["borderColor"] = (void*) new Vector4;
	hintEntry.second["outsideColor"] = (void*) new Vector4;
	hintEntry.second["padding"] = (void*) new float;
	hintEntry.second["border"] = (void*) new float;
	hintEntry.second["containerTimer"] = &(gameGraphics->hardTimer);
	hintEntry.second["textTimer"] = &(gameGraphics->hardTimer);

	radarContainerEntry.first = "container";
	radarContainerEntry.second["metrics"] = (void*) new UIMetrics;
	radarContainerEntry.second["insideColor"] = (void*) new Vector4;
	radarContainerEntry.second["highlightColor"] = (void*) new Vector4;
	radarContainerEntry.second["borderColor"] = (void*) new Vector4;
	radarContainerEntry.second["outsideColor"] = (void*) new Vector4;
	radarContainerEntry.second["padding"] = (void*) new float;
	radarContainerEntry.second["border"] = (void*) new float;
	radarContainerEntry.second["containerTimer"] = NULL;
	radarContainerEntry.second["textTimer"] = NULL;

	radarEntry.first = "radar";
	radarEntry.second["metrics"] = (void*) new UIMetrics;

	ship1Entry.first = "shipRenderer";

	terrain1Entry.first = "terrainRenderer";

	waterEntry.first = "waterRenderer";

	// set initial logic
	lastClockUpdate = 0;
	myCallsign = "";
	lastFrameTime = 0;
	hintExpiration = 0;

	// bad programming practice, but set the global variable to us since
	// we should only exist once, and both gameGraphics and schemes need
	// to find us
	gameLogic = this;

	// build the initial draw stack
	currentScheme = SCHEME_WELCOME;
	reScheme();

	// draw the initial frame
	gameGraphics->execute();

//*((std::string*) joinCallsignFieldEntry.second["text"]) = "Augustus";
//startButtonClickListener->buttonHit = true;
}

GameLogic::~GameLogic() {
	// remove key listeners
	inputHandler->keyboard->removeListener(quitKeyListener);
	delete(quitKeyListener);
	inputHandler->keyboard->removeListener(fullScreenKeyListener);
	delete(fullScreenKeyListener);
	inputHandler->keyboard->removeListener(dashboardKeyListener);
	delete(dashboardKeyListener);
	inputHandler->keyboard->removeListener(deleteKeyListener);
	delete(deleteKeyListener);
	inputHandler->keyboard->removeListener(testKeyListener);
	delete(testKeyListener);

	// remove mouse listeners
	inputHandler->mouse->removeListener(cursorMovementListener);
	delete(cursorMovementListener);

	inputHandler->mouse->removeListener(quitButtonClickListener);
	delete(quitButtonClickListener);

	inputHandler->mouse->removeListener(quitButtonZoneListener);
	delete(quitButtonZoneListener);

	inputHandler->mouse->removeListener(startButtonZoneListener);
	delete(startButtonZoneListener);

	inputHandler->mouse->removeListener(startButtonClickListener);
	delete(startButtonClickListener);

	// free memory allocated in drawing stack entries
	delete((UIMetrics*) titleEntry.second["metrics"]);
	delete((float*) titleEntry.second["fontSize"]);
	delete((Vector4*) titleEntry.second["fontColor"]);
	delete((std::string*) titleEntry.second["text"]);

	delete((UIMetrics*) quitButtonEntry.second["metrics"]);
	delete((float*) quitButtonEntry.second["fontSize"]);
	delete((Vector4*) quitButtonEntry.second["fontColor"]);
	delete((std::string*) quitButtonEntry.second["text"]);
	delete((Vector4*) quitButtonEntry.second["insideColor"]);
	delete((Vector4*) quitButtonEntry.second["highlightColor"]);
	delete((Vector4*) quitButtonEntry.second["borderColor"]);
	delete((Vector4*) quitButtonEntry.second["outsideColor"]);
	delete((float*) quitButtonEntry.second["padding"]);
	delete((float*) quitButtonEntry.second["border"]);

	delete((UIMetrics*) logoEntry.second["metrics"]);
	delete((std::string*) logoEntry.second["texture"]);

	delete((UIMetrics*) joinContainerEntry.second["metrics"]);
	delete((Vector4*) joinContainerEntry.second["insideColor"]);
	delete((Vector4*) joinContainerEntry.second["highLightColor"]);
	delete((Vector4*) joinContainerEntry.second["borderColor"]);
	delete((Vector4*) joinContainerEntry.second["outsideColor"]);
	delete((float*) joinContainerEntry.second["padding"]);
	delete((float*) joinContainerEntry.second["border"]);

	delete((UIMetrics*) joinHeaderLabelEntry.second["metrics"]);
	delete((float*) joinHeaderLabelEntry.second["fontSize"]);
	delete((Vector4*) joinHeaderLabelEntry.second["fontColor"]);
	delete((std::string*) joinHeaderLabelEntry.second["text"]);

	delete((UIMetrics*) joinCallsignLabelEntry.second["metrics"]);
	delete((float*) joinCallsignLabelEntry.second["fontSize"]);
	delete((Vector4*) joinCallsignLabelEntry.second["fontColor"]);
	delete((std::string*) joinCallsignLabelEntry.second["text"]);

	delete((UIMetrics*) joinTeamLabelEntry.second["metrics"]);
	delete((float*) joinTeamLabelEntry.second["fontSize"]);
	delete((Vector4*) joinTeamLabelEntry.second["fontColor"]);
	delete((std::string*) joinTeamLabelEntry.second["text"]);

	delete ((UIMetrics*) joinButtonEntry.second["metrics"]);
	delete ((float*) joinButtonEntry.second["fontSize"]);
	delete ((Vector4*) joinButtonEntry.second["fontColor"]);
	delete ((std::string*) joinButtonEntry.second["text"]);
	delete ((Vector4*) joinButtonEntry.second["insideColor"]);
	delete ((Vector4*) joinButtonEntry.second["highlightColor"]);
	delete ((Vector4*) joinButtonEntry.second["borderColor"]);
	delete ((Vector4*) joinButtonEntry.second["outsideColor"]);
	delete ((float*) joinButtonEntry.second["padding"]);
	delete ((float*) joinButtonEntry.second["border"]);

	delete((UIMetrics*) joinStatusLabelEntry.second["metrics"]);
	delete((float*) joinStatusLabelEntry.second["fontSize"]);
	delete((Vector4*) joinStatusLabelEntry.second["fontColor"]);
	delete((std::string*) joinStatusLabelEntry.second["text"]);

	delete((UIMetrics*) welcomeHelpEntry.second["metrics"]);
	delete((float*) welcomeHelpEntry.second["fontSize"]);
	delete((Vector4*) welcomeHelpEntry.second["fontColor"]);
	delete((std::string*) welcomeHelpEntry.second["text"]);
	delete((float*) welcomeHelpEntry.second["wrap"]);
	delete((Vector4*) welcomeHelpEntry.second["insideColor"]);
	delete((Vector4*) welcomeHelpEntry.second["highLightColor"]);
	delete((Vector4*) welcomeHelpEntry.second["borderColor"]);
	delete((Vector4*) welcomeHelpEntry.second["outsideColor"]);
	delete((float*) welcomeHelpEntry.second["padding"]);
	delete((float*) welcomeHelpEntry.second["border"]);

	delete((UIMetrics*) consoleEntry.second["metrics"]);
	delete((float*) consoleEntry.second["fontSize"]);
	delete((Vector4*) consoleEntry.second["fontColor"]);
	delete((std::string*) consoleEntry.second["text"]);
	delete((float*) consoleEntry.second["wrap"]);
	delete((Vector4*) consoleEntry.second["insideColor"]);
	delete((Vector4*) consoleEntry.second["highLightColor"]);
	delete((Vector4*) consoleEntry.second["borderColor"]);
	delete((Vector4*) consoleEntry.second["outsideColor"]);
	delete((float*) consoleEntry.second["padding"]);
	delete((float*) consoleEntry.second["border"]);

	delete((UIMetrics*) playerInfoEntry.second["metrics"]);
	delete((float*) playerInfoEntry.second["fontSize"]);
	delete((Vector4*) playerInfoEntry.second["fontColor"]);
	delete((std::string*) playerInfoEntry.second["text"]);
	delete((float*) playerInfoEntry.second["wrap"]);
	delete((Vector4*) playerInfoEntry.second["insideColor"]);
	delete((Vector4*) playerInfoEntry.second["highLightColor"]);
	delete((Vector4*) playerInfoEntry.second["borderColor"]);
	delete((Vector4*) playerInfoEntry.second["outsideColor"]);
	delete((float*) playerInfoEntry.second["padding"]);
	delete((float*) playerInfoEntry.second["border"]);

	delete((UIMetrics*) debugInfoEntry.second["metrics"]);
	delete((float*) debugInfoEntry.second["fontSize"]);
	delete((Vector4*) debugInfoEntry.second["fontColor"]);
	delete((std::string*) debugInfoEntry.second["text"]);
	delete((float*) debugInfoEntry.second["wrap"]);
	delete((Vector4*) debugInfoEntry.second["insideColor"]);
	delete((Vector4*) debugInfoEntry.second["highLightColor"]);
	delete((Vector4*) debugInfoEntry.second["borderColor"]);
	delete((Vector4*) debugInfoEntry.second["outsideColor"]);
	delete((float*) debugInfoEntry.second["padding"]);
	delete((float*) debugInfoEntry.second["border"]);

	delete((Vector4*) grayOutEntry.second["color"]);

	delete((UIMetrics*) controlsHelpEntry.second["metrics"]);
	delete((float*) controlsHelpEntry.second["fontSize"]);
	delete((Vector4*) controlsHelpEntry.second["fontColor"]);
	delete((std::string*) controlsHelpEntry.second["text"]);
	delete((float*) controlsHelpEntry.second["wrap"]);
	delete((Vector4*) controlsHelpEntry.second["insideColor"]);
	delete((Vector4*) controlsHelpEntry.second["highLightColor"]);
	delete((Vector4*) controlsHelpEntry.second["borderColor"]);
	delete((Vector4*) controlsHelpEntry.second["outsideColor"]);
	delete((float*) controlsHelpEntry.second["padding"]);
	delete((float*) controlsHelpEntry.second["border"]);

	delete((UIMetrics*) scoreboardEntry.second["metrics"]);
	delete((float*) scoreboardEntry.second["fontSize"]);
	delete((Vector4*) scoreboardEntry.second["fontColor"]);
	delete((std::string*) scoreboardEntry.second["text"]);
	delete((float*) scoreboardEntry.second["wrap"]);
	delete((Vector4*) scoreboardEntry.second["insideColor"]);
	delete((Vector4*) scoreboardEntry.second["highLightColor"]);
	delete((Vector4*) scoreboardEntry.second["borderColor"]);
	delete((Vector4*) scoreboardEntry.second["outsideColor"]);
	delete((float*) scoreboardEntry.second["padding"]);
	delete((float*) scoreboardEntry.second["border"]);

	delete((float*) controlBoxEntry.second["spotSize"]);
	delete((float*) controlBoxEntry.second["boxSize"]);
	delete((Vector4*) controlBoxEntry.second["color"]);

	delete((float*) cursorEntry.second["size"]);
	delete((float*) cursorEntry.second["thickness"]);
	delete((Vector4*) cursorEntry.second["color"]);
	delete((Vector2**) cursorEntry.second["position"]);

	delete((UIMetrics*) hintEntry.second["metrics"]);
	delete((float*) hintEntry.second["fontSize"]);
	delete((Vector4*) hintEntry.second["fontColor"]);
	delete((std::string*) hintEntry.second["text"]);
	delete((float*) hintEntry.second["wrap"]);
	delete((Vector4*) hintEntry.second["insideColor"]);
	delete((Vector4*) hintEntry.second["highLightColor"]);
	delete((Vector4*) hintEntry.second["borderColor"]);
	delete((Vector4*) hintEntry.second["outsideColor"]);
	delete((float*) hintEntry.second["padding"]);
	delete((float*) hintEntry.second["border"]);

	delete((UIMetrics*) radarContainerEntry.second["metrics"]);
	delete((Vector4*) radarContainerEntry.second["insideColor"]);
	delete((Vector4*) radarContainerEntry.second["highLightColor"]);
	delete((Vector4*) radarContainerEntry.second["borderColor"]);
	delete((Vector4*) radarContainerEntry.second["outsideColor"]);
	delete((float*) radarContainerEntry.second["padding"]);
	delete((float*) radarContainerEntry.second["border"]);

	delete((UIMetrics*) radarEntry.second["metrics"]);
}

void GameLogic::reScheme() {
	// always clear the current stacks
	gameLogic->drawStack.clear();
	gameLogic->uiLayoutAuthority->metrics.clear();
	inputHandler->keyboard->clearListeners();
	inputHandler->mouse->clearListeners();

	// run the appropriate scheme
	switch (currentScheme) {
	case SCHEME_WELCOME:
		SDL_ShowCursor(1);

		Schemes::welcomeScheme(
				*((std::string*) joinCallsignFieldEntry.second["text"])
			);

		break;
	case SCHEME_PLAYING:
		SDL_ShowCursor(0);

		Schemes::playingScheme(
				true, //bool showHUD,
				myCallsign, //std::string callsignText,
//				"", //std::string team,
				"", //std::string scoreText,
				"", //std::string powerText,
				true //bool showDebugInfo,
			);

		break;
	case SCHEME_DASHBOARD:
		SDL_ShowCursor(1);

		Schemes::dashboardScheme();

		break;
	}
}

unsigned int GameLogic::execute() {
	bool needRedraw = false;

	// do timed events
	if(platform->getExecMills() - lastClockUpdate > 1000.0f) {
		reScheme();

		lastClockUpdate = platform->getExecMills();
	}

	if(lastFrameTime != gameGraphics->frameTime) {
		reScheme();

		lastFrameTime = gameGraphics->frameTime;
	}

	if(hintExpiration != 0 && hintExpiration <= platform->getExecMills()) {
		reScheme();

		hintExpiration = 0;
	}

	// see if we need to quit
	if(
			quitKeyListener->popKey() != SDLK_UNKNOWN ||
			quitButtonClickListener->wasClicked()
		)
		keepDominicusAlive = false;

	// see if we need to start the game
	if(startButtonClickListener->wasClicked()) {
		gameState = new GameState();

		mainLoopModules[gameState] = 0;

		gameState->ships.push_back(new Ship());
		(*(gameState->ships.begin()))->position.y = 20.0f;
		(*(gameState->ships.begin()))->position.z = -gameState->world->diameter / 2.0f;

		shipRelativeCamera = new ShipRelativeCamera(gameState->ships[0]);
		terrainRelativeCamera = new TerrainRelativeCamera(gameState->ships[0]);
		followCamera = new FollowCamera(gameState->ships[0]);

		ship1Entry.second["camera"] = terrainRelativeCamera;
		ship1Entry.second["camera"] = followCamera;
		terrain1Entry.second["camera"] = terrainRelativeCamera;
		terrain1Entry.second["camera"] = followCamera;
		waterEntry.second["camera"] = terrainRelativeCamera;
		waterEntry.second["camera"] = followCamera;

		mainLoopModules[gameGraphics] = 0;
		((TerrainRenderer*) gameGraphics->drawers["terrainRenderer"])->reloadGraphics();
		((DrawRadar*) gameGraphics->drawers["radar"])->reloadGraphics();
		gameSystem->log(GameSystem::LOG_VERBOSE, "Terrain geometry reloaded in renderer");

		SDL_WarpMouse(
				gameGraphics->resolutionX / 2,
				gameGraphics->resolutionY / 2
			);

		lastClockUpdate = platform->getExecMills();
		myCallsign = *((std::string*) joinCallsignFieldEntry.second["text"]);
		hintExpiration = platform->getExecMills() + 10000;

		currentScheme = SCHEME_PLAYING;
		reScheme();
	}

	// see if we need to toggle fullscreen
	if(fullScreenKeyListener->popKey() != SDLK_UNKNOWN) {
		bool fullScreenGraphics = ! gameGraphics->fullScreen;

		bool isInMainLoopModules = (mainLoopModules.find(gameGraphics) != mainLoopModules.end());
		if(isInMainLoopModules)
			mainLoopModules.erase(mainLoopModules.find(gameGraphics));

		delete(gameGraphics);
		gameGraphics = new GameGraphics(fullScreenGraphics);

		if(isInMainLoopModules)
			mainLoopModules[gameGraphics] = 0;

		if(currentScheme == SCHEME_PLAYING) {
			((TerrainRenderer*) gameGraphics->drawers["terrainRenderer"])->reloadGraphics();
			((DrawRadar*) gameGraphics->drawers["radar"])->reloadGraphics();

		}

		delete(uiLayoutAuthority);
		uiLayoutAuthority = new UILayoutAuthority(
				Vector2(gameSystem->getFloat("hudElementMargin") / gameGraphics->resolutionX,
						gameSystem->getFloat("hudElementMargin") / gameGraphics->resolutionY)
			);

		joinButtonEntry.second["containerTimer"] = &(gameGraphics->waveTimer);
		quitButtonEntry.second["containerTimer"] = &(gameGraphics->waveTimer);
		hintEntry.second["containerTimer"] = &(gameGraphics->hardTimer);

		reScheme();
		needRedraw = true;
	}

	// see if we need to toggle the dashboard
	if(dashboardKeyListener->popKey() != SDLK_UNKNOWN) {
		if(currentScheme == SCHEME_PLAYING)
			currentScheme = SCHEME_DASHBOARD;
		else
			currentScheme = SCHEME_PLAYING;

		hintExpiration = platform->getExecMills() + 10000;

		reScheme();
	}

	// multi-scheme UI logic
	if(
			quitButtonZoneListener->wasChanged()
		) {
		reScheme();
//printf("quit button zone hit\n");
		if(currentScheme == SCHEME_WELCOME)
			needRedraw = true;
	}


	// scheme-specific logic
	if(currentScheme == SCHEME_WELCOME) {
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

		// update buttons
		if(
				startButtonZoneListener->wasChanged()
			) {
			reScheme();
			needRedraw = true;
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
			printf("T key press\n");
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
//keepDominicusAlive = false;

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
