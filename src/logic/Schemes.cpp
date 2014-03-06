// Schemes.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "logic/Schemes.h"

void Schemes::addQuitButton() {
	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_RIGHT;
	*((float*) gameLogic->quitButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->quitButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->quitButtonEntry.second["text"]) = "Quit";
	*((Vector4*) gameLogic->quitButtonEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
	*((Vector4*) gameLogic->quitButtonEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContaineraltInsideColor");
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
	std::string fontColorMedium = gameSystem->getString("fontColorMedium");

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
			textLines << /*lineTimeStr << " " << */
					"\\" << fontColorMedium << linePrefix << " " <<
					"\\" << fontColorLight << thisLine;
		else
			textLines << /*lineTimeStr << " " << */ thisLine;

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

void Schemes::welcomeScheme(
			std::string callsignText
		) {
	// splash background
	gameLogic->drawStack.push_back(gameLogic->splashEntry);

	// title
	((UIMetrics*) gameLogic->titleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((float*) gameLogic->titleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->titleEntry.second["fontColor"]) = gameSystem->getColor("fontColorDark");
	*((std::string*) gameLogic->titleEntry.second["text"]) = "DOMINICUS";

	((UIMetrics*) gameLogic->titleEntry.second["metrics"])->size =
			((DrawLabel*) gameGraphics->drawers["label"])->getSize(gameLogic->titleEntry.second);

	gameLogic->drawStack.push_back(gameLogic->titleEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->titleEntry.second["metrics"]);

	// quit button
	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_RIGHT;
	*((float*) gameLogic->quitButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->quitButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->quitButtonEntry.second["text"]) = "Quit";
	*((Vector4*) gameLogic->quitButtonEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
	*((Vector4*) gameLogic->quitButtonEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContaineraltInsideColor");
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

	// logo
	((UIMetrics*) gameLogic->logoEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((std::string*) gameLogic->logoEntry.second["texture"]) = "logo/logo";

	((UIMetrics*) gameLogic->logoEntry.second["metrics"])->size =
			((DrawTexture*) gameGraphics->drawers["texture"])->getSize(gameLogic->logoEntry.second);

	gameLogic->drawStack.push_back(gameLogic->logoEntry);
	gameLogic->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->logoEntry.second["metrics"]);

	// join info
	((UIMetrics*) gameLogic->joinContainerEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	*((Vector4*) gameLogic->joinContainerEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
	*((Vector4*) gameLogic->joinContainerEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
	*((Vector4*) gameLogic->joinContainerEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
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
			gameSystem->getColor("hudContainerAltInsideColor");
	*((Vector4*) gameLogic->joinButtonEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
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
			gameSystem->getColor("hudContainerAltInsideColor");
	*((Vector4*) gameLogic->welcomeHelpEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
	*((Vector4*) gameLogic->welcomeHelpEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
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
	addConsole();

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

void Schemes::playingScheme(
			bool showHUD,
			std::string callsignText,
//			std::string team,
			std::string scoreText,
			std::string powerText,
			bool showDebugInfo
		) {
	// repeatedly used variables
	std::string fontColorLight = gameSystem->getString("fontColorLight");
	std::string fontColorMedium = gameSystem->getString("fontColorMedium");
	std::stringstream textLines;

	// sky
	gameLogic->drawStack.push_back(gameLogic->skyEntry);

	// water
	gameLogic->drawStack.push_back(gameLogic->waterEntry);

	// terrains
	gameLogic->drawStack.push_back(gameLogic->terrain1Entry);

	// ships
	gameLogic->drawStack.push_back(gameLogic->ship1Entry);

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

	textLines << "\\" << fontColorMedium << "Time: " << "\\" << fontColorLight << timeString << "\n\n";

	textLines << "\\" << fontColorMedium << "Callsign: " << "\\" << fontColorLight << callsignText << "\n";

	textLines << "\\" << fontColorMedium << "Team: " << "\\" << fontColorLight;

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

	textLines << "\\" << fontColorMedium << "Score: " << "\\" << fontColorLight << "0 (0 - 0)\n\n";

	textLines << "\\" << fontColorMedium << "Power: " << "\\" << fontColorLight << gameState->ships[0]->power * 100.0f << "%";

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

		textLines << "\\" << fontColorMedium << "FPS: " << "\\" << fontColorLight << gameGraphics->currentFPS << " ";
		textLines << "\\" << fontColorMedium << "(target: " << "\\" << fontColorLight << gameSystem->getFloat("displayFPS") << ")\n";

		textLines << "\\" << fontColorMedium << "Frame Time: " << "\\" << fontColorLight << gameGraphics->frameTime << "ms\n";

		char execTimeStr[13];
		sprintf(execTimeStr, "%1.1f", round((float) platform->getExecMills() / 1000.0f, 1));
		textLines << "\\" << fontColorMedium << "Execution Time: " << "\\" << fontColorLight <<
				execTimeStr;
/*
		textLines <<
				"\\ff0000ff" << "C" <<
				"\\00ff00ff" << "O" <<
				"\\0000ffff" << "L" <<
				"\\ff00ffff" << "O" <<
				"\\ffff00ff" << "R";
*/
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

	// hint
	if(gameLogic->hintExpiration > platform->getExecMills()) {
		*((float*) gameLogic->hintEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
		*((Vector4*) gameLogic->hintEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
		textLines.str("");
		textLines << "Press Shift for Dashboard";
		*((std::string*) gameLogic->hintEntry.second["text"]) = textLines.str();
		*((float*) gameLogic->hintEntry.second["wrap"]) = 2.0f -
				gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX -
				gameSystem->getFloat("hudContainerPadding") * 2.0f / (float) gameGraphics->resolutionX;
		*((Vector4*) gameLogic->hintEntry.second["insideColor"]) =
				gameSystem->getColor("hudContainerAltInsideColor");
		*((Vector4*) gameLogic->hintEntry.second["highlightColor"]) =
				gameSystem->getColor("hudContainerAltInsideColor");
		*((Vector4*) gameLogic->hintEntry.second["borderColor"]) =
				gameSystem->getColor("hudContainerAltInsideColor");
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
}

void Schemes::dashboardScheme() {
	// repeatedly used variables
	std::string fontColorLight = gameSystem->getString("fontColorLight");
	std::string fontColorMedium = gameSystem->getString("fontColorMedium");
	std::stringstream textLines;

	// water
	gameLogic->drawStack.push_back(gameLogic->waterEntry);

	// ships
	gameLogic->drawStack.push_back(gameLogic->ship1Entry);

	// terrains
	gameLogic->drawStack.push_back(gameLogic->terrain1Entry);

	// console
	addConsole();

	// radar
	addRadar();

	// gray out
	*((Vector4*) gameLogic->grayOutEntry.second["color"]) =
			Vector4(0.6f, 0.6f, 0.6f, 0.4f);
	gameLogic->drawStack.push_back(gameLogic->grayOutEntry);

	// quit button
	addQuitButton();

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
			"DEBUGGING CONTROLS\n\n" <<
			"Regenerate Terrain\tT";
    
	*((std::string*) gameLogic->controlsHelpEntry.second["text"]) = textLines.str();
	*((float*) gameLogic->controlsHelpEntry.second["wrap"]) = 2.0f -
			gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX -
			gameSystem->getFloat("hudContainerPadding") * 2.0f / (float) gameGraphics->resolutionX;
	*((Vector4*) gameLogic->controlsHelpEntry.second["insideColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
	*((Vector4*) gameLogic->controlsHelpEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
	*((Vector4*) gameLogic->controlsHelpEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
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
			gameSystem->getColor("hudContainerAltInsideColor");
	*((Vector4*) gameLogic->scoreboardEntry.second["highlightColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
	*((Vector4*) gameLogic->scoreboardEntry.second["borderColor"]) =
			gameSystem->getColor("hudContainerAltInsideColor");
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
				gameSystem->getColor("hudContainerAltInsideColor");
		*((Vector4*) gameLogic->hintEntry.second["highlightColor"]) =
				gameSystem->getColor("hudContainerAltInsideColor");
		*((Vector4*) gameLogic->hintEntry.second["borderColor"]) =
				gameSystem->getColor("hudContainerAltInsideColor");
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
