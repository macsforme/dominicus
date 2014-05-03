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
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->playingKeyListener);
	inputHandler->keyboard->addListener(gameLogic->cameraAheadKeyListener);
	inputHandler->keyboard->addListener(gameLogic->cameraUpKeyListener);
	inputHandler->keyboard->addListener(gameLogic->cameraDownKeyListener);
	inputHandler->keyboard->addListener(gameLogic->cameraLeftKeyListener);
	inputHandler->keyboard->addListener(gameLogic->cameraRightKeyListener);

	// sky
//	gameLogic->drawStack.push_back(gameLogic->skyEntry);

	// water
	gameLogic->drawStack.push_back(gameLogic->waterEntry);

	// terrain
	gameLogic->drawStack.push_back(gameLogic->terrainEntry);

	// ships
	gameLogic->drawStack.push_back(gameLogic->shipEntry);

	// tower
	gameLogic->drawStack.push_back(gameLogic->towerEntry);

	// score
	*((float*) gameLogic->scoreLabel.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->scoreLabel.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->scoreLabel.second["text"]) = "364";
	((UIMetrics*) gameLogic->scoreLabel.second["metrics"])->bearing1 = UIMetrics::BEARING_RIGHT;
	((UIMetrics*) gameLogic->scoreLabel.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->scoreLabel.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->scoreLabel.second);
	gameLogic->drawStack.push_back(gameLogic->scoreLabel);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->scoreLabel.second["metrics"]);

	// tip
	*((float*) gameLogic->playingTipEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->playingTipEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->playingTipEntry.second["text"]) = "Press esc to pause and show options";
	((UIMetrics*) gameLogic->playingTipEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->playingTipEntry.second["metrics"])->size = ((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->playingTipEntry.second);
	gameLogic->drawStack.push_back(gameLogic->playingTipEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->playingTipEntry.second["metrics"]);

	// control box
	gameLogic->drawStack.push_back(gameLogic->controlBoxEntry);

	// health gauge image
	*((std::string*) gameLogic->healthGaugeImage.second["texture"]) = "gauge/heart";
	((UIMetrics*) gameLogic->healthGaugeImage.second["metrics"])->bearing1 = UIMetrics::BEARING_LEFT;
	((UIMetrics*) gameLogic->healthGaugeImage.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->healthGaugeImage.second["metrics"])->size = ((DrawTexture*) gameGraphics->drawers["texture"])->getSize(gameLogic->healthGaugeImage.second);
	gameLogic->drawStack.push_back(gameLogic->healthGaugeImage);

	// health gauge bar
	*((Vector4*) gameLogic->healthGaugeBar.second["color1Top"]) = gameSystem->getColor("hudGaugeHealthBarColor");
	*((Vector4*) gameLogic->healthGaugeBar.second["color1Bottom"]) = Vector4(
			gameSystem->getColor("hudGaugeHealthBarColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor("hudGaugeHealthBarColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor("hudGaugeHealthBarColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor("hudGaugeHealthBarColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	*((Vector4*) gameLogic->healthGaugeBar.second["color2Top"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) gameLogic->healthGaugeBar.second["color2Bottom"]) = Vector4(
			gameSystem->getColor("hudGaugeBackgroundColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor("hudGaugeBackgroundColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor("hudGaugeBackgroundColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor("hudGaugeBackgroundColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	*((float*) gameLogic->healthGaugeBar.second["progression"]) = 0.75f;
	*((Vector2*) gameLogic->healthGaugeBar.second["size"]) = Vector2(
			gameSystem->getFloat("hudGaugeWidth"),
			gameSystem->getFloat("hudGaugeHeight")
		);
	((UIMetrics*) gameLogic->healthGaugeBar.second["metrics"])->size = ((DrawProgressBar*) gameGraphics->drawers["progressBar"])->getSize(gameLogic->healthGaugeBar.second);
	gameLogic->drawStack.push_back(gameLogic->healthGaugeBar);

	// ammo gauge image
	*((std::string*) gameLogic->ammoGaugeImage.second["texture"]) = "gauge/shell";
	((UIMetrics*) gameLogic->ammoGaugeImage.second["metrics"])->bearing1 = UIMetrics::BEARING_LEFT;
	((UIMetrics*) gameLogic->ammoGaugeImage.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->ammoGaugeImage.second["metrics"])->size = ((DrawTexture*) gameGraphics->drawers["texture"])->getSize(gameLogic->ammoGaugeImage.second);
	gameLogic->drawStack.push_back(gameLogic->ammoGaugeImage);

	// ammo gauge bar
	*((Vector4*) gameLogic->ammoGaugeBar.second["color1Top"]) = gameSystem->getColor("hudGaugeAmmoBarColor");
	*((Vector4*) gameLogic->ammoGaugeBar.second["color1Bottom"]) = Vector4(
			gameSystem->getColor("hudGaugeAmmoBarColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor("hudGaugeAmmoBarColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor("hudGaugeAmmoBarColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor("hudGaugeAmmoBarColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	*((Vector4*) gameLogic->ammoGaugeBar.second["color2Top"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) gameLogic->ammoGaugeBar.second["color2Bottom"]) = Vector4(
			gameSystem->getColor("hudGaugeBackgroundColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor("hudGaugeBackgroundColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor("hudGaugeBackgroundColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor("hudGaugeBackgroundColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	*((float*) gameLogic->ammoGaugeBar.second["progression"]) = 0.25f;
	*((Vector2*) gameLogic->ammoGaugeBar.second["size"]) = Vector2(
			gameSystem->getFloat("hudGaugeWidth"),
			gameSystem->getFloat("hudGaugeHeight")
		);
	((UIMetrics*) gameLogic->ammoGaugeBar.second["metrics"])->size = ((DrawProgressBar*) gameGraphics->drawers["progressBar"])->getSize(gameLogic->ammoGaugeBar.second);
	gameLogic->drawStack.push_back(gameLogic->ammoGaugeBar);

	// shock gauge image
	*((std::string*) gameLogic->shockGaugeImage.second["texture"]) = "gauge/bolt";
	((UIMetrics*) gameLogic->shockGaugeImage.second["metrics"])->bearing1 = UIMetrics::BEARING_LEFT;
	((UIMetrics*) gameLogic->shockGaugeImage.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->shockGaugeImage.second["metrics"])->size = ((DrawTexture*) gameGraphics->drawers["texture"])->getSize(gameLogic->shockGaugeImage.second);
	gameLogic->drawStack.push_back(gameLogic->shockGaugeImage);

	// shock gauge bar
	*((Vector4*) gameLogic->shockGaugeBar.second["color1Top"]) = gameSystem->getColor("hudGaugeShockChargingBarColor");
	*((Vector4*) gameLogic->shockGaugeBar.second["color1Bottom"]) = Vector4(
			gameSystem->getColor("hudGaugeShockChargingBarColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor("hudGaugeShockChargingBarColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor("hudGaugeShockChargingBarColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor("hudGaugeShockChargingBarColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	*((Vector4*) gameLogic->shockGaugeBar.second["color2Top"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) gameLogic->shockGaugeBar.second["color2Bottom"]) = Vector4(
			gameSystem->getColor("hudGaugeBackgroundColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor("hudGaugeBackgroundColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor("hudGaugeBackgroundColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor("hudGaugeBackgroundColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	*((float*) gameLogic->shockGaugeBar.second["progression"]) = 0.5f;
	*((Vector2*) gameLogic->shockGaugeBar.second["size"]) = Vector2(
			gameSystem->getFloat("hudGaugeWidth"),
			gameSystem->getFloat("hudGaugeHeight")
		);
	((UIMetrics*) gameLogic->shockGaugeBar.second["metrics"])->size = ((DrawProgressBar*) gameGraphics->drawers["progressBar"])->getSize(gameLogic->shockGaugeBar.second);
	gameLogic->drawStack.push_back(gameLogic->shockGaugeBar);

	// fix metrics for gauge images and bars and re-arrange the UI
	Vector2 maxSize(0.0f, 0.0f);

	if(((UIMetrics*) gameLogic->healthGaugeImage.second["metrics"])->size.x > maxSize.x) maxSize.x = ((UIMetrics*) gameLogic->healthGaugeImage.second["metrics"])->size.x;
	if(((UIMetrics*) gameLogic->ammoGaugeImage.second["metrics"])->size.x > maxSize.x) maxSize.x = ((UIMetrics*) gameLogic->ammoGaugeImage.second["metrics"])->size.x;
	if(((UIMetrics*) gameLogic->shockGaugeImage.second["metrics"])->size.x > maxSize.x) maxSize.x = ((UIMetrics*) gameLogic->shockGaugeImage.second["metrics"])->size.x;

	if(((UIMetrics*) gameLogic->healthGaugeImage.second["metrics"])->size.y > maxSize.y) maxSize.y = ((UIMetrics*) gameLogic->healthGaugeImage.second["metrics"])->size.y;
	if(((UIMetrics*) gameLogic->healthGaugeBar.second["metrics"])->size.y > maxSize.y) maxSize.y = ((UIMetrics*) gameLogic->healthGaugeBar.second["metrics"])->size.y;
	if(((UIMetrics*) gameLogic->ammoGaugeImage.second["metrics"])->size.y > maxSize.y) maxSize.y = ((UIMetrics*) gameLogic->ammoGaugeImage.second["metrics"])->size.y;
	if(((UIMetrics*) gameLogic->ammoGaugeBar.second["metrics"])->size.y > maxSize.y) maxSize.y = ((UIMetrics*) gameLogic->ammoGaugeBar.second["metrics"])->size.y;
	if(((UIMetrics*) gameLogic->shockGaugeImage.second["metrics"])->size.y > maxSize.y) maxSize.y = ((UIMetrics*) gameLogic->shockGaugeImage.second["metrics"])->size.y;
	if(((UIMetrics*) gameLogic->shockGaugeBar.second["metrics"])->size.y > maxSize.y) maxSize.y = ((UIMetrics*) gameLogic->shockGaugeBar.second["metrics"])->size.y;

	((UIMetrics*) gameLogic->healthGaugeImage.second["metrics"])->size = maxSize;
	((UIMetrics*) gameLogic->ammoGaugeImage.second["metrics"])->size = maxSize;
	((UIMetrics*) gameLogic->shockGaugeImage.second["metrics"])->size = maxSize;

	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->healthGaugeImage.second["metrics"]);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->ammoGaugeImage.second["metrics"]);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->shockGaugeImage.second["metrics"]);

	gameLogic->uiLayoutAuthority->rearrange();

	float gaugeBarXCoordinate =
			((UIMetrics*) gameLogic->healthGaugeImage.second["metrics"])->position.x +
			((UIMetrics*) gameLogic->healthGaugeImage.second["metrics"])->size.x / 2.0f +
			gameLogic->uiLayoutAuthority->elementMargin.x +
			((UIMetrics*) gameLogic->healthGaugeBar.second["metrics"])->size.x / 2.0f;
	((UIMetrics*) gameLogic->healthGaugeBar.second["metrics"])->position = Vector2(gaugeBarXCoordinate, ((UIMetrics*) gameLogic->healthGaugeImage.second["metrics"])->position.y);
	((UIMetrics*) gameLogic->ammoGaugeBar.second["metrics"])->position = Vector2(gaugeBarXCoordinate, ((UIMetrics*) gameLogic->ammoGaugeImage.second["metrics"])->position.y);
	((UIMetrics*) gameLogic->shockGaugeBar.second["metrics"])->position = Vector2(gaugeBarXCoordinate, ((UIMetrics*) gameLogic->shockGaugeImage.second["metrics"])->position.y);

/*

	// repeatedly used variables
	std::string fontColorLight = gameSystem->getString("fontColorLight");
	std::string fontColorDark = gameSystem->getString("fontColorDark");
	std::stringstream textLines;


	// control box
	*((float*) gameLogic->controlBoxEntry.second["spotSize"]) = gameSystem->getFloat("hudControlBoxSpotSize");
	*((float*) gameLogic->controlBoxEntry.second["boxSize"]) = gameSystem->getFloat("hudControlBoxSize");
	*((Vector4*) gameLogic->controlBoxEntry.second["color"]) = gameSystem->getColor("hudControlBoxColor");

	gameLogic->drawStack.push_back(gameLogic->controlBoxEntry);

	// player info
	((UIMetrics*) gameLogic->playerInfoEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_RIGHT;
	((UIMetrics*) gameLogic->playerInfoEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->playerInfoEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->playerInfoEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");

	time_t rawTime;
	struct tm* timeInfo;
	time(&rawTime);
	timeInfo = localtime(&rawTime);

	char timeString[26];
	strftime(timeString, 25, "%a %b %d %Y %H:%M:%S", timeInfo);

	textLines << "\\" << fontColorDark << "Time: " << "\\" << fontColorLight << timeString << "\n\n";

	textLines << "\\" << fontColorDark << "Callsign: " << "\\" << fontColorLight << callsignText << "\n";

	textLines << "\\" << fontColorDark << "Team: " << "\\" << fontColorLight;
*/
/*
	switch(gameState->ships[0]->team) {
	case Ship::TEAM_DESERT:
		textLines << "Desert\n";

		break;
	case Ship::TEAM_FOREST:
		textLines << "Forest\n";

		break;
	case Ship::TEAM_AQUA:
		textLines << "Aqua\n";

		break;
	}

	textLines << "\\" << fontColorDark << "Score: " << "\\" << fontColorLight << "0 (0 - 0)\n\n";

	textLines << "\\" << fontColorDark << "Power: " << "\\" << fontColorLight << gameState->ships[0]->power * 100.0f << "%";
*/
/*
	*((std::string*) gameLogic->playerInfoEntry.second["text"]) = textLines.str();
	*((float*) gameLogic->playerInfoEntry.second["wrap"]) = 2.0f -
			gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX -
			gameSystem->getFloat("hudContainerPadding") * 2.0f / (float) gameGraphics->resolutionX;
	*((Vector4*) gameLogic->playerInfoEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->playerInfoEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerHighlightColor");
	*((Vector4*) gameLogic->playerInfoEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->playerInfoEntry.second["outsideColor"]) =
			Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	*((float*) gameLogic->playerInfoEntry.second["padding"]) =
			gameSystem->getFloat("hudContainerPadding");
	*((float*) gameLogic->playerInfoEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");

	((UIMetrics*) gameLogic->playerInfoEntry.second["metrics"])->size =
			((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->playerInfoEntry.second);

	gameLogic->drawStack.push_back(gameLogic->playerInfoEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->playerInfoEntry.second["metrics"]);

	// debug info
	if(showDebugInfo) {
		((UIMetrics*) gameLogic->debugInfoEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_RIGHT;
		((UIMetrics*) gameLogic->debugInfoEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
		*((float*) gameLogic->debugInfoEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
		*((Vector4*) gameLogic->debugInfoEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");

		textLines.str("");

		textLines << "\\" << fontColorDark << "FPS: " << "\\" << fontColorLight << gameGraphics->currentFPS << " ";
		textLines << "\\" << fontColorDark << "(target: " << "\\" << fontColorLight << gameSystem->getFloat("displayFPS") << ")\n";

		textLines << "\\" << fontColorDark << "Frame Time: " << "\\" << fontColorLight << gameGraphics->frameTime << "ms\n";

		char execTimeStr[13];
		sprintf(execTimeStr, "%1.1f", round((float) platform->getExecMills() / 1000.0f, 1));
		textLines << "\\" << fontColorDark << "Execution Time: " << "\\" << fontColorLight <<
				execTimeStr;
*/
/*
		textLines <<
				"\\ff0000ff" << "C" <<
				"\\00ff00ff" << "O" <<
				"\\0000ffff" << "L" <<
				"\\ff00ffff" << "O" <<
				"\\ffff00ff" << "R";
*/
/*
		*((std::string*) gameLogic->debugInfoEntry.second["text"]) = textLines.str();
		*((float*) gameLogic->debugInfoEntry.second["wrap"]) = 2.0f -
						gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX -
						gameSystem->getFloat("hudContainerPadding") * 2.0f / (float) gameGraphics->resolutionX;
		*((Vector4*) gameLogic->debugInfoEntry.second["insideColor"]) =
						gameSystem->getColor("hudContainerInsideColor");
		*((Vector4*) gameLogic->debugInfoEntry.second["highlightColor"]) =
						gameSystem->getColor("hudContainerHighlightColor");
		*((Vector4*) gameLogic->debugInfoEntry.second["borderColor"]) =
						gameSystem->getColor("hudContainerBorderColor");
		*((Vector4*) gameLogic->debugInfoEntry.second["outsideColor"]) =
						Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		*((float*) gameLogic->debugInfoEntry.second["padding"]) =
						gameSystem->getFloat("hudContainerPadding");
		*((float*) gameLogic->debugInfoEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");

		((UIMetrics*) gameLogic->debugInfoEntry.second["metrics"])->size =
						((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->debugInfoEntry.second);

		gameLogic->drawStack.push_back(gameLogic->debugInfoEntry);
		gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->debugInfoEntry.second["metrics"]);
	}

	// console
	addConsole();

	// radar
	addRadar();

	// cursor
	*((float*) gameLogic->cursorEntry.second["size"]) = gameSystem->getFloat("hudCursorSize");
	*((float*) gameLogic->cursorEntry.second["thickness"]) = gameSystem->getFloat("hudCursorThickness");
	*((Vector4*) gameLogic->cursorEntry.second["color"]) = gameSystem->getColor("hudCursorColor");
	*((Vector2**) gameLogic->cursorEntry.second["position"]) = &(inputHandler->mouse->position);

	gameLogic->drawStack.push_back(gameLogic->cursorEntry);

	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();

	// fix metrics for elements that don't have their own layout authority entry
	((UIMetrics*) gameLogic->radarEntry.second["metrics"])->position =
			((UIMetrics*) gameLogic->radarContainerEntry.second["metrics"])->position;

	// listeners
	inputHandler->keyboard->clearListeners();
	inputHandler->mouse->clearListeners();

	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->dashboardKeyListener);
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->testKeyListener);

	inputHandler->keyboard->unicodeChars = "";
	inputHandler->keyboard->listenUnicode = false;
	inputHandler->keyboard->addListener(gameLogic->testKeyAbsoluteListener);
	inputHandler->mouse->addListener(gameLogic->cursorMovementListener);
*/
}

void Schemes::welcomeScheme(
			std::string callsignText
		) {
	// join info
	((UIMetrics*) gameLogic->joinContainerEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((Vector4*) gameLogic->joinContainerEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->joinContainerEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->joinContainerEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->joinContainerEntry.second["outsideColor"]) =
			Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	*((float*) gameLogic->joinContainerEntry.second["padding"]) =
			gameSystem->getFloat("hudContainerPadding");
	*((float*) gameLogic->joinContainerEntry.second["border"]) = 0.0f;

	*((float*) gameLogic->joinHeaderLabelEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->joinHeaderLabelEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->joinHeaderLabelEntry.second["text"]) = "PLAYER INFO";

	((UIMetrics*) gameLogic->joinHeaderLabelEntry.second["metrics"])->size =
			((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->joinHeaderLabelEntry.second);

	*((float*) gameLogic->joinCallsignLabelEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->joinCallsignLabelEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->joinCallsignLabelEntry.second["text"]) = "Callsign:";

	((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->size =
			((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->joinCallsignLabelEntry.second);

	((UIMetrics*) gameLogic->joinCallsignFieldEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->joinCallsignFieldEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->joinCallsignFieldEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->joinCallsignFieldEntry.second["text"]) = "";
	for(unsigned int i = 0; i < gameSystem->getFloat("hudFieldWidth"); ++i)
		*((std::string*) gameLogic->joinCallsignFieldEntry.second["text"]) += "#";
	*((Vector4*) gameLogic->joinCallsignFieldEntry.second["boxColor"]) = gameSystem->getColor("hudFieldInactiveColor");

	((UIMetrics*) gameLogic->joinCallsignFieldEntry.second["metrics"])->size =
			((DrawField*) gameGraphics->drawers["field"])->getSize(gameLogic->joinCallsignFieldEntry.second);

	*((std::string*) gameLogic->joinCallsignFieldEntry.second["text"]) = callsignText.c_str();

	*((float*) gameLogic->joinButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->joinButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->joinButtonEntry.second["text"]) = "Start";
	*((Vector4*) gameLogic->joinButtonEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->joinButtonEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->joinButtonEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->joinButtonEntry.second["outsideColor"]) =
			Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	*((float*) gameLogic->joinButtonEntry.second["padding"]) =
			gameSystem->getFloat("hudButtonPadding");
	*((float*) gameLogic->joinButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	if(gameLogic->startButtonZoneListener->isEntered)
		*((Vector4*) gameLogic->joinButtonEntry.second["insideColor"]) =
				gameSystem->getColor("hudContainerHighlightColor");

	((UIMetrics*) gameLogic->joinButtonEntry.second["metrics"])->size =
			((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->joinButtonEntry.second);

	Vector2* containerSize = &(((UIMetrics*) gameLogic->joinContainerEntry.second["metrics"])->size);
	containerSize->x = 0.0f; containerSize->y = 0.0f;

	if(((UIMetrics*) gameLogic->joinHeaderLabelEntry.second["metrics"])->size.x > containerSize->x)
		containerSize->x = ((UIMetrics*) gameLogic->joinHeaderLabelEntry.second["metrics"])->size.x;
	if(
			((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->size.x +
			gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionX * 2.0f +
			((UIMetrics*) gameLogic->joinCallsignFieldEntry.second["metrics"])->size.x >
			containerSize->x
		)
		containerSize->x =
				((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->size.x +
				gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionX * 2.0f +
				((UIMetrics*) gameLogic->joinCallsignFieldEntry.second["metrics"])->size.x;
	if(((UIMetrics*) gameLogic->joinButtonEntry.second["metrics"])->size.x > containerSize->x)
		containerSize->x = ((UIMetrics*) gameLogic->joinButtonEntry.second["metrics"])->size.x;
	containerSize->x +=
			(gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionX * 2.0f) * 2.0f;

	if(
			((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->size.y >
			((UIMetrics*) gameLogic->joinCallsignFieldEntry.second["metrics"])->size.y
		)
		containerSize->y = ((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->size.y;
	else
		containerSize->y = ((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->size.y;

	containerSize->y +=
			((UIMetrics*) gameLogic->joinHeaderLabelEntry.second["metrics"])->size.y +
			((UIMetrics*) gameLogic->joinButtonEntry.second["metrics"])->size.y +
			(gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionY * 2.0f) * 4.0f;

	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->joinContainerEntry.second["metrics"]);

	gameLogic->drawStack.push_back(gameLogic->joinContainerEntry);
	gameLogic->drawStack.push_back(gameLogic->joinHeaderLabelEntry);
	gameLogic->drawStack.push_back(gameLogic->joinCallsignLabelEntry);
	gameLogic->drawStack.push_back(gameLogic->joinCallsignFieldEntry);
	gameLogic->drawStack.push_back(gameLogic->joinButtonEntry);

	// welcome help
	((UIMetrics*) gameLogic->welcomeHelpEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->welcomeHelpEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->welcomeHelpEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	std::stringstream welcomeInfo;
	welcomeInfo <<
			"To begin playing, enter a callsign above and click \"Start.\"\n\n" <<
			"To quit, press F12 or click the button in the upper-right corner.";
	*((std::string*) gameLogic->welcomeHelpEntry.second["text"]) = welcomeInfo.str();
	*((float*) gameLogic->welcomeHelpEntry.second["wrap"]) = 2.0f -
			gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX -
			gameSystem->getFloat("hudContainerPadding") * 2.0f / (float) gameGraphics->resolutionX;
	*((Vector4*) gameLogic->welcomeHelpEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->welcomeHelpEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->welcomeHelpEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->welcomeHelpEntry.second["outsideColor"]) =
			Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	*((float*) gameLogic->welcomeHelpEntry.second["padding"]) =
			gameSystem->getFloat("hudContainerPadding");
	*((float*) gameLogic->welcomeHelpEntry.second["border"]) = 0.0f;

	((UIMetrics*) gameLogic->welcomeHelpEntry.second["metrics"])->size =
			((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->welcomeHelpEntry.second);

	gameLogic->drawStack.push_back(gameLogic->welcomeHelpEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->welcomeHelpEntry.second["metrics"]);

	// console
//	addConsole();

	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();

	// fix metrics for elements that don't have their own layout authority entry
	((UIMetrics*) gameLogic->joinHeaderLabelEntry.second["metrics"])->position.x =
			((UIMetrics*) gameLogic->joinContainerEntry.second["metrics"])->position.x;
	((UIMetrics*) gameLogic->joinHeaderLabelEntry.second["metrics"])->position.y =
			((UIMetrics*) gameLogic->joinContainerEntry.second["metrics"])->position.y +
			((UIMetrics*) gameLogic->joinContainerEntry.second["metrics"])->size.y / 2.0f -
			gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionY * 2.0f -
			((UIMetrics*) gameLogic->joinHeaderLabelEntry.second["metrics"])->size.y / 2.0f;

	((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->position.x =
			((UIMetrics*) gameLogic->joinContainerEntry.second["metrics"])->position.x -
			((UIMetrics*) gameLogic->joinContainerEntry.second["metrics"])->size.x / 2.0f +
			gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionX * 2.0f +
			((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->size.x / 2.0f;
	((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->position.y =
			((UIMetrics*) gameLogic->joinHeaderLabelEntry.second["metrics"])->position.y -
			((UIMetrics*) gameLogic->joinHeaderLabelEntry.second["metrics"])->size.y / 2.0f -
			gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionY * 2.0f -
			((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->size.y / 2.0f;

	((UIMetrics*) gameLogic->joinCallsignFieldEntry.second["metrics"])->position.x =
			((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->position.x +
			((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->size.x / 2.0f +
			gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionX * 2.0f +
			((UIMetrics*) gameLogic->joinCallsignFieldEntry.second["metrics"])->size.x / 2.0f;
	((UIMetrics*) gameLogic->joinCallsignFieldEntry.second["metrics"])->position.y =
			((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->position.y;

	((UIMetrics*) gameLogic->joinButtonEntry.second["metrics"])->position.x =
			((UIMetrics*) gameLogic->joinContainerEntry.second["metrics"])->position.x;
	((UIMetrics*) gameLogic->joinButtonEntry.second["metrics"])->position.y =
			((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->position.y -
			((UIMetrics*) gameLogic->joinCallsignLabelEntry.second["metrics"])->size.y / 2.0f -
			gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionY * 2.0f -
			((UIMetrics*) gameLogic->joinButtonEntry.second["metrics"])->size.y / 2.0f;

	// update the mouse zone listeners for buttons
	gameLogic->quitButtonZoneListener->ll =
			((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->position -
			((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->quitButtonZoneListener->ur =
			((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->position +
			((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size / 2.0f;

	gameLogic->quitButtonClickListener->ll = gameLogic->quitButtonZoneListener->ll;
	gameLogic->quitButtonClickListener->ur = gameLogic->quitButtonZoneListener->ur;

	gameLogic->startButtonZoneListener->ll =
			((UIMetrics*) gameLogic->joinButtonEntry.second["metrics"])->position -
			((UIMetrics*) gameLogic->joinButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->startButtonZoneListener->ur =
			((UIMetrics*) gameLogic->joinButtonEntry.second["metrics"])->position +
			((UIMetrics*) gameLogic->joinButtonEntry.second["metrics"])->size / 2.0f;

	gameLogic->startButtonClickListener->ll = gameLogic->startButtonZoneListener->ll;
	gameLogic->startButtonClickListener->ur = gameLogic->startButtonZoneListener->ur;

	// listeners
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->deleteKeyListener);
//	inputHandler->keyboard->addListener(gameLogic->testKeyListener);

	inputHandler->keyboard->unicodeChars = "";
	inputHandler->keyboard->listenUnicode = true;

//	inputHandler->mouse->addListener(gameLogic->cursorMovementListener);
	inputHandler->mouse->addListener(gameLogic->quitButtonZoneListener);
	inputHandler->mouse->addListener(gameLogic->quitButtonClickListener);
	inputHandler->mouse->addListener(gameLogic->startButtonZoneListener);
	inputHandler->mouse->addListener(gameLogic->startButtonClickListener);
}





void Schemes::dashboardScheme() {
	// repeatedly used variables
	std::string fontColorLight = gameSystem->getString("fontColorLight");
	std::string fontColorDark = gameSystem->getString("fontColorDark");
	std::stringstream textLines;

	// water
	gameLogic->drawStack.push_back(gameLogic->waterEntry);

	// ships
	gameLogic->drawStack.push_back(gameLogic->shipEntry);

	// terrains
	gameLogic->drawStack.push_back(gameLogic->terrainEntry);

	// console
//	addConsole();

	// radar
//	addRadar();

	// gray out
	*((Vector4*) gameLogic->grayOutEntry.second["color"]) =
			Vector4(0.6f, 0.6f, 0.6f, 0.4f);
	gameLogic->drawStack.push_back(gameLogic->grayOutEntry);

	// quit button
//	addQuitButton();

	// controls help
	((UIMetrics*) gameLogic->controlsHelpEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_LEFT;
	((UIMetrics*) gameLogic->controlsHelpEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->controlsHelpEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->controlsHelpEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	textLines.str("");
/*
textLines <<
		"abg\n"	<<
//		"abg\tabg\tabg\n" <<
		"abg\tabg\n" <<
		"abg\n" <<
		"abg\tabg\tabg\n";
*/
	textLines <<
			"GENERAL CONTROLS\n\n" <<
			"Toggle Dashboard:\tShift\n" <<
			"Toggle Fullscreen:\tF1\n" <<
//			"Minimize:\tF2\n" <<
			"Quit:\tF12\n\n"
//			"Cancel:\tesc\n\n" <<
			"PLAYING CONTROLS\n\n" <<
			"Accelerate:\tSpace, Mouse Button 2\n" <<
//			"Primary Fire:\tD, Mouse Button 1\n" <<
//			"Secondary Fire:\tF, Mouse Button 3\n" <<
			"Movement:\tArrow Keys, Mouse\n\n" <<
//			"Toggle Info Display:\tF3\n" <<
//			"Toggle Radar Display:\tF4\n" <<
//			"Toggle Extended Info Display:\tF5\n\n" <<
//			"DASHBOARD CONTROLS\n\n" <<
//			"Send Public Message:\tReturn\n" <<
//			"Send Team Message:\tC\n" <<
//			"Send Admin Report:\tX\n" <<
//			"Send Server Command:\tZ\n\n" <<
"Volume\t[                   +]\n" <<
"Volume\t---------------------|\n\n" <<
			"DEBUGGING CONTROLS\n\n" <<
			"Regenerate Terrain\tT";
    
	*((std::string*) gameLogic->controlsHelpEntry.second["text"]) = textLines.str();
	*((float*) gameLogic->controlsHelpEntry.second["wrap"]) = 2.0f -
			gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX -
			gameSystem->getFloat("hudContainerPadding") * 2.0f / (float) gameGraphics->resolutionX;
	*((Vector4*) gameLogic->controlsHelpEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->controlsHelpEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->controlsHelpEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->controlsHelpEntry.second["outsideColor"]) =
			Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	*((float*) gameLogic->controlsHelpEntry.second["padding"]) =
			gameSystem->getFloat("hudContainerPadding");
	*((float*) gameLogic->controlsHelpEntry.second["border"]) = 0.0f;

	((UIMetrics*) gameLogic->controlsHelpEntry.second["metrics"])->size =
			((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->controlsHelpEntry.second);

	gameLogic->drawStack.push_back(gameLogic->controlsHelpEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->controlsHelpEntry.second["metrics"]);

	// scoreboard
	((UIMetrics*) gameLogic->scoreboardEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_RIGHT;
	((UIMetrics*) gameLogic->scoreboardEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->scoreboardEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->scoreboardEntry.second["fontColor"]) = Vector4(0.0f, 0.0f, 0.0f, 0.0f); //gameSystem->getColor("fontColorLight");

	textLines.str("");
	textLines <<
			"\\" << fontColorLight << "SCOREBOARD\n\n" <<
			"\\" << fontColorLight << "CALLSIGN\t" <<
			"\\" << fontColorLight << "SCORE\t" <<
			"\\" << fontColorLight << "PERCENT\t" <<
			"\\" << fontColorLight << "KILLS\n\n";

	textLines <<
			"\\fff7ebff" <<
			"Desert Team\t" <<
			"\\fff7ebff" << "0 (0 - 0)" << "\t" <<
			"\\fff7ebff" << "0" << "\t" <<
			"\\fff7ebff" << "\n";

	textLines <<
			"\\3cd385ff" <<
			"Forest Team\t" <<
			"\\3cd385ff" << "0 (0 - 0)" << "\t" <<
			"\\3cd385ff" << "0" << "\t" <<
			"\\3cd385ff" << "\n";

	textLines <<
			"\\47eaffff" <<
			"Aqua Team\t" <<
			"\\47eaffff" << "0 (0 - 0)" << "\t" <<
			"\\47eaffff" << "0" << "\t" <<
			"\\47eaffff" << "\n\n";


	textLines <<
			"\\" << fontColorLight << gameLogic->myCallsign << "\t" <<
			"\\" << fontColorLight << "0 (0 - 0)" << "\t" <<
			"\\" << fontColorLight << "0" << "\t" <<
			"\\" << fontColorLight << "\n\n";

	*((std::string*) gameLogic->scoreboardEntry.second["text"]) = textLines.str();
	*((float*) gameLogic->scoreboardEntry.second["wrap"]) = 2.0f -
			gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX -
			gameSystem->getFloat("hudContainerPadding") * 2.0f / (float) gameGraphics->resolutionX;
	*((Vector4*) gameLogic->scoreboardEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->scoreboardEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->scoreboardEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->scoreboardEntry.second["outsideColor"]) =
			Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	*((float*) gameLogic->scoreboardEntry.second["padding"]) =
			gameSystem->getFloat("hudContainerPadding");
	*((float*) gameLogic->scoreboardEntry.second["border"]) = 0.0f;

	((UIMetrics*) gameLogic->scoreboardEntry.second["metrics"])->size =
			((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->scoreboardEntry.second);

	gameLogic->drawStack.push_back(gameLogic->scoreboardEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->scoreboardEntry.second["metrics"]);

	// hint
/*
	if(gameLogic->hintExpiration > platform->getExecMills()) {
		*((float*) gameLogic->hintEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
		*((Vector4*) gameLogic->hintEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
		textLines.str("");
		textLines << "Press Shift to return";
		*((std::string*) gameLogic->hintEntry.second["text"]) = textLines.str();
		*((float*) gameLogic->hintEntry.second["wrap"]) = 2.0f -
				gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX -
				gameSystem->getFloat("hudContainerPadding") * 2.0f / (float) gameGraphics->resolutionX;
		*((Vector4*) gameLogic->hintEntry.second["insideColor"]) =
				gameSystem->getColor("hudContainerInsideColor");
		*((Vector4*) gameLogic->hintEntry.second["highlightColor"]) =
				gameSystem->getColor("hudContainerInsideColor");
		*((Vector4*) gameLogic->hintEntry.second["borderColor"]) =
				gameSystem->getColor("hudContainerInsideColor");
		*((Vector4*) gameLogic->hintEntry.second["outsideColor"]) =
				Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		*((float*) gameLogic->hintEntry.second["padding"]) =
				gameSystem->getFloat("hudContainerPadding");
		*((float*) gameLogic->hintEntry.second["border"]) = 0.0f;

		((UIMetrics*) gameLogic->hintEntry.second["metrics"])->size =
				((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->hintEntry.second);
		((UIMetrics*) gameLogic->hintEntry.second["metrics"])->position = Vector2(0.0f, 0.5f);

		gameLogic->drawStack.push_back(gameLogic->hintEntry);
	}
*/
	// re-arrange the UI
	gameLogic->uiLayoutAuthority->rearrange();

	// update the mouse zone listeners for buttons
	gameLogic->quitButtonZoneListener->ll =
			((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->position -
			((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->quitButtonZoneListener->ur =
			((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->position +
			((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size / 2.0f;

	gameLogic->quitButtonClickListener->ll = gameLogic->quitButtonZoneListener->ll;
	gameLogic->quitButtonClickListener->ur = gameLogic->quitButtonZoneListener->ur;

	// listeners
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->dashboardKeyListener);
	inputHandler->keyboard->addListener(gameLogic->testKeyListener);

	inputHandler->keyboard->unicodeChars = "";
	inputHandler->keyboard->listenUnicode = false;
	inputHandler->keyboard->addListener(gameLogic->testKeyAbsoluteListener);
	inputHandler->mouse->addListener(gameLogic->cursorMovementListener);
	inputHandler->mouse->addListener(gameLogic->quitButtonZoneListener);
	inputHandler->mouse->addListener(gameLogic->quitButtonClickListener);
}

/*
void Schemes::addQuitButton() {
	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_RIGHT;
	*((float*) gameLogic->quitButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->quitButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->quitButtonEntry.second["text"]) = "Quit";
	*((Vector4*) gameLogic->quitButtonEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->quitButtonEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->quitButtonEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->quitButtonEntry.second["outsideColor"]) = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	*((float*) gameLogic->quitButtonEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
	*((float*) gameLogic->quitButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	if(gameLogic->quitButtonZoneListener->isEntered)
		*((Vector4*) gameLogic->quitButtonEntry.second["insideColor"]) =
				gameSystem->getColor("hudContainerHighlightColor");

	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size =
			((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->quitButtonEntry.second);

	gameLogic->drawStack.push_back(gameLogic->quitButtonEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"]);
}

void Schemes::addConsole() {
	((UIMetrics*) gameLogic->consoleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->consoleEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_LEFT;
	*((float*) gameLogic->consoleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->consoleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");

	std::stringstream textLines;
	std::string fontColorLight = gameSystem->getString("fontColorLight");
	std::string fontColorDark = gameSystem->getString("fontColorDark");

	for(
			int i = (int) gameSystem->logLines.size() - gameSystem->getFloat("hudConsoleMaxLines");
			i < (int) gameSystem->logLines.size();
			++i
		) {
		if(i < 0)
			continue;

		std::string thisLine = gameSystem->logLines[i];

		unsigned int lineTime = (unsigned int) atol(thisLine.c_str());
		lineTime /= 100;
		char lineTimeStr[10];
		sprintf(lineTimeStr, "%03.1f", (float) lineTime / 10.0f);

		if(thisLine.find(" ") != std::string::npos)
			thisLine = thisLine.substr(thisLine.find(" ") + 1);

		std::string linePrefix = "";

		if(thisLine.substr(0, 6) == "INFO: ") {
			linePrefix = "INFO: ";
			thisLine = thisLine.substr(thisLine.find(" ") + 1);
		} else if(thisLine.substr(0, 9) == "VERBOSE: ") {
			linePrefix = "VERBOSE: ";
			thisLine = thisLine.substr(thisLine.find(" ") + 1);
		}

		if(linePrefix != "")
			textLines << 
//					lineTimeStr << " " <<
					"\\" << fontColorDark << linePrefix << " " <<
					"\\" << fontColorLight << thisLine;
		else
			textLines << 
//					lineTimeStr << " " <<
					thisLine;

		if(i + 1 < (int) gameSystem->logLines.size())
			textLines << "\n";
	}
	*((std::string*) gameLogic->consoleEntry.second["text"]) = textLines.str();
	*((float*) gameLogic->consoleEntry.second["wrap"]) = 2.0f -
			gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX -
			gameSystem->getFloat("hudContainerPadding") * 2.0f / (float) gameGraphics->resolutionX;
	*((Vector4*) gameLogic->consoleEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->consoleEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerHighlightColor");
	*((Vector4*) gameLogic->consoleEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->consoleEntry.second["outsideColor"]) =
			Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	*((float*) gameLogic->consoleEntry.second["padding"]) =
			gameSystem->getFloat("hudContainerPadding");
	*((float*) gameLogic->consoleEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");

	((UIMetrics*) gameLogic->consoleEntry.second["metrics"])->size =
			((DrawButton*) gameGraphics->drawers["button"])->getSize(gameLogic->consoleEntry.second);

	gameLogic->drawStack.push_back(gameLogic->consoleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->consoleEntry.second["metrics"]);
}

void Schemes::addRadar() {
	((UIMetrics*) gameLogic->radarContainerEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->radarContainerEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_RIGHT;
	((UIMetrics*) gameLogic->radarContainerEntry.second["metrics"])->size =
			((DrawRadar*) gameGraphics->drawers["radar"])->getSize(gameLogic->radarEntry.second) +
			Vector2(
					(gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionX * 2.0f) * 2.0f,
					(gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionY * 2.0f) * 2.0f
				);
	*((Vector4*) gameLogic->radarContainerEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) gameLogic->radarContainerEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerHighlightColor");
	*((Vector4*) gameLogic->radarContainerEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameLogic->radarContainerEntry.second["outsideColor"]) =
			Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	*((float*) gameLogic->radarContainerEntry.second["padding"]) =
			gameSystem->getFloat("hudContainerPadding");
	*((float*) gameLogic->radarContainerEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");

	((UIMetrics*) gameLogic->radarEntry.second["metrics"])->size =
			((DrawRadar*) gameGraphics->drawers["radar"])->getSize(gameLogic->radarEntry.second);

	gameLogic->drawStack.push_back(gameLogic->radarContainerEntry);
	gameLogic->drawStack.push_back(gameLogic->radarEntry);

	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->radarContainerEntry.second["metrics"]);
}
*/
