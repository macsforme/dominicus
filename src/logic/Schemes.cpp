// Schemes.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "logic/Schemes.h"

void Schemes::mainMenuScheme() {
	// input
	inputHandler->mouse->addListener(gameLogic->mouseMotionListener);
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->mainMenuKeyListener);

	// splash background
	gameLogic->drawStack.push_back(gameLogic->splashEntry);

	// brand logo
	*((std::string*) gameLogic->brandEntry.second["texture"]) = "branding/brand";
	((UIMetrics*) gameLogic->brandEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->brandEntry.second["metrics"])->size = ((DrawTexture*) gameGraphics->drawers["texture"])->getSize(gameLogic->brandEntry.second);
	gameLogic->drawStack.push_back(gameLogic->brandEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->brandEntry.second["metrics"]);

	// presents label
	*((float*) gameLogic->presentsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->presentsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->presentsEntry.second["text"]) = "PRESENTS";
	((UIMetrics*) gameLogic->presentsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->presentsEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->presentsEntry.second);
	gameLogic->drawStack.push_back(gameLogic->presentsEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->presentsEntry.second["metrics"]);

	// title label
	*((float*) gameLogic->titleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSuper");
	*((Vector4*) gameLogic->titleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->titleEntry.second["text"]) = "CRUCIBLE ISLAND";
	((UIMetrics*) gameLogic->titleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->titleEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->titleEntry.second);
	gameLogic->drawStack.push_back(gameLogic->titleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->titleEntry.second["metrics"]);

	// game logo
	*((std::string*) gameLogic->logoEntry.second["texture"]) = "branding/logo";
	((UIMetrics*) gameLogic->logoEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->logoEntry.second["metrics"])->size = ((DrawTexture*) gameGraphics->drawers["texture"])->getSize(gameLogic->logoEntry.second);
	gameLogic->drawStack.push_back(gameLogic->logoEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->logoEntry.second["metrics"]);

	// spacer
	gameLogic->mainMenuSpacerMetrics->bearing1 = UIMetrics::BEARING_TOP;
	gameLogic->uiLayoutAuthority->metrics.push_back(gameLogic->mainMenuSpacerMetrics);

	// play button
	((UIMetrics*) gameLogic->playButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->playButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->playButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->playButtonEntry.second["text"]) = "Play";
	*((float*) gameLogic->playButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
	*((float*) gameLogic->playButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->playButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->playButtonEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->playButtonEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->playButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->playButtonEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->playButtonEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->playButtonEntry.second);
	gameLogic->drawStack.push_back(gameLogic->playButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->playButtonEntry.second["metrics"]);

	// settings button
	((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->settingsButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->settingsButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->settingsButtonEntry.second["text"]) = "Settings";
	*((float*) gameLogic->settingsButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
	*((float*) gameLogic->settingsButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->settingsButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->settingsButtonEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->settingsButtonEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->settingsButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->settingsButtonEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->settingsButtonEntry.second);
	gameLogic->drawStack.push_back(gameLogic->settingsButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"]);

	// high scores button
	((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->highScoresButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->highScoresButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->highScoresButtonEntry.second["text"]) = "High Scores";
	*((float*) gameLogic->highScoresButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
	*((float*) gameLogic->highScoresButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->highScoresButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->highScoresButtonEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->highScoresButtonEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->highScoresButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->highScoresButtonEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->highScoresButtonEntry.second);
	gameLogic->drawStack.push_back(gameLogic->highScoresButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"]);

	// help button
	((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->helpButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->helpButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->helpButtonEntry.second["text"]) = "Help";
	*((float*) gameLogic->helpButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
	*((float*) gameLogic->helpButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->helpButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->helpButtonEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->helpButtonEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->helpButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->helpButtonEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->helpButtonEntry.second);
	gameLogic->drawStack.push_back(gameLogic->helpButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"]);

	// quit button
	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->quitButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->quitButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->quitButtonEntry.second["text"]) = "Quit";
	*((float*) gameLogic->quitButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
	*((float*) gameLogic->quitButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->quitButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->quitButtonEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->quitButtonEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->quitButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->quitButtonEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->quitButtonEntry.second);
	gameLogic->drawStack.push_back(gameLogic->quitButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"]);

	// helpful tip labels
	*((float*) gameLogic->menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip1Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip1Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip1Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"]);

	*((float*) gameLogic->menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip2Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip2Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"]);

	*((float*) gameLogic->menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip3Entry.second["text"]) = "Press ENTER to select";
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip3Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip3Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"]);

	// make all buttons the same width
	float maxWidth = 0.0f;
	if(((UIMetrics*) gameLogic->playButtonEntry.second["metrics"])->size.x > maxWidth)
		maxWidth = ((UIMetrics*) gameLogic->playButtonEntry.second["metrics"])->size.x;
	if(((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"])->size.x > maxWidth)
		maxWidth = ((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"])->size.x;
	if(((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"])->size.x > maxWidth)
		maxWidth = ((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"])->size.x;
	if(((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"])->size.x > maxWidth)
		maxWidth = ((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"])->size.x;
	if(((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size.x > maxWidth)
		maxWidth = ((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size.x;
	((UIMetrics*) gameLogic->playButtonEntry.second["metrics"])->size.x = maxWidth;
	((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"])->size.x = maxWidth;
	((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"])->size.x = maxWidth;
	((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"])->size.x = maxWidth;
	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size.x = maxWidth;

	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();

	// set up the button listeners with the new width and positions
	gameLogic->playButtonZoneListener->ll = ((UIMetrics*) gameLogic->playButtonEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->playButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->playButtonZoneListener->ur = ((UIMetrics*) gameLogic->playButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->playButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->playButtonZoneListener);
	gameLogic->playButtonClickListener->ll = gameLogic->playButtonZoneListener->ll;
	gameLogic->playButtonClickListener->ur = gameLogic->playButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->playButtonClickListener);
	gameLogic->settingsButtonZoneListener->ll = ((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->settingsButtonZoneListener->ur = ((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->settingsButtonZoneListener);
	gameLogic->settingsButtonClickListener->ll = gameLogic->settingsButtonZoneListener->ll;
	gameLogic->settingsButtonClickListener->ur = gameLogic->settingsButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->settingsButtonClickListener);
	gameLogic->helpButtonZoneListener->ll = ((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->helpButtonZoneListener->ur = ((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->helpButtonZoneListener);
	gameLogic->helpButtonClickListener->ll = gameLogic->helpButtonZoneListener->ll;
	gameLogic->helpButtonClickListener->ur = gameLogic->helpButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->helpButtonClickListener);
	gameLogic->highScoresButtonZoneListener->ll = ((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->highScoresButtonZoneListener->ur = ((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->highScoresButtonZoneListener);
	gameLogic->highScoresButtonClickListener->ll = gameLogic->highScoresButtonZoneListener->ll;
	gameLogic->highScoresButtonClickListener->ur = gameLogic->highScoresButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->highScoresButtonClickListener);
	gameLogic->quitButtonZoneListener->ll = ((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->quitButtonZoneListener->ur = ((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->quitButtonZoneListener);
	gameLogic->quitButtonClickListener->ll = gameLogic->quitButtonZoneListener->ll;
	gameLogic->quitButtonClickListener->ur = gameLogic->quitButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->quitButtonClickListener);
}

void Schemes::helpScheme() {
	// input
	inputHandler->mouse->addListener(gameLogic->mouseMotionListener);
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->helpMenuKeyListener);

	// splash background
	gameLogic->drawStack.push_back(gameLogic->splashEntry);

	// help title label
	*((float*) gameLogic->helpTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->helpTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->helpTitleEntry.second["text"]) = "HELP";
	((UIMetrics*) gameLogic->helpTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->helpTitleEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->helpTitleEntry.second);
	gameLogic->drawStack.push_back(gameLogic->helpTitleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->helpTitleEntry.second["metrics"]);

	// controls title label
	*((float*) gameLogic->controlsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->controlsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->controlsTitleEntry.second["text"]) = "Controls";
	((UIMetrics*) gameLogic->controlsTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->controlsTitleEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->controlsTitleEntry.second);
	gameLogic->drawStack.push_back(gameLogic->controlsTitleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->controlsTitleEntry.second["metrics"]);

	// controls label
	*((float*) gameLogic->controlsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->controlsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->controlsEntry.second["text"]) = "Move Turret:\tArrow Keys / Mouse Movement\nFire Cannon:\tSpace / Left Mouse Button\nCharge / Fire Shock Field:\tTab / Right Mouse Button\nPause / Resume:\tesc\nToggle Fullscreen:\tF1\nFast Quit:\tF12";
	((UIMetrics*) gameLogic->controlsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->controlsEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->controlsEntry.second);
	gameLogic->drawStack.push_back(gameLogic->controlsEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->controlsEntry.second["metrics"]);

	// instructions title label
	*((float*) gameLogic->instructionsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->instructionsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->instructionsTitleEntry.second["text"]) = "Instructions";
	((UIMetrics*) gameLogic->instructionsTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->instructionsTitleEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->instructionsTitleEntry.second);
	gameLogic->drawStack.push_back(gameLogic->instructionsTitleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->instructionsTitleEntry.second["metrics"]);

	// instructions label
	*((float*) gameLogic->instructionsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->instructionsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((float*) gameLogic->instructionsEntry.second["wrap"]) = 2.0f * (gameGraphics->resolutionX > 1024 ? 1024.0f / (float) gameGraphics->resolutionX : 1.0f) - (gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX);
	*((std::string*) gameLogic->instructionsEntry.second["text"]) = "Welcome to Crucible Island. You occupy a tower atop an island mountain range. Enemy ships circle the island on a mission to destroy you. You must shoot down the missiles they fire at you using your cannon or by deploying shock fields. Firing cannon shells depletes your ammunition reservoir. Deploying shock fields requires a two-second charging period and depletes both your ammunition and health reservoirs by a quarter of their total capacities. A missile impact will deplete your health by half of its total capacity. Your ammunition and health reservoirs will recharge at a fixed rate. When your health level reaches zero, the game is over. The rate of enemy fire will increase as the game goes on.\n\nYour health level, ammunition level, and shock field charging indicators are at the top left corner of your screen. Your radar and heads-up display show enemy ship and incoming missile positions. You gain one point for every enemy missile you destroy. Good luck!";
	((UIMetrics*) gameLogic->instructionsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->instructionsEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->instructionsEntry.second);
	gameLogic->drawStack.push_back(gameLogic->instructionsEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->instructionsEntry.second["metrics"]);

	// about button
	((UIMetrics*) gameLogic->aboutButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->aboutButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->aboutButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->aboutButtonEntry.second["text"]) = "About";
	*((float*) gameLogic->aboutButtonEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
	*((float*) gameLogic->aboutButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->aboutButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->aboutButtonEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->aboutButtonEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->aboutButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->aboutButtonEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->aboutButtonEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->aboutButtonEntry.second);
	gameLogic->drawStack.push_back(gameLogic->aboutButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->aboutButtonEntry.second["metrics"]);

	// back button
	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->backButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->backButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->backButtonEntry.second["text"]) = "Back";
	*((float*) gameLogic->backButtonEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
	*((float*) gameLogic->backButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->backButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->backButtonEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->backButtonEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->backButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->backButtonEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->backButtonEntry.second);
	gameLogic->drawStack.push_back(gameLogic->backButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->backButtonEntry.second["metrics"]);

	// helpful tip labels
	*((float*) gameLogic->menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip1Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip1Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip1Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"]);

	*((float*) gameLogic->menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip2Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip2Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"]);

	*((float*) gameLogic->menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip3Entry.second["text"]) = "Press ENTER to select";
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip3Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip3Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"]);

	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();
	
	// set up button listeners with new position
	gameLogic->aboutButtonZoneListener->ll = ((UIMetrics*) gameLogic->aboutButtonEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->aboutButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->aboutButtonZoneListener->ur = ((UIMetrics*) gameLogic->aboutButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->aboutButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->aboutButtonZoneListener);
	gameLogic->aboutButtonClickListener->ll = gameLogic->aboutButtonZoneListener->ll;
	gameLogic->aboutButtonClickListener->ur = gameLogic->aboutButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->aboutButtonClickListener);
	gameLogic->backButtonZoneListener->ll = ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->backButtonZoneListener->ur = ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->backButtonZoneListener);
	gameLogic->backButtonClickListener->ll = gameLogic->backButtonZoneListener->ll;
	gameLogic->backButtonClickListener->ur = gameLogic->backButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->backButtonClickListener);
}

void Schemes::aboutScheme() {
	// input
	inputHandler->mouse->addListener(gameLogic->mouseMotionListener);
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->helpMenuKeyListener);

	// splash background
	gameLogic->drawStack.push_back(gameLogic->splashEntry);

	// about title label
	*((float*) gameLogic->aboutTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->aboutTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->aboutTitleEntry.second["text"]) = "ABOUT";
	((UIMetrics*) gameLogic->aboutTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->aboutTitleEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->aboutTitleEntry.second);
	gameLogic->drawStack.push_back(gameLogic->aboutTitleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->aboutTitleEntry.second["metrics"]);

	// version title label
	*((float*) gameLogic->versionTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->versionTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->versionTitleEntry.second["text"]) = "Version Information";
	((UIMetrics*) gameLogic->versionTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->versionTitleEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->versionTitleEntry.second);
	gameLogic->drawStack.push_back(gameLogic->versionTitleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->versionTitleEntry.second["metrics"]);

	// version label
	*((float*) gameLogic->versionEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->versionEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((float*) gameLogic->versionEntry.second["wrap"]) = 2.0f * (gameGraphics->resolutionX > 1024 ? 1024.0f / (float) gameGraphics->resolutionX : 1.0f) - (gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX);
	*((std::string*) gameLogic->versionEntry.second["text"]) = gameSystem->versionString;
	((UIMetrics*) gameLogic->versionEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->versionEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->versionEntry.second);
	gameLogic->drawStack.push_back(gameLogic->versionEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->versionEntry.second["metrics"]);

	// credits title label
	*((float*) gameLogic->creditsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->creditsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->creditsTitleEntry.second["text"]) = "Credits";
	((UIMetrics*) gameLogic->creditsTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->creditsTitleEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->creditsTitleEntry.second);
	gameLogic->drawStack.push_back(gameLogic->creditsTitleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->creditsTitleEntry.second["metrics"]);

	// credits label
	*((float*) gameLogic->creditsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->creditsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((float*) gameLogic->creditsEntry.second["wrap"]) = 2.0f * (gameGraphics->resolutionX > 1024 ? 1024.0f / (float) gameGraphics->resolutionX : 1.0f) - (gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX);
	*((std::string*) gameLogic->creditsEntry.second["text"]) = "Dedicated to Sergeant Sean Drenth #6894 of the Phoenix Police Department, EOW October 18, 2010.\n\nCreated by Joshua Bodine.\n\nMusic and sound effects by Michael Birch.\n\nThis software uses the Simple DirectMedia Layer library (http://www.libsdl.org/).\n\nPortions of this software are copyright (c) 2010 The FreeType Project (www.freetype.org).  All rights reserved.";
	((UIMetrics*) gameLogic->creditsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->creditsEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->creditsEntry.second);
	gameLogic->drawStack.push_back(gameLogic->creditsEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->creditsEntry.second["metrics"]);

	// back button
	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->backButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->backButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->backButtonEntry.second["text"]) = "Back";
	*((float*) gameLogic->backButtonEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
	*((float*) gameLogic->backButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->backButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->backButtonEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->backButtonEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->backButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->backButtonEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->backButtonEntry.second);
	gameLogic->drawStack.push_back(gameLogic->backButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->backButtonEntry.second["metrics"]);

	// helpful tip labels
	*((float*) gameLogic->menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip1Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip1Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip1Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"]);

	*((float*) gameLogic->menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip2Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip2Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"]);

	*((float*) gameLogic->menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip3Entry.second["text"]) = "Press ENTER to select";
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip3Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip3Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"]);

	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();
	
	// set up button listener with new position
	gameLogic->backButtonZoneListener->ll = ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->backButtonZoneListener->ur = ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->backButtonZoneListener);
	gameLogic->backButtonClickListener->ll = gameLogic->backButtonZoneListener->ll;
	gameLogic->backButtonClickListener->ur = gameLogic->backButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->backButtonClickListener);
}

void Schemes::settingsScheme() {
	// input
	inputHandler->mouse->addListener(gameLogic->mouseMotionListener);
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->settingsMenuKeyListener);

	// splash background
	gameLogic->drawStack.push_back(gameLogic->splashEntry);

	// settings title label
	*((float*) gameLogic->settingsMenuTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->settingsMenuTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->settingsMenuTitleEntry.second["text"]) = "SETTINGS";
	((UIMetrics*) gameLogic->settingsMenuTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->settingsMenuTitleEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->settingsMenuTitleEntry.second);
	gameLogic->drawStack.push_back(gameLogic->settingsMenuTitleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->settingsMenuTitleEntry.second["metrics"]);

	// level setting control label
	*((float*) gameLogic->levelSettingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->levelSettingEntry.second["fontColor"]) = (gameLogic->activeMenuSelection == &gameLogic->levelSettingEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
	std::stringstream ss; ss << "Starting Level: " << gameSystem->getString("gameStartingLevel");
	*((std::string*) gameLogic->levelSettingEntry.second["text"]) = ss.str().c_str();
	((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->levelSettingEntry.second);
	gameLogic->drawStack.push_back(gameLogic->levelSettingEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"]);

	// music setting control label
	*((float*) gameLogic->musicSettingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->musicSettingEntry.second["fontColor"]) = (gameLogic->activeMenuSelection == &gameLogic->musicSettingEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
	ss.str(""); ss << "Music Volume: ";
	int musicLevelInt = (int) (gameSystem->getFloat("audioMusicVolume") * 10.0f);
	std::string musicLevelString = "Off";
	if(musicLevelInt > 0.0f) {
		musicLevelString = "-----------";
		musicLevelString.replace((size_t) (musicLevelInt), 1, "|");
	}
	ss << "[ " << musicLevelString << " ]";
	*((std::string*) gameLogic->musicSettingEntry.second["text"]) = ss.str().c_str();
	((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->musicSettingEntry.second);
	gameLogic->drawStack.push_back(gameLogic->musicSettingEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"]);

	// audio effects setting control label
	*((float*) gameLogic->audioEffectsSettingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->audioEffectsSettingEntry.second["fontColor"]) = (gameLogic->activeMenuSelection == &gameLogic->audioEffectsSettingEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
	ss.str(""); ss << "Audio Effects Volume: ";
	int audioEffectsLevelInt = (int) (gameSystem->getFloat("audioEffectsVolume") * 10.0f);
	std::string audioEffectsLevelString = "Off";
	if(audioEffectsLevelInt > 0.0f) {
		audioEffectsLevelString = "-----------";
		audioEffectsLevelString.replace((size_t) (audioEffectsLevelInt), 1, "|");
	}
	ss << "[ " << audioEffectsLevelString << " ]";
	*((std::string*) gameLogic->audioEffectsSettingEntry.second["text"]) = ss.str().c_str();
	((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->audioEffectsSettingEntry.second);
	gameLogic->drawStack.push_back(gameLogic->audioEffectsSettingEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"]);

	// fullscreen setting control label
	*((float*) gameLogic->fullscreenSettingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->fullscreenSettingEntry.second["fontColor"]) = (gameLogic->activeMenuSelection == &gameLogic->fullscreenSettingEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
	*((std::string*) gameLogic->fullscreenSettingEntry.second["text"]) = (gameSystem->getBool("displayStartFullscreen") ? "Start Fullscreen: Yes" : "Start Fullscreen: No");
	((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->fullscreenSettingEntry.second);
	gameLogic->drawStack.push_back(gameLogic->fullscreenSettingEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"]);

	// high scores reset button
	((UIMetrics*) gameLogic->resetHighScoresEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->resetHighScoresEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->resetHighScoresEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->resetHighScoresEntry.second["text"]) = "Reset High Scores";
	*((float*) gameLogic->resetHighScoresEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
	*((float*) gameLogic->resetHighScoresEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->resetHighScoresEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->resetHighScoresEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->resetHighScoresEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->resetHighScoresEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->resetHighScoresEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->resetHighScoresEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->resetHighScoresEntry.second);
	gameLogic->drawStack.push_back(gameLogic->resetHighScoresEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->resetHighScoresEntry.second["metrics"]);

	// back button
	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->backButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->backButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->backButtonEntry.second["text"]) = "Back";
	*((float*) gameLogic->backButtonEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
	*((float*) gameLogic->backButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->backButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->backButtonEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->backButtonEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->backButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->backButtonEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->backButtonEntry.second);
	gameLogic->drawStack.push_back(gameLogic->backButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->backButtonEntry.second["metrics"]);

	// helpful tip labels
	*((float*) gameLogic->menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip1Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip1Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip1Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"]);

	*((float*) gameLogic->menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip2Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip2Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"]);

	*((float*) gameLogic->menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip3Entry.second["text"]) = "Press ENTER to select";
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip3Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip3Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"]);

	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();
	
	// set up button listeners with new positions
	gameLogic->levelButtonZoneListener->ll = ((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"])->size / 2.0f;
	gameLogic->levelButtonZoneListener->ur = ((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->levelButtonZoneListener);
	gameLogic->levelButtonClickListener->ll = gameLogic->levelButtonZoneListener->ll;
	gameLogic->levelButtonClickListener->ur = gameLogic->levelButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->levelButtonClickListener);

	gameLogic->musicButtonZoneListener->ll = ((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"])->size / 2.0f;
	gameLogic->musicButtonZoneListener->ur = ((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->musicButtonZoneListener);
	gameLogic->musicButtonClickListener->ll = gameLogic->musicButtonZoneListener->ll;
	gameLogic->musicButtonClickListener->ur = gameLogic->musicButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->musicButtonClickListener);

	gameLogic->audioEffectsButtonZoneListener->ll = ((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"])->size / 2.0f;
	gameLogic->audioEffectsButtonZoneListener->ur = ((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->audioEffectsButtonZoneListener);
	gameLogic->audioEffectsButtonClickListener->ll = gameLogic->audioEffectsButtonZoneListener->ll;
	gameLogic->audioEffectsButtonClickListener->ur = gameLogic->audioEffectsButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->audioEffectsButtonClickListener);

	gameLogic->fullscreenButtonZoneListener->ll = ((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"])->size / 2.0f;
	gameLogic->fullscreenButtonZoneListener->ur = ((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->fullscreenButtonZoneListener);
	gameLogic->fullscreenButtonClickListener->ll = gameLogic->fullscreenButtonZoneListener->ll;
	gameLogic->fullscreenButtonClickListener->ur = gameLogic->fullscreenButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->fullscreenButtonClickListener);

	gameLogic->backButtonZoneListener->ll = ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->backButtonZoneListener->ur = ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->backButtonZoneListener);
	gameLogic->backButtonClickListener->ll = gameLogic->backButtonZoneListener->ll;
	gameLogic->backButtonClickListener->ur = gameLogic->backButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->backButtonClickListener);

	gameLogic->resetHighScoresButtonZoneListener->ll = ((UIMetrics*) gameLogic->resetHighScoresEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->resetHighScoresEntry.second["metrics"])->size / 2.0f;
	gameLogic->resetHighScoresButtonZoneListener->ur = ((UIMetrics*) gameLogic->resetHighScoresEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->resetHighScoresEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->resetHighScoresButtonZoneListener);
	gameLogic->resetHighScoresButtonClickListener->ll = gameLogic->resetHighScoresButtonZoneListener->ll;
	gameLogic->resetHighScoresButtonClickListener->ur = gameLogic->resetHighScoresButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->resetHighScoresButtonClickListener);
}

void Schemes::highScoresScheme() {
	// input
	inputHandler->mouse->addListener(gameLogic->mouseMotionListener);
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->highScoresMenuKeyListener);

	// splash background
	gameLogic->drawStack.push_back(gameLogic->splashEntry);

	// high scores title label
	*((float*) gameLogic->highScoresTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->highScoresTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->highScoresTitleEntry.second["text"]) = "HIGH SCORES";
	((UIMetrics*) gameLogic->highScoresTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->highScoresTitleEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->highScoresTitleEntry.second);
	gameLogic->drawStack.push_back(gameLogic->highScoresTitleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->highScoresTitleEntry.second["metrics"]);

	// high scores label
	*((float*) gameLogic->highScoresLabelEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->highScoresLabelEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	std::stringstream stringStream;
	for(size_t i = 0; i < gameSystem->highScores.size(); ++i)
		stringStream << (i > 0 ? "\n" : "") << gameSystem->highScores[i].second << "\t" << gameSystem->highScores[i].first;
	if(stringStream.str().length() == 0) stringStream.str("No high scores have been recorded yet.");
	*((std::string*) gameLogic->highScoresLabelEntry.second["text"]) = stringStream.str();
	((UIMetrics*) gameLogic->highScoresLabelEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->highScoresLabelEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->highScoresLabelEntry.second);
	gameLogic->drawStack.push_back(gameLogic->highScoresLabelEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->highScoresLabelEntry.second["metrics"]);

	// back button
	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->backButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->backButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->backButtonEntry.second["text"]) = "Back";
	*((float*) gameLogic->backButtonEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
	*((float*) gameLogic->backButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->backButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->backButtonEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->backButtonEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->backButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->backButtonEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->backButtonEntry.second);
	gameLogic->drawStack.push_back(gameLogic->backButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->backButtonEntry.second["metrics"]);

	// helpful tip labels
	*((float*) gameLogic->menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip1Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip1Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip1Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"]);

	*((float*) gameLogic->menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip2Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip2Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"]);

	*((float*) gameLogic->menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip3Entry.second["text"]) = "Press ENTER to select";
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip3Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip3Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"]);

	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();
	
	// set up button listener with new position
	gameLogic->backButtonZoneListener->ll = ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->backButtonZoneListener->ur = ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->backButtonZoneListener);
	gameLogic->backButtonClickListener->ll = gameLogic->backButtonZoneListener->ll;
	gameLogic->backButtonClickListener->ur = gameLogic->backButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->backButtonClickListener);
}

void Schemes::loadingScheme() {
	// input
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);

	// splash background
	gameLogic->drawStack.push_back(gameLogic->splashEntry);

	// loading label
	*((float*) gameLogic->loadingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->loadingEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->loadingEntry.second["text"]) = "Loading...";
	((UIMetrics*) gameLogic->loadingEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->loadingEntry.second);
	gameLogic->drawStack.push_back(gameLogic->loadingEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->loadingEntry.second["metrics"]);

	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();
}

void Schemes::playingScheme() {
	// input
	inputHandler->mouse->addListener(gameLogic->mouseMotionListener);
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->playingKeyListener);
	inputHandler->keyboard->addListener(gameLogic->cameraAheadKeyListener);
	inputHandler->keyboard->addListener(gameLogic->turretUpKeyListener);
	inputHandler->keyboard->addListener(gameLogic->turretDownKeyListener);
	inputHandler->keyboard->addListener(gameLogic->turretLeftKeyListener);
	inputHandler->keyboard->addListener(gameLogic->turretRightKeyListener);

	// sky
	gameLogic->drawStack.push_back(gameLogic->skyEntry);

	// water
	gameLogic->drawStack.push_back(gameLogic->waterEntry);

	// terrain
	gameLogic->drawStack.push_back(gameLogic->terrainEntry);

	// ships
	gameLogic->drawStack.push_back(gameLogic->shipEntry);

	// tower
	gameLogic->drawStack.push_back(gameLogic->towerEntry);

	// missiles
	gameLogic->drawStack.push_back(gameLogic->missileEntry);

	// clock
/*
	*((float*) gameLogic->clockLabel.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->clockLabel.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	time_t rawTime;
	struct tm* timeInfo;
	time(&rawTime);
	timeInfo = localtime(&rawTime);
	char timeString[6];
	strftime(timeString, 6, "%H:%M", timeInfo);
	*((std::string*) gameLogic->clockLabel.second["text"]) = timeString;
	((UIMetrics*) gameLogic->clockLabel.second["metrics"])->bearing1 = UIMetrics::BEARING_LEFT;
	((UIMetrics*) gameLogic->clockLabel.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->clockLabel.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->clockLabel.second);
	gameLogic->drawStack.push_back(gameLogic->clockLabel);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->clockLabel.second["metrics"]);
*/

	// score
	*((float*) gameLogic->scoreLabel.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->scoreLabel.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	char scoreString[8];  scoreString[0] = '\0'; if(gameState->score <= 9999999) sprintf(scoreString, "%u", gameState->score);
	*((std::string*) gameLogic->scoreLabel.second["text"]) = scoreString;
	((UIMetrics*) gameLogic->scoreLabel.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->scoreLabel.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->scoreLabel.second);
	gameLogic->drawStack.push_back(gameLogic->scoreLabel);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->scoreLabel.second["metrics"]);

	// tip
	*((float*) gameLogic->playingTipEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->playingTipEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->playingTipEntry.second["text"]) = "Press ESC to pause and show options";
	((UIMetrics*) gameLogic->playingTipEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->playingTipEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->playingTipEntry.second);
	gameLogic->drawStack.push_back(gameLogic->playingTipEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->playingTipEntry.second["metrics"]);

	// control box
	*((bool*) gameLogic->controlBoxEntry.second["drawCorners"]) = (gameLogic->mouseActive ? true : false);
	gameLogic->drawStack.push_back(gameLogic->controlBoxEntry);

	// gauge panel
	*((float*) gameLogic->gaugePanelEntry.second["padding"]) = gameSystem->getFloat("hudGaugePadding");
	*((float*) gameLogic->gaugePanelEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->gaugePanelEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->gaugePanelEntry.second["insideColor"]) = gameSystem->getColor("hudControlBoxColor");
	*((Vector4*) gameLogic->gaugePanelEntry.second["borderColor"]) = gameSystem->getColor("hudControlBoxColor");
	*((Vector4*) gameLogic->gaugePanelEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudControlBoxColor").x,
			gameSystem->getColor("hudControlBoxColor").y,
			gameSystem->getColor("hudControlBoxColor").z,
			0.0f
		);
	*((size_t*) gameLogic->gaugePanelEntry.second["elements"]) = 3;
	(*((std::vector<std::string>*) gameLogic->gaugePanelEntry.second["textureNames"]))[0] = "gauge/heart";
	(*((std::vector<std::string>*) gameLogic->gaugePanelEntry.second["textureNames"]))[1] = "gauge/shell";
	(*((std::vector<std::string>*) gameLogic->gaugePanelEntry.second["textureNames"]))[2] = "gauge/bolt";
	*((Vector2*) gameLogic->gaugePanelEntry.second["progressBarSize"]) = Vector2(gameSystem->getFloat("hudGaugeWidth"), gameSystem->getFloat("hudGaugeHeight"));
	(*((std::vector<float>*) gameLogic->gaugePanelEntry.second["progressions"]))[0] = gameState->fortress.health;
	(*((std::vector<float>*) gameLogic->gaugePanelEntry.second["progressions"]))[1] = gameState->fortress.ammunition;
	(*((std::vector<float>*) gameLogic->gaugePanelEntry.second["progressions"]))[2] = gameState->fortress.shock;
	*((Vector4*) gameLogic->gaugePanelEntry.second["backgroundColorTop"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) gameLogic->gaugePanelEntry.second["backgroundColorBottom"]) = Vector4(
			gameSystem->getColor("hudGaugeBackgroundColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor("hudGaugeBackgroundColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor("hudGaugeBackgroundColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor("hudGaugeBackgroundColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	(*((std::vector<Vector4>*) gameLogic->gaugePanelEntry.second["progressBarColorsTop"]))[0] = gameSystem->getColor("hudGaugeHealthBarColor");
	(*((std::vector<Vector4>*) gameLogic->gaugePanelEntry.second["progressBarColorsTop"]))[1] = gameSystem->getColor("hudGaugeAmmoBarColor");
	(*((std::vector<Vector4>*) gameLogic->gaugePanelEntry.second["progressBarColorsTop"]))[2] = gameSystem->getColor(gameState->fortress.shock < 1.0f ? "hudGaugeShockChargingBarColor" : "hudGaugeShockChargedBarColor");
	(*((std::vector<Vector4>*) gameLogic->gaugePanelEntry.second["progressBarColorsBottom"]))[0] = Vector4(
			gameSystem->getColor("hudGaugeHealthBarColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor("hudGaugeHealthBarColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor("hudGaugeHealthBarColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor("hudGaugeHealthBarColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	(*((std::vector<Vector4>*) gameLogic->gaugePanelEntry.second["progressBarColorsBottom"]))[1] = Vector4(
			gameSystem->getColor("hudGaugeAmmoBarColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor("hudGaugeAmmoBarColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor("hudGaugeAmmoBarColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor("hudGaugeAmmoBarColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	(*((std::vector<Vector4>*) gameLogic->gaugePanelEntry.second["progressBarColorsBottom"]))[2] = Vector4(
			gameSystem->getColor(gameState->fortress.shock < 1.0f ? "hudGaugeShockChargingBarColor" : "hudGaugeShockChargedBarColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor(gameState->fortress.shock < 1.0f ? "hudGaugeShockChargingBarColor" : "hudGaugeShockChargedBarColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor(gameState->fortress.shock < 1.0f ? "hudGaugeShockChargingBarColor" : "hudGaugeShockChargedBarColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor(gameState->fortress.shock < 1.0f ? "hudGaugeShockChargingBarColor" : "hudGaugeShockChargedBarColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	((UIMetrics*) gameLogic->gaugePanelEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_LEFT;
	((UIMetrics*) gameLogic->gaugePanelEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->gaugePanelEntry.second["metrics"])->size = ((DrawGaugePanel*) gameGraphics->drawers["gaugePanel"])->getSize(gameLogic->gaugePanelEntry.second);
	gameLogic->drawStack.push_back(gameLogic->gaugePanelEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->gaugePanelEntry.second["metrics"]);

	// radar
	*((float*) gameLogic->radarEntry.second["padding"]) = gameSystem->getFloat("hudGaugePadding");
	*((float*) gameLogic->radarEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->radarEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->radarEntry.second["insideColor"]) = gameSystem->getColor("hudControlBoxColor");
	*((Vector4*) gameLogic->radarEntry.second["borderColor"]) = gameSystem->getColor("hudControlBoxColor");
	*((Vector4*) gameLogic->radarEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudControlBoxColor").x,
			gameSystem->getColor("hudControlBoxColor").y,
			gameSystem->getColor("hudControlBoxColor").z,
			0.0f
		);
	((UIMetrics*) gameLogic->radarEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_RIGHT;
	((UIMetrics*) gameLogic->radarEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->radarEntry.second["metrics"])->size =
			((DrawRadar*) gameGraphics->drawers["radar"])->getSize(gameLogic->radarEntry.second);
	gameLogic->drawStack.push_back(gameLogic->radarEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->radarEntry.second["metrics"]);

	// cursor
	if(gameLogic->mouseActive) {
		*((float*) gameLogic->cursorEntry.second["size"]) = gameSystem->getFloat("hudCursorSize");
		gameLogic->cursorEntry.second["position"] = (gameLogic->mouseActive ? (void*) &inputHandler->mouse->position : &gameLogic->keyboardCursorPosition);
		*((float*) gameLogic->cursorEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
		*((float*) gameLogic->cursorEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
		*((Vector4*) gameLogic->cursorEntry.second["insideColor"]) = gameSystem->getColor("hudCursorInsideColor");
		*((Vector4*) gameLogic->cursorEntry.second["borderColor"]) = gameSystem->getColor("hudCursorBorderColor");
		*((Vector4*) gameLogic->cursorEntry.second["outsideColor"]) = Vector4(
				gameSystem->getColor("hudCursorBorderColor").x,
				gameSystem->getColor("hudCursorBorderColor").y,
				gameSystem->getColor("hudCursorBorderColor").z,
				0.0f
			);
		gameLogic->drawStack.push_back(gameLogic->cursorEntry);
	}

	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();
}

void Schemes::introScheme() {
	// input
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->introKeyListener);

	gameLogic->introMouseButtonListener->ll = Vector2(-1.0f, -1.0f);
	gameLogic->introMouseButtonListener->ur = Vector2(1.0f, 1.0f);
	inputHandler->mouse->addListener(gameLogic->introMouseButtonListener);

	// sky
	gameLogic->drawStack.push_back(gameLogic->skyEntry);

	// water
	gameLogic->drawStack.push_back(gameLogic->waterEntry);

	// terrain
	gameLogic->drawStack.push_back(gameLogic->terrainEntry);

	// ships
	gameLogic->drawStack.push_back(gameLogic->shipEntry);

	// tower
	gameLogic->drawStack.push_back(gameLogic->towerEntry);

	// missiles
	gameLogic->drawStack.push_back(gameLogic->missileEntry);

	// tip
	*((float*) gameLogic->introHintEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->introHintEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->introHintEntry.second["text"]) = "LEFT CLICK or press SPACE to skip introduction";
	((UIMetrics*) gameLogic->introHintEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->introHintEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->introHintEntry.second);
	gameLogic->drawStack.push_back(gameLogic->introHintEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->introHintEntry.second["metrics"]);

	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();
}

void Schemes::pausedScheme() {
	// input
	inputHandler->mouse->addListener(gameLogic->mouseMotionListener);
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->pausedMenuKeyListener);

	// sky
	gameLogic->drawStack.push_back(gameLogic->skyEntry);

	// water
	gameLogic->drawStack.push_back(gameLogic->waterEntry);

	// terrain
	gameLogic->drawStack.push_back(gameLogic->terrainEntry);

	// ships
	gameLogic->drawStack.push_back(gameLogic->shipEntry);

	// tower
	gameLogic->drawStack.push_back(gameLogic->towerEntry);

	// missiles
	gameLogic->drawStack.push_back(gameLogic->missileEntry);

	// gray out
	*((Vector4*) gameLogic->grayOutEntry.second["color"]) = gameSystem->getColor("hudGrayOutColor");
	gameLogic->drawStack.push_back(gameLogic->grayOutEntry);

	// paused title label
	*((float*) gameLogic->pausedEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->pausedEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->pausedEntry.second["text"]) = "PAUSED";
	((UIMetrics*) gameLogic->pausedEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->pausedEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->pausedEntry.second);
	gameLogic->drawStack.push_back(gameLogic->pausedEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->pausedEntry.second["metrics"]);

	// controls title label
	*((float*) gameLogic->controlsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->controlsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->controlsTitleEntry.second["text"]) = "Controls";
	((UIMetrics*) gameLogic->controlsTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->controlsTitleEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->controlsTitleEntry.second);
	gameLogic->drawStack.push_back(gameLogic->controlsTitleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->controlsTitleEntry.second["metrics"]);

	// controls label
	*((float*) gameLogic->controlsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->controlsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->controlsEntry.second["text"]) = "Move Turret:\tArrow Keys / Mouse Movement\nFire Cannon:\tSpace / Left Mouse Button\nCharge / Fire Shock Field:\tTab / Right Mouse Button\nPause / Resume:\tesc\nToggle Fullscreen:\tF1\nFast Quit:\tF12";
	((UIMetrics*) gameLogic->controlsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->controlsEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->controlsEntry.second);
	gameLogic->drawStack.push_back(gameLogic->controlsEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->controlsEntry.second["metrics"]);

	// spacer
	gameLogic->pausedMenuSpacerMetrics->bearing1 = UIMetrics::BEARING_TOP;
	gameLogic->uiLayoutAuthority->metrics.push_back(gameLogic->pausedMenuSpacerMetrics);

	// resume button
	*((float*) gameLogic->resumeButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->resumeButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->resumeButtonEntry.second["text"]) = "Resume";
	*((float*) gameLogic->resumeButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
	*((float*) gameLogic->resumeButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->resumeButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->resumeButtonEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->resumeButtonEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->resumeButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->resumeButtonEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->resumeButtonEntry.second);
	gameLogic->drawStack.push_back(gameLogic->resumeButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"]);

	// end game button
	((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->endGameButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->endGameButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->endGameButtonEntry.second["text"]) = "End Game";
	*((float*) gameLogic->endGameButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
	*((float*) gameLogic->endGameButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->endGameButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->endGameButtonEntry.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->endGameButtonEntry ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->endGameButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->endGameButtonEntry.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->endGameButtonEntry.second);
	gameLogic->drawStack.push_back(gameLogic->endGameButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"]);

	// helpful tip labels
	*((float*) gameLogic->pausedMenuTipEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->pausedMenuTipEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->pausedMenuTipEntry.second["text"]) = "Press ESC to return to the game";
	((UIMetrics*) gameLogic->pausedMenuTipEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->pausedMenuTipEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->pausedMenuTipEntry.second);
	gameLogic->drawStack.push_back(gameLogic->pausedMenuTipEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->pausedMenuTipEntry.second["metrics"]);

	*((float*) gameLogic->menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip2Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip2Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"]);

	*((float*) gameLogic->menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip3Entry.second["text"]) = "Press ENTER to select";
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->menuTip3Entry.second);
	gameLogic->drawStack.push_back(gameLogic->menuTip3Entry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"]);

	// make all buttons the same width
	float maxWidth = 0.0f;
	if(((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->size.x > maxWidth)
		maxWidth = ((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->size.x;
	if(((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->size.x > maxWidth)
		maxWidth = ((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->size.x;
	((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->size.x = maxWidth;
	((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->size.x = maxWidth;

	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();
	
	// set up button listeners with new position
	gameLogic->resumeButtonZoneListener->ll = ((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->resumeButtonZoneListener->ur = ((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->resumeButtonZoneListener);
	gameLogic->resumeButtonClickListener->ll = gameLogic->resumeButtonZoneListener->ll;
	gameLogic->resumeButtonClickListener->ur = gameLogic->resumeButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->resumeButtonClickListener);

	gameLogic->endGameButtonZoneListener->ll = ((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->position -	((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->endGameButtonZoneListener->ur = ((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->endGameButtonZoneListener);
	gameLogic->endGameButtonClickListener->ll = gameLogic->endGameButtonZoneListener->ll;
	gameLogic->endGameButtonClickListener->ur = gameLogic->endGameButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->endGameButtonClickListener);
}

void Schemes::gameOverScheme() {
	// input
	inputHandler->mouse->addListener(gameLogic->mouseMotionListener);
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->gameOverKeyListener);
	inputHandler->keyboard->addListener(gameLogic->deleteKeyListener);

	// splash background
	gameLogic->drawStack.push_back(gameLogic->splashEntry);

	// game over title label
	*((float*) gameLogic->gameOverTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->gameOverTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->gameOverTitleEntry.second["text"]) = "GAME OVER";
	((UIMetrics*) gameLogic->gameOverTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->gameOverTitleEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->gameOverTitleEntry.second);
	gameLogic->drawStack.push_back(gameLogic->gameOverTitleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->gameOverTitleEntry.second["metrics"]);

	// score title label
	*((float*) gameLogic->yourScoreTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->yourScoreTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->yourScoreTitleEntry.second["text"]) = "Score";
	((UIMetrics*) gameLogic->yourScoreTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->yourScoreTitleEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->yourScoreTitleEntry.second);
	gameLogic->drawStack.push_back(gameLogic->yourScoreTitleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->yourScoreTitleEntry.second["metrics"]);

	// score label
	*((float*) gameLogic->yourScoreEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->yourScoreEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	char scoreString[8];  scoreString[0] = '\0'; if(gameState->score <= 9999999) sprintf(scoreString, "%u", gameState->score);
	*((std::string*) gameLogic->yourScoreEntry.second["text"]) = scoreString;
	((UIMetrics*) gameLogic->yourScoreEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->yourScoreEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->yourScoreEntry.second);
	gameLogic->drawStack.push_back(gameLogic->yourScoreEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->yourScoreEntry.second["metrics"]);

	// high scores section label
	*((float*) gameLogic->highScoresSectionEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->highScoresSectionEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->highScoresSectionEntry.second["text"]) = "High Scores";
	((UIMetrics*) gameLogic->highScoresSectionEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->highScoresSectionEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->highScoresSectionEntry.second);
	gameLogic->drawStack.push_back(gameLogic->highScoresSectionEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->highScoresSectionEntry.second["metrics"]);

	// high scores label
	*((float*) gameLogic->highScoresLabelEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->highScoresLabelEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	std::stringstream stringStream;
	for(size_t i = 0; i < gameSystem->highScores.size(); ++i)
		stringStream << (i > 0 ? "\n" : "") << gameSystem->highScores[i].second << "\t" << gameSystem->highScores[i].first;
	if(stringStream.str().length() == 0) stringStream.str("No high scores have been recorded yet.");
	*((std::string*) gameLogic->highScoresLabelEntry.second["text"]) = stringStream.str();
	((UIMetrics*) gameLogic->highScoresLabelEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->highScoresLabelEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->highScoresLabelEntry.second);
	gameLogic->drawStack.push_back(gameLogic->highScoresLabelEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->highScoresLabelEntry.second["metrics"]);

	if(gameState->score > 0 && (gameSystem->highScores.size() == 0 || gameState->score > gameSystem->highScores[0].first)) {
		// high scores label
		*((float*) gameLogic->newHighScoreLabel.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
		*((Vector4*) gameLogic->newHighScoreLabel.second["fontColor"]) = gameSystem->getColor("fontColorLight");
		*((std::string*) gameLogic->newHighScoreLabel.second["text"]) = "Enter New High Score";
		((UIMetrics*) gameLogic->newHighScoreLabel.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
		((UIMetrics*) gameLogic->newHighScoreLabel.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->newHighScoreLabel.second);
		gameLogic->drawStack.push_back(gameLogic->newHighScoreLabel);
		gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->newHighScoreLabel.second["metrics"]);

		// name prompt label
		*((float*) gameLogic->newHighScoreNameLabel.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
		*((Vector4*) gameLogic->newHighScoreNameLabel.second["fontColor"]) = gameSystem->getColor("fontColorLight");
		*((std::string*) gameLogic->newHighScoreNameLabel.second["text"]) = "Player Name:";
		((UIMetrics*) gameLogic->newHighScoreNameLabel.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->newHighScoreNameLabel.second);
//FIXME FIIIIIIIIXXXXX MEEEEEEEE text doesn't line up if one but not the other goes below the baseline!

		// name prompt field
		*((float*) gameLogic->newHighScoreNameField.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
		*((Vector4*) gameLogic->newHighScoreNameField.second["fontColor"]) = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		*((Vector4*) gameLogic->newHighScoreNameField.second["boxColor"]) = gameSystem->getColor("hudFieldColor");
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) = "";
		for(size_t i = 0; i < gameSystem->getFloat("hudFieldWidth") + 1; ++i) *((std::string*) gameLogic->newHighScoreNameField.second["text"]) += "#";
		((UIMetrics*) gameLogic->newHighScoreNameField.second["metrics"])->size = ((DrawField*) gameGraphics->drawers["field"])->getSize(gameLogic->newHighScoreNameField.second);
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) = "\\";
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) += gameSystem->getString("fontColorLight");
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) += gameLogic->playerName;
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) += "\\";
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) += gameSystem->getString("fontColorDark");
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) += "|";

		// name prompt container
		*((float*) gameLogic->newHighScoreContainer.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
		*((float*) gameLogic->newHighScoreContainer.second["border"]) = 0.0f;
		*((float*) gameLogic->newHighScoreContainer.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
		*((Vector4*) gameLogic->newHighScoreContainer.second["insideColor"]) = gameSystem->getColor("hudContainerInsideColor");
		*((Vector4*) gameLogic->newHighScoreContainer.second["borderColor"]) = gameSystem->getColor("hudContainerOutsideColor");
		*((Vector4*) gameLogic->newHighScoreContainer.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
		((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
		((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->size = Vector2(
				((UIMetrics*) gameLogic->newHighScoreNameLabel.second["metrics"])->size.x +
						((UIMetrics*) gameLogic->newHighScoreNameField.second["metrics"])->size.x +
						gameSystem->getFloat("hudButtonPadding") / gameGraphics->resolutionX * 2.0f * 3.0f,
				(((UIMetrics*) gameLogic->newHighScoreNameLabel.second["metrics"])->size.y >
						((UIMetrics*) gameLogic->newHighScoreNameField.second["metrics"])->size.y ?
						((UIMetrics*) gameLogic->newHighScoreNameLabel.second["metrics"])->size.y :
						((UIMetrics*) gameLogic->newHighScoreNameField.second["metrics"])->size.y) +
						gameSystem->getFloat("hudBigButtonPadding") / gameGraphics->resolutionY * 4.0f
			);
		gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"]);
	}

	// end game button
	((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->gameOverContinueButton.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->gameOverContinueButton.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->gameOverContinueButton.second["text"]) = "Continue";
	*((float*) gameLogic->gameOverContinueButton.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
	*((float*) gameLogic->gameOverContinueButton.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->gameOverContinueButton.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->gameOverContinueButton.second["insideColor"]) = (
			gameLogic->activeMenuSelection == &gameLogic->gameOverContinueButton ?
			gameSystem->getColor("hudContainerHighlightColor") :
			gameSystem->getColor("hudContainerInsideColor")
		);
	*((Vector4*) gameLogic->gameOverContinueButton.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->gameOverContinueButton.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
	((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"])->size = ((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->gameOverContinueButton.second);
	gameLogic->drawStack.push_back(gameLogic->gameOverContinueButton);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"]);

	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();

	if(gameState->score > 0 && (gameSystem->highScores.size() == 0 || gameState->score > gameSystem->highScores[0].first)) {
		// assemble prompt container contents
		((UIMetrics*) gameLogic->newHighScoreNameLabel.second["metrics"])->position = Vector2(
				((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->position.x -
						((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->size.x / 2.0f +
						gameSystem->getFloat("hudBigButtonPadding") / gameGraphics->resolutionX * 2.0f +
						((UIMetrics*) gameLogic->newHighScoreNameLabel.second["metrics"])->size.x / 2.0f,
				((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->position.y
			);
		((UIMetrics*) gameLogic->newHighScoreNameField.second["metrics"])->position = Vector2(
				((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->position.x +
						((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->size.x / 2.0f -
						gameSystem->getFloat("hudBigButtonPadding") / gameGraphics->resolutionX * 2.0f -
						((UIMetrics*) gameLogic->newHighScoreNameField.second["metrics"])->size.x / 2.0f,
				((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->position.y
			);
		gameLogic->drawStack.push_back(gameLogic->newHighScoreContainer);
		gameLogic->drawStack.push_back(gameLogic->newHighScoreNameLabel);
		gameLogic->drawStack.push_back(gameLogic->newHighScoreNameField);
	}

	// set up button listener with new position
	gameLogic->gameOverContinueButtonZoneListener->ll = ((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"])->position -	((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"])->size / 2.0f;
	gameLogic->gameOverContinueButtonZoneListener->ur = ((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"])->position + ((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->gameOverContinueButtonZoneListener);
	gameLogic->gameOverContinueButtonClickListener->ll = gameLogic->gameOverContinueButtonZoneListener->ll;
	gameLogic->gameOverContinueButtonClickListener->ur = gameLogic->gameOverContinueButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->gameOverContinueButtonClickListener);
}
