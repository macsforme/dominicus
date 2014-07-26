// Schemes.cpp
// Crucible Island

#include "logic/Schemes.h"

#include "graphics/DrawingMaster.h"
#include "graphics/GameGraphics.h"
#include "graphics/text/TextBlock.h"
#include "graphics/2dgraphics/DrawProgressBar.h"
#include "graphics/2dgraphics/DrawButton.h"
#include "graphics/2dgraphics/DrawContainer.h"
#include "graphics/2dgraphics/DrawLabel.h"
#include "graphics/2dgraphics/DrawField.h"
#include "graphics/2dgraphics/DrawGaugePanel.h"
#include "graphics/2dgraphics/DrawStrikeEffect.h"
#include "graphics/2dgraphics/DrawTexture.h"
#include "logic/GameLogic.h"
#include "graphics/UILayoutAuthority.h"
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "state/GameState.h"

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>

extern DrawingMaster* drawingMaster;
extern GameGraphics* gameGraphics;
extern GameLogic* gameLogic;
extern GameState* gameState;

void Schemes::mainMenuScheme() {
	// input
	inputHandler->mouse->addListener(gameLogic->mouseMotionListener);
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->mainMenuKeyListener);

	// splash background
	drawingMaster->drawStack.push_back(gameLogic->splashEntry);

	// spacer
	gameLogic->mainMenuSpacer1Metrics->bearing1 = UIMetrics::BEARING_TOP;
	drawingMaster->uiLayoutAuthority->metrics.push_back(gameLogic->mainMenuSpacer1Metrics);

	// title label
	*((float*) gameLogic->titleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSuper");
	*((Vector4*) gameLogic->titleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->titleEntry.second["text"]) = "CRUCIBLE ISLAND";
	((UIMetrics*) gameLogic->titleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->titleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->titleEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->titleEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->titleEntry.second["metrics"]);

	// game logo
	*((std::string*) gameLogic->logoEntry.second["texture"]) = "branding/logo";
	*((Vector2*) gameLogic->logoEntry.second["size"]) = Vector2(
			gameSystem->getFloat("logoHeight") * gameGraphics->getTexture("branding/logo")->width / gameGraphics->getTexture("branding/logo")->height / (float) gameGraphics->resolutionX * 2.0f,
			gameSystem->getFloat("logoHeight") / (float) gameGraphics->resolutionY * 2.0f
		);
	((UIMetrics*) gameLogic->logoEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->logoEntry.second["metrics"])->size = ((DrawTexture*) drawingMaster->drawers["texture"])->getSize(gameLogic->logoEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->logoEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->logoEntry.second["metrics"]);

	// spacer
	gameLogic->mainMenuSpacer2Metrics->bearing1 = UIMetrics::BEARING_TOP;
	drawingMaster->uiLayoutAuthority->metrics.push_back(gameLogic->mainMenuSpacer2Metrics);

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
	Vector2* playButtonSize = (Vector2*) gameLogic->playButtonEntry.second["size"];
	gameLogic->playButtonEntry.second.erase(gameLogic->playButtonEntry.second.find("size"));
	*playButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->playButtonEntry.second);
	gameLogic->playButtonEntry.second["size"] = playButtonSize;
	((UIMetrics*) gameLogic->playButtonEntry.second["metrics"])->size = *playButtonSize;
	drawingMaster->drawStack.push_back(gameLogic->playButtonEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->playButtonEntry.second["metrics"]);

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
	Vector2* settingsButtonSize = (Vector2*) gameLogic->settingsButtonEntry.second["size"];
	gameLogic->settingsButtonEntry.second.erase(gameLogic->settingsButtonEntry.second.find("size"));
	*settingsButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->settingsButtonEntry.second);
	gameLogic->settingsButtonEntry.second["size"] = settingsButtonSize;
	((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"])->size = *settingsButtonSize;
	drawingMaster->drawStack.push_back(gameLogic->settingsButtonEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"]);

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
	Vector2* highScoresButtonSize = (Vector2*) gameLogic->highScoresButtonEntry.second["size"];
	gameLogic->highScoresButtonEntry.second.erase(gameLogic->highScoresButtonEntry.second.find("size"));
	*highScoresButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->highScoresButtonEntry.second);
	gameLogic->highScoresButtonEntry.second["size"] = highScoresButtonSize;
	((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"])->size = *highScoresButtonSize;
	drawingMaster->drawStack.push_back(gameLogic->highScoresButtonEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"]);

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
	Vector2* helpButtonSize = (Vector2*) gameLogic->helpButtonEntry.second["size"];
	gameLogic->highScoresButtonEntry.second.erase(gameLogic->helpButtonEntry.second.find("size"));
	*helpButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->helpButtonEntry.second);
	gameLogic->helpButtonEntry.second["size"] = helpButtonSize;
	((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"])->size = *helpButtonSize;
	drawingMaster->drawStack.push_back(gameLogic->helpButtonEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"]);

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
	Vector2* quitButtonSize = (Vector2*) gameLogic->quitButtonEntry.second["size"];
	gameLogic->highScoresButtonEntry.second.erase(gameLogic->quitButtonEntry.second.find("size"));
	*quitButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->quitButtonEntry.second);
	gameLogic->quitButtonEntry.second["size"] = quitButtonSize;
	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size = *quitButtonSize;
	drawingMaster->drawStack.push_back(gameLogic->quitButtonEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"]);

	// helpful tip labels
	*((float*) gameLogic->menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip1Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip1Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip1Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"]);

	*((float*) gameLogic->menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip2Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip2Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"]);

	*((float*) gameLogic->menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip3Entry.second["text"]) = "Press ENTER to select";
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip3Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip3Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"]);

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
	*((Vector2*) gameLogic->playButtonEntry.second["size"]) =
			((UIMetrics*) gameLogic->playButtonEntry.second["metrics"])->size;
	((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"])->size.x = maxWidth;
	*((Vector2*) gameLogic->settingsButtonEntry.second["size"]) =
			((UIMetrics*) gameLogic->settingsButtonEntry.second["metrics"])->size;
	((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"])->size.x = maxWidth;
	*((Vector2*) gameLogic->helpButtonEntry.second["size"]) =
			((UIMetrics*) gameLogic->helpButtonEntry.second["metrics"])->size;
	((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"])->size.x = maxWidth;
	*((Vector2*) gameLogic->highScoresButtonEntry.second["size"]) =
			((UIMetrics*) gameLogic->highScoresButtonEntry.second["metrics"])->size;
	((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size.x = maxWidth;
	*((Vector2*) gameLogic->quitButtonEntry.second["size"]) =
			((UIMetrics*) gameLogic->quitButtonEntry.second["metrics"])->size;

	// re-arrange the UI
	drawingMaster->uiLayoutAuthority->rearrange();

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
	drawingMaster->drawStack.push_back(gameLogic->splashEntry);

	// help title label
	*((float*) gameLogic->helpTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->helpTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->helpTitleEntry.second["text"]) = "HELP";
	((UIMetrics*) gameLogic->helpTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->helpTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->helpTitleEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->helpTitleEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->helpTitleEntry.second["metrics"]);

	// controls title label
	*((float*) gameLogic->controlsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->controlsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->controlsTitleEntry.second["text"]) = "Controls";
	((UIMetrics*) gameLogic->controlsTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->controlsTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->controlsTitleEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->controlsTitleEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->controlsTitleEntry.second["metrics"]);

	// controls label
	*((float*) gameLogic->controlsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->controlsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->controlsEntry.second["text"]) = gameSystem->getString("textControls");
	((UIMetrics*) gameLogic->controlsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->controlsEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->controlsEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->controlsEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->controlsEntry.second["metrics"]);

	// instructions title label
	*((float*) gameLogic->instructionsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->instructionsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->instructionsTitleEntry.second["text"]) = "Instructions";
	((UIMetrics*) gameLogic->instructionsTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->instructionsTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->instructionsTitleEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->instructionsTitleEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->instructionsTitleEntry.second["metrics"]);

	// instructions label
	*((float*) gameLogic->instructionsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->instructionsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((float*) gameLogic->instructionsEntry.second["wrap"]) = 2.0f * (gameSystem->getFloat("helpTextScreenPortion")) - (gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX);
	*((std::string*) gameLogic->instructionsEntry.second["text"]) = gameSystem->getString("textInstructions");
	((UIMetrics*) gameLogic->instructionsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->instructionsEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->instructionsEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->instructionsEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->instructionsEntry.second["metrics"]);

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
	((UIMetrics*) gameLogic->aboutButtonEntry.second["metrics"])->size = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->aboutButtonEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->aboutButtonEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->aboutButtonEntry.second["metrics"]);

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
	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->backButtonEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->backButtonEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->backButtonEntry.second["metrics"]);

	// helpful tip labels
	*((float*) gameLogic->menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip1Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip1Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip1Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"]);

	*((float*) gameLogic->menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip2Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip2Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"]);

	*((float*) gameLogic->menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip3Entry.second["text"]) = "Press ENTER to select";
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip3Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip3Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"]);

	// re-arrange the UI
	drawingMaster->uiLayoutAuthority->rearrange();
	
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
	drawingMaster->drawStack.push_back(gameLogic->splashEntry);

	// about title label
	*((float*) gameLogic->aboutTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->aboutTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->aboutTitleEntry.second["text"]) = "ABOUT";
	((UIMetrics*) gameLogic->aboutTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->aboutTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->aboutTitleEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->aboutTitleEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->aboutTitleEntry.second["metrics"]);

	// version title label
	*((float*) gameLogic->versionTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->versionTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->versionTitleEntry.second["text"]) = "Version Information";
	((UIMetrics*) gameLogic->versionTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->versionTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->versionTitleEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->versionTitleEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->versionTitleEntry.second["metrics"]);

	// version label
	*((float*) gameLogic->versionEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->versionEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((float*) gameLogic->versionEntry.second["wrap"]) = 2.0f * (gameGraphics->resolutionX > 1024 ? 1024.0f / (float) gameGraphics->resolutionX : 1.0f) - (gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX);
	*((std::string*) gameLogic->versionEntry.second["text"]) = gameSystem->versionString;
	((UIMetrics*) gameLogic->versionEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->versionEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->versionEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->versionEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->versionEntry.second["metrics"]);

	// credits title label
	*((float*) gameLogic->creditsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->creditsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->creditsTitleEntry.second["text"]) = "Credits";
	((UIMetrics*) gameLogic->creditsTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->creditsTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->creditsTitleEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->creditsTitleEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->creditsTitleEntry.second["metrics"]);

	// credits label
	*((float*) gameLogic->creditsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->creditsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((float*) gameLogic->creditsEntry.second["wrap"]) = 2.0f * (gameGraphics->resolutionX > 1024 ? 1024.0f / (float) gameGraphics->resolutionX : 1.0f) - (gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX);
	*((std::string*) gameLogic->creditsEntry.second["text"]) = gameSystem->getString("textCredits");
	((UIMetrics*) gameLogic->creditsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->creditsEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->creditsEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->creditsEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->creditsEntry.second["metrics"]);

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
	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->backButtonEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->backButtonEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->backButtonEntry.second["metrics"]);

	// helpful tip labels
	*((float*) gameLogic->menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip1Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip1Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip1Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"]);

	*((float*) gameLogic->menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip2Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip2Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"]);

	*((float*) gameLogic->menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip3Entry.second["text"]) = "Press ENTER to select";
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip3Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip3Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"]);

	// re-arrange the UI
	drawingMaster->uiLayoutAuthority->rearrange();
	
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
	drawingMaster->drawStack.push_back(gameLogic->splashEntry);

	// settings title label
	*((float*) gameLogic->settingsMenuTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->settingsMenuTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->settingsMenuTitleEntry.second["text"]) = "SETTINGS";
	((UIMetrics*) gameLogic->settingsMenuTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->settingsMenuTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->settingsMenuTitleEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->settingsMenuTitleEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->settingsMenuTitleEntry.second["metrics"]);

	// level setting control label
	*((float*) gameLogic->levelSettingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->levelSettingEntry.second["fontColor"]) = (gameLogic->activeMenuSelection == &gameLogic->levelSettingEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
	std::stringstream ss; ss << "Starting Level: " << gameSystem->getString("gameStartingLevel");
	*((std::string*) gameLogic->levelSettingEntry.second["text"]) = ss.str().c_str();
	((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->levelSettingEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->levelSettingEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"]);

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
	((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->musicSettingEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->musicSettingEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"]);

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
	((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->audioEffectsSettingEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->audioEffectsSettingEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"]);

	// fullscreen setting control label
	*((float*) gameLogic->fullscreenSettingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->fullscreenSettingEntry.second["fontColor"]) = (gameLogic->activeMenuSelection == &gameLogic->fullscreenSettingEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
	*((std::string*) gameLogic->fullscreenSettingEntry.second["text"]) = (gameSystem->getBool("displayStartFullscreen") ? "Start Fullscreen: Yes" : "Start Fullscreen: No");
	((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->fullscreenSettingEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->fullscreenSettingEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"]);

	// windowed screen resolution setting control label
	*((float*) gameLogic->windowedScreenResolutionEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->windowedScreenResolutionEntry.second["fontColor"]) = (gameLogic->activeMenuSelection == &gameLogic->windowedScreenResolutionEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
	std::stringstream windowedResolutionSettingText; windowedResolutionSettingText << "Window Resolution: " << gameSystem->getString("displayWindowedResolution");
	*((std::string*) gameLogic->windowedScreenResolutionEntry.second["text"]) = windowedResolutionSettingText.str();
	((UIMetrics*) gameLogic->windowedScreenResolutionEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->windowedScreenResolutionEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->windowedScreenResolutionEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->windowedScreenResolutionEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->windowedScreenResolutionEntry.second["metrics"]);

	// framerate limiting control label
	*((float*) gameLogic->framerateLimitingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->framerateLimitingEntry.second["fontColor"]) = (gameLogic->activeMenuSelection == &gameLogic->framerateLimitingEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
	*((std::string*) gameLogic->framerateLimitingEntry.second["text"]) = (
			(int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_VSYNC ? "Framerate Limiting: Vsync" :
			(int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_30 ? "Framerate Limiting: 30 FPS" :
			(int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_60 ? "Framerate Limiting: 60 FPS" :
			(int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_120 ? "Framerate Limiting: 120 FPS" :
			"Framerate Limiting: Off"
		);
	((UIMetrics*) gameLogic->framerateLimitingEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->framerateLimitingEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->framerateLimitingEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->framerateLimitingEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->framerateLimitingEntry.second["metrics"]);

	// multisampling level control label
	*((float*) gameLogic->multisamplingLevelEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->multisamplingLevelEntry.second["fontColor"]) = (gameLogic->activeMenuSelection == &gameLogic->multisamplingLevelEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
	*((std::string*) gameLogic->multisamplingLevelEntry.second["text"]) = (
			gameSystem->getFloat("displayMultisamplingLevel") == 2.0f ? "Multisampling Level: Low" :
			gameSystem->getFloat("displayMultisamplingLevel") == 4.0f ? "Multisampling Level: High" :
			"Multisampling Level: Off"
		);
	((UIMetrics*) gameLogic->multisamplingLevelEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->multisamplingLevelEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->multisamplingLevelEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->multisamplingLevelEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->multisamplingLevelEntry.second["metrics"]);

	// island terrain detail control label
	*((float*) gameLogic->terrainDetailEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->terrainDetailEntry.second["fontColor"]) = (gameLogic->activeMenuSelection == &gameLogic->terrainDetailEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
	*((std::string*) gameLogic->terrainDetailEntry.second["text"]) = (
			gameSystem->getFloat("islandTerrainDetail") == 2.0f ? "Terrain Detail: Medium" :
			gameSystem->getFloat("islandTerrainDetail") == 3.0f ? "Terrain Detail: High" :
			"Terrain Detail: Low"
		);
	((UIMetrics*) gameLogic->terrainDetailEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->terrainDetailEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->terrainDetailEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->terrainDetailEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->terrainDetailEntry.second["metrics"]);

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
	((UIMetrics*) gameLogic->resetHighScoresEntry.second["metrics"])->size = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->resetHighScoresEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->resetHighScoresEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->resetHighScoresEntry.second["metrics"]);

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
	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->backButtonEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->backButtonEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->backButtonEntry.second["metrics"]);

	// helpful tip labels
	*((float*) gameLogic->menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip1Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip1Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip1Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"]);

	*((float*) gameLogic->menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip2Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip2Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"]);

	*((float*) gameLogic->menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip3Entry.second["text"]) = "Press ENTER to select";
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip3Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip3Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"]);

	// re-arrange the UI
	drawingMaster->uiLayoutAuthority->rearrange();
	
	// set up button listeners with new positions
	gameLogic->levelButtonZoneListener->ll = ((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"])->position - ((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"])->size / 2.0f;
	gameLogic->levelButtonZoneListener->ur = ((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->levelSettingEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->levelButtonZoneListener);
	gameLogic->levelButtonClickListener->ll = gameLogic->levelButtonZoneListener->ll;
	gameLogic->levelButtonClickListener->ur = gameLogic->levelButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->levelButtonClickListener);

	gameLogic->musicButtonZoneListener->ll = ((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"])->position - ((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"])->size / 2.0f;
	gameLogic->musicButtonZoneListener->ur = ((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->musicSettingEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->musicButtonZoneListener);
	gameLogic->musicButtonClickListener->ll = gameLogic->musicButtonZoneListener->ll;
	gameLogic->musicButtonClickListener->ur = gameLogic->musicButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->musicButtonClickListener);

	gameLogic->audioEffectsButtonZoneListener->ll = ((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"])->position - ((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"])->size / 2.0f;
	gameLogic->audioEffectsButtonZoneListener->ur = ((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->audioEffectsSettingEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->audioEffectsButtonZoneListener);
	gameLogic->audioEffectsButtonClickListener->ll = gameLogic->audioEffectsButtonZoneListener->ll;
	gameLogic->audioEffectsButtonClickListener->ur = gameLogic->audioEffectsButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->audioEffectsButtonClickListener);

	gameLogic->fullscreenButtonZoneListener->ll = ((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"])->position - ((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"])->size / 2.0f;
	gameLogic->fullscreenButtonZoneListener->ur = ((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->fullscreenSettingEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->fullscreenButtonZoneListener);
	gameLogic->fullscreenButtonClickListener->ll = gameLogic->fullscreenButtonZoneListener->ll;
	gameLogic->fullscreenButtonClickListener->ur = gameLogic->fullscreenButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->fullscreenButtonClickListener);

	gameLogic->windowedScreenResolutionButtonZoneListener->ll = ((UIMetrics*) gameLogic->windowedScreenResolutionEntry.second["metrics"])->position - ((UIMetrics*) gameLogic->windowedScreenResolutionEntry.second["metrics"])->size / 2.0f;
	gameLogic->windowedScreenResolutionButtonZoneListener->ur = ((UIMetrics*) gameLogic->windowedScreenResolutionEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->windowedScreenResolutionEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->windowedScreenResolutionButtonZoneListener);
	gameLogic->windowedScreenResolutionButtonClickListener->ll = gameLogic->windowedScreenResolutionButtonZoneListener->ll;
	gameLogic->windowedScreenResolutionButtonClickListener->ur = gameLogic->windowedScreenResolutionButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->windowedScreenResolutionButtonClickListener);

	gameLogic->framerateLimitingButtonZoneListener->ll = ((UIMetrics*) gameLogic->framerateLimitingEntry.second["metrics"])->position - ((UIMetrics*) gameLogic->framerateLimitingEntry.second["metrics"])->size / 2.0f;
	gameLogic->framerateLimitingButtonZoneListener->ur = ((UIMetrics*) gameLogic->framerateLimitingEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->framerateLimitingEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->framerateLimitingButtonZoneListener);
	gameLogic->framerateLimitingButtonClickListener->ll = gameLogic->framerateLimitingButtonZoneListener->ll;
	gameLogic->framerateLimitingButtonClickListener->ur = gameLogic->framerateLimitingButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->framerateLimitingButtonClickListener);

	gameLogic->multisamplingButtonZoneListener->ll = ((UIMetrics*) gameLogic->multisamplingLevelEntry.second["metrics"])->position - ((UIMetrics*) gameLogic->multisamplingLevelEntry.second["metrics"])->size / 2.0f;
	gameLogic->multisamplingButtonZoneListener->ur = ((UIMetrics*) gameLogic->multisamplingLevelEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->multisamplingLevelEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->multisamplingButtonZoneListener);
	gameLogic->multisamplingButtonClickListener->ll = gameLogic->multisamplingButtonZoneListener->ll;
	gameLogic->multisamplingButtonClickListener->ur = gameLogic->multisamplingButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->multisamplingButtonClickListener);

	gameLogic->terrainDetailButtonZoneListener->ll = ((UIMetrics*) gameLogic->terrainDetailEntry.second["metrics"])->position - ((UIMetrics*) gameLogic->terrainDetailEntry.second["metrics"])->size / 2.0f;
	gameLogic->terrainDetailButtonZoneListener->ur = ((UIMetrics*) gameLogic->terrainDetailEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->terrainDetailEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->terrainDetailButtonZoneListener);
	gameLogic->terrainDetailButtonClickListener->ll = gameLogic->terrainDetailButtonZoneListener->ll;
	gameLogic->terrainDetailButtonClickListener->ur = gameLogic->terrainDetailButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->terrainDetailButtonClickListener);

	gameLogic->backButtonZoneListener->ll = ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->position - ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size / 2.0f;
	gameLogic->backButtonZoneListener->ur = ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->position + ((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->backButtonZoneListener);
	gameLogic->backButtonClickListener->ll = gameLogic->backButtonZoneListener->ll;
	gameLogic->backButtonClickListener->ur = gameLogic->backButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->backButtonClickListener);

	gameLogic->resetHighScoresButtonZoneListener->ll = ((UIMetrics*) gameLogic->resetHighScoresEntry.second["metrics"])->position - ((UIMetrics*) gameLogic->resetHighScoresEntry.second["metrics"])->size / 2.0f;
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
	drawingMaster->drawStack.push_back(gameLogic->splashEntry);

	// high scores title label
	*((float*) gameLogic->highScoresTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->highScoresTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->highScoresTitleEntry.second["text"]) = "HIGH SCORES";
	((UIMetrics*) gameLogic->highScoresTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->highScoresTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->highScoresTitleEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->highScoresTitleEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->highScoresTitleEntry.second["metrics"]);

	// high scores label
	*((float*) gameLogic->highScoresLabelEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->highScoresLabelEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	std::stringstream stringStream;
	for(size_t i = 0; i < gameSystem->highScores.size(); ++i)
		stringStream << (i > 0 ? "\n" : "") << gameSystem->highScores[i].second << "\t" << gameSystem->highScores[i].first;
	if(stringStream.str().length() == 0) stringStream.str("No high scores have been recorded yet.");
	*((std::string*) gameLogic->highScoresLabelEntry.second["text"]) = stringStream.str();
	((UIMetrics*) gameLogic->highScoresLabelEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->highScoresLabelEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->highScoresLabelEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->highScoresLabelEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->highScoresLabelEntry.second["metrics"]);

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
	((UIMetrics*) gameLogic->backButtonEntry.second["metrics"])->size = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->backButtonEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->backButtonEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->backButtonEntry.second["metrics"]);

	// helpful tip labels
	*((float*) gameLogic->menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip1Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip1Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip1Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip1Entry.second["metrics"]);

	*((float*) gameLogic->menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip2Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip2Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"]);

	*((float*) gameLogic->menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip3Entry.second["text"]) = "Press ENTER to select";
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip3Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip3Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"]);

	// re-arrange the UI
	drawingMaster->uiLayoutAuthority->rearrange();
	
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
	drawingMaster->drawStack.push_back(gameLogic->splashEntry);

	// loading label
	*((float*) gameLogic->loadingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->loadingEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->loadingEntry.second["text"]) = "Loading...";
	((UIMetrics*) gameLogic->loadingEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->loadingEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->loadingEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->loadingEntry.second["metrics"]);

	// re-arrange the UI
	drawingMaster->uiLayoutAuthority->rearrange();
}

void Schemes::playingScheme() {
	// input
	inputHandler->mouse->addListener(gameLogic->mouseMotionListener);
	inputHandler->mouse->addListener(gameLogic->primaryFireClickListener);
	inputHandler->mouse->addListener(gameLogic->secondaryFireClickListener);
	inputHandler->mouse->addListener(gameLogic->binocularsClickListener);
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->playingKeyListener);
	inputHandler->keyboard->addListener(gameLogic->cameraAheadKeyListener);
	inputHandler->keyboard->addListener(gameLogic->turretUpKeyListener);
	inputHandler->keyboard->addListener(gameLogic->turretDownKeyListener);
	inputHandler->keyboard->addListener(gameLogic->turretLeftKeyListener);
	inputHandler->keyboard->addListener(gameLogic->turretRightKeyListener);

	// sky
	drawingMaster->drawStack.push_back(gameLogic->skyEntry);

	// water
	drawingMaster->drawStack.push_back(gameLogic->waterEntry);

	// terrain
	drawingMaster->drawStack.push_back(gameLogic->terrainEntry);

	// ships
	drawingMaster->drawStack.push_back(gameLogic->shipEntry);

	// missile trails
	drawingMaster->drawStack.push_back(gameLogic->missileTrailEntry);

	// missiles
	drawingMaster->drawStack.push_back(gameLogic->missileEntry);

	// shells
	drawingMaster->drawStack.push_back(gameLogic->shellEntry);

	// explosions
	drawingMaster->drawStack.push_back(gameLogic->explosionEntry);

	// fortress
	drawingMaster->drawStack.push_back(gameLogic->fortressEntry);

	// missile indicators
	*((Vector4*) gameLogic->missileIndicators.second["color"]) = gameSystem->getColor("hudMissileIndicatorColor");
	*((Vector2*) gameLogic->missileIndicators.second["size"]) = Vector2(
			gameSystem->getFloat("hudMissileIndicatorSize") * (gameState->binoculars ? gameSystem->getFloat("hudMissileIndicatorBinocularsFactor") : 1.0f) / (float) gameGraphics->resolutionX * 2.0f,
			gameSystem->getFloat("hudMissileIndicatorSize") * (gameState->binoculars ? gameSystem->getFloat("hudMissileIndicatorBinocularsFactor") : 1.0f) / (float) gameGraphics->resolutionY * 2.0f
		);
	drawingMaster->drawStack.push_back(gameLogic->missileIndicators);

	// score
	*((float*) gameLogic->scoreLabel.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->scoreLabel.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	char scoreString[8]; scoreString[0] = '\0'; if(gameState->score <= 9999999) sprintf(scoreString, "%u", gameState->score);
	*((std::string*) gameLogic->scoreLabel.second["text"]) = scoreString;
	((UIMetrics*) gameLogic->scoreLabel.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->scoreLabel.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->scoreLabel.second);
	drawingMaster->drawStack.push_back(gameLogic->scoreLabel);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->scoreLabel.second["metrics"]);

	// tip
	*((float*) gameLogic->playingTipEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->playingTipEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->playingTipEntry.second["text"]) = "Press ESC to pause and show options";
	((UIMetrics*) gameLogic->playingTipEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->playingTipEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->playingTipEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->playingTipEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->playingTipEntry.second["metrics"]);

	// control spot
	*((Vector2*) gameLogic->controlSpotEntry.second["size"]) = Vector2(
		gameSystem->getFloat("hudControlSpotSize") * 2.0f / (float) gameGraphics->resolutionX,
		gameSystem->getFloat("hudControlSpotSize") * 2.0f / (float) gameGraphics->resolutionY
	);
	*((Vector2*) gameLogic->controlSpotEntry.second["position"]) = Vector2(0.0f, 0.0f);
	*((float*) gameLogic->controlSpotEntry.second["border"]) = gameSystem->getFloat("hudControlSpotBorder");
	*((float*) gameLogic->controlSpotEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->controlSpotEntry.second["insideColor"]) = Vector4(
			gameSystem->getColor("hudControlSpotColor").x,
			gameSystem->getColor("hudControlSpotColor").y,
			gameSystem->getColor("hudControlSpotColor").z,
			0.0f
		);
	*((Vector4*) gameLogic->controlSpotEntry.second["borderColor"]) = gameSystem->getColor("hudControlSpotColor");
	*((Vector4*) gameLogic->controlSpotEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudControlSpotColor").x,
			gameSystem->getColor("hudControlSpotColor").y,
			gameSystem->getColor("hudControlSpotColor").z,
			0.0f
		);
	drawingMaster->drawStack.push_back(gameLogic->controlSpotEntry);

	// gauge panel
	*((float*) gameLogic->gaugePanelEntry.second["padding"]) = gameSystem->getFloat("hudGaugePadding");
	*((float*) gameLogic->gaugePanelEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->gaugePanelEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->gaugePanelEntry.second["insideColor"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) gameLogic->gaugePanelEntry.second["borderColor"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) gameLogic->gaugePanelEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudGaugeBackgroundColor").x,
			gameSystem->getColor("hudGaugeBackgroundColor").y,
			gameSystem->getColor("hudGaugeBackgroundColor").z,
			0.0f
		);
	*((size_t*) gameLogic->gaugePanelEntry.second["elements"]) = 3;
	(*((std::vector<std::string>*) gameLogic->gaugePanelEntry.second["textureNames"]))[0] = "gauge/heart";
	(*((std::vector<std::string>*) gameLogic->gaugePanelEntry.second["textureNames"]))[1] = "gauge/shell";
	(*((std::vector<std::string>*) gameLogic->gaugePanelEntry.second["textureNames"]))[2] = "gauge/bolt";
	((std::vector<Vector2>*) gameLogic->gaugePanelEntry.second["textureSizes"])->at(0) = Vector2(
			gameSystem->getFloat("gaugeImagesHeight") * gameGraphics->getTexture("gauge/heart")->width / gameGraphics->getTexture("gauge/heart")->height / (float) gameGraphics->resolutionX * 2.0f,
			gameSystem->getFloat("gaugeImagesHeight") / (float) gameGraphics->resolutionY * 2.0f
		);
	((std::vector<Vector2>*) gameLogic->gaugePanelEntry.second["textureSizes"])->at(1) = Vector2(
			gameSystem->getFloat("gaugeImagesHeight") * gameGraphics->getTexture("gauge/shell")->width / gameGraphics->getTexture("gauge/shell")->height / (float) gameGraphics->resolutionX * 2.0f,
			gameSystem->getFloat("gaugeImagesHeight") / (float) gameGraphics->resolutionY * 2.0f
		);
	((std::vector<Vector2>*) gameLogic->gaugePanelEntry.second["textureSizes"])->at(2) = Vector2(
			gameSystem->getFloat("gaugeImagesHeight") * gameGraphics->getTexture("gauge/bolt")->width / gameGraphics->getTexture("gauge/bolt")->height / (float) gameGraphics->resolutionX * 2.0f,
			gameSystem->getFloat("gaugeImagesHeight") / (float) gameGraphics->resolutionY * 2.0f
		);
	*((Vector2*) gameLogic->gaugePanelEntry.second["progressBarSize"]) = Vector2(gameSystem->getFloat("hudGaugeWidth"), gameSystem->getFloat("hudGaugeHeight"));
	(*((std::vector<float>*) gameLogic->gaugePanelEntry.second["progressions"]))[0] = gameState->fortress.health;
	(*((std::vector<float>*) gameLogic->gaugePanelEntry.second["progressions"]))[1] = gameState->fortress.ammunition;
	(*((std::vector<float>*) gameLogic->gaugePanelEntry.second["progressions"]))[2] = (gameState->fortress.emp > 1.0f ? 2.0f - gameState->fortress.emp : gameState->fortress.emp);
	*((Vector4*) gameLogic->gaugePanelEntry.second["backgroundColorTop"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) gameLogic->gaugePanelEntry.second["backgroundColorBottom"]) = Vector4(
			gameSystem->getColor("hudGaugeBackgroundColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor("hudGaugeBackgroundColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor("hudGaugeBackgroundColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor("hudGaugeBackgroundColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	(*((std::vector<Vector4>*) gameLogic->gaugePanelEntry.second["progressBarColorsTop"]))[0] = gameSystem->getColor("hudGaugeHealthBarColor");
	(*((std::vector<Vector4>*) gameLogic->gaugePanelEntry.second["progressBarColorsTop"]))[1] = gameSystem->getColor("hudGaugeAmmoBarColor");
	(*((std::vector<Vector4>*) gameLogic->gaugePanelEntry.second["progressBarColorsTop"]))[2] = gameSystem->getColor(gameState->fortress.emp > 1.0f ? "hudGaugeEMPChargingBarColor" : "hudGaugeEMPChargedBarColor");
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
			gameSystem->getColor(gameState->fortress.emp > 1.0f ? "hudGaugeEMPChargingBarColor" : "hudGaugeEMPChargedBarColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor(gameState->fortress.emp > 1.0f ? "hudGaugeEMPChargingBarColor" : "hudGaugeEMPChargedBarColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor(gameState->fortress.emp > 1.0f ? "hudGaugeEMPChargingBarColor" : "hudGaugeEMPChargedBarColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor(gameState->fortress.emp > 1.0f ? "hudGaugeEMPChargingBarColor" : "hudGaugeEMPChargedBarColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	((UIMetrics*) gameLogic->gaugePanelEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_LEFT;
	((UIMetrics*) gameLogic->gaugePanelEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->gaugePanelEntry.second["metrics"])->size = ((DrawGaugePanel*) drawingMaster->drawers["gaugePanel"])->getSize(gameLogic->gaugePanelEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->gaugePanelEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->gaugePanelEntry.second["metrics"]);

	// radar
	*((float*) gameLogic->radarEntry.second["padding"]) = gameSystem->getFloat("hudGaugePadding");
	*((float*) gameLogic->radarEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
	*((float*) gameLogic->radarEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
	*((Vector4*) gameLogic->radarEntry.second["insideColor"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) gameLogic->radarEntry.second["borderColor"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) gameLogic->radarEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudGaugeBackgroundColor").x,
			gameSystem->getColor("hudGaugeBackgroundColor").y,
			gameSystem->getColor("hudGaugeBackgroundColor").z,
			0.0f
		);
	((UIMetrics*) gameLogic->radarEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_RIGHT;
	((UIMetrics*) gameLogic->radarEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->radarEntry.second["metrics"])->size =
			((DrawRadar*) drawingMaster->drawers["radar"])->getSize(gameLogic->radarEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->radarEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->radarEntry.second["metrics"]);

	// cursor
	if(gameLogic->mouseActive) {
		*((Vector2*) gameLogic->cursorEntry.second["size"]) = Vector2(
			gameSystem->getFloat("hudCursorSize") * 2.0f / (float) gameGraphics->resolutionX,
			gameSystem->getFloat("hudCursorSize") * 2.0f / (float) gameGraphics->resolutionY
		);
		gameLogic->cursorEntry.second["position"] = (gameLogic->mouseActive ? (void*) &inputHandler->mouse->position : &gameLogic->keyboardCursorPosition);
		*((float*) gameLogic->cursorEntry.second["border"]) = 0.0f;
		*((float*) gameLogic->cursorEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
		*((Vector4*) gameLogic->cursorEntry.second["insideColor"]) = gameSystem->getColor("hudCursorColor");
		*((Vector4*) gameLogic->cursorEntry.second["borderColor"]) = Vector4(
				gameSystem->getColor("hudCursorColor").x,
				gameSystem->getColor("hudCursorColor").y,
				gameSystem->getColor("hudCursorColor").z,
				0.0f
			);
		*((Vector4*) gameLogic->cursorEntry.second["outsideColor"]) = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		drawingMaster->drawStack.push_back(gameLogic->cursorEntry);
	}

	// strike effect
	drawingMaster->drawStack.push_back(gameLogic->strikeEffectEntry);

	// re-arrange the UI
	drawingMaster->uiLayoutAuthority->rearrange();
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
	drawingMaster->drawStack.push_back(gameLogic->skyEntry);

	// water
	drawingMaster->drawStack.push_back(gameLogic->waterEntry);

	// terrain
	drawingMaster->drawStack.push_back(gameLogic->terrainEntry);

	// ships
	drawingMaster->drawStack.push_back(gameLogic->shipEntry);

	// fortress
	drawingMaster->drawStack.push_back(gameLogic->fortressEntry);

	// missiles
	drawingMaster->drawStack.push_back(gameLogic->missileEntry);

	// tip
	*((float*) gameLogic->introHintEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->introHintEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->introHintEntry.second["text"]) = "LEFT CLICK or press SPACE to skip introduction";
	((UIMetrics*) gameLogic->introHintEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->introHintEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->introHintEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->introHintEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->introHintEntry.second["metrics"]);

	// re-arrange the UI
	drawingMaster->uiLayoutAuthority->rearrange();
}

void Schemes::pausedScheme() {
	// input
	inputHandler->mouse->addListener(gameLogic->mouseMotionListener);
	inputHandler->keyboard->addListener(gameLogic->quitKeyListener);
	inputHandler->keyboard->addListener(gameLogic->fullScreenKeyListener);
	inputHandler->keyboard->addListener(gameLogic->pausedMenuKeyListener);

	// sky
	drawingMaster->drawStack.push_back(gameLogic->skyEntry);

	// water
	drawingMaster->drawStack.push_back(gameLogic->waterEntry);

	// terrain
	drawingMaster->drawStack.push_back(gameLogic->terrainEntry);

	// ships
	drawingMaster->drawStack.push_back(gameLogic->shipEntry);

	// fortress
	drawingMaster->drawStack.push_back(gameLogic->fortressEntry);

	// missiles
	drawingMaster->drawStack.push_back(gameLogic->missileEntry);

	// gray out
	*((Vector4*) gameLogic->grayOutEntry.second["color"]) = gameSystem->getColor("hudGrayOutColor");
	drawingMaster->drawStack.push_back(gameLogic->grayOutEntry);

	// paused title label
	*((float*) gameLogic->pausedEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->pausedEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->pausedEntry.second["text"]) = "PAUSED";
	((UIMetrics*) gameLogic->pausedEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->pausedEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->pausedEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->pausedEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->pausedEntry.second["metrics"]);

	// controls title label
	*((float*) gameLogic->controlsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->controlsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->controlsTitleEntry.second["text"]) = "Controls";
	((UIMetrics*) gameLogic->controlsTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->controlsTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->controlsTitleEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->controlsTitleEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->controlsTitleEntry.second["metrics"]);

	// controls label
	*((float*) gameLogic->controlsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->controlsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->controlsEntry.second["text"]) = gameSystem->getString("textControls");
	((UIMetrics*) gameLogic->controlsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->controlsEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->controlsEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->controlsEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->controlsEntry.second["metrics"]);

	// spacer
	gameLogic->pausedMenuSpacerMetrics->bearing1 = UIMetrics::BEARING_TOP;
	drawingMaster->uiLayoutAuthority->metrics.push_back(gameLogic->pausedMenuSpacerMetrics);

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
	((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->size = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->resumeButtonEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->resumeButtonEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"]);

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
	((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->size = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->endGameButtonEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->endGameButtonEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"]);

	// helpful tip labels
	*((float*) gameLogic->pausedMenuTipEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->pausedMenuTipEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->pausedMenuTipEntry.second["text"]) = "Press ESC to return to the game";
	((UIMetrics*) gameLogic->pausedMenuTipEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->pausedMenuTipEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->pausedMenuTipEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->pausedMenuTipEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->pausedMenuTipEntry.second["metrics"]);

	*((float*) gameLogic->menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip2Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip2Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip2Entry.second["metrics"]);

	*((float*) gameLogic->menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->menuTip3Entry.second["text"]) = "Press ENTER to select";
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->menuTip3Entry.second);
	drawingMaster->drawStack.push_back(gameLogic->menuTip3Entry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->menuTip3Entry.second["metrics"]);

	// make all buttons the same width
	float maxWidth = 0.0f;
	if(((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->size.x > maxWidth)
		maxWidth = ((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->size.x;
	if(((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->size.x > maxWidth)
		maxWidth = ((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->size.x;
	((UIMetrics*) gameLogic->resumeButtonEntry.second["metrics"])->size.x = maxWidth;
	((UIMetrics*) gameLogic->endGameButtonEntry.second["metrics"])->size.x = maxWidth;

	// re-arrange the UI
	drawingMaster->uiLayoutAuthority->rearrange();
	
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
	drawingMaster->drawStack.push_back(gameLogic->splashEntry);

	// game over title label
	*((float*) gameLogic->gameOverTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
	*((Vector4*) gameLogic->gameOverTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->gameOverTitleEntry.second["text"]) = "GAME OVER";
	((UIMetrics*) gameLogic->gameOverTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->gameOverTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->gameOverTitleEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->gameOverTitleEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->gameOverTitleEntry.second["metrics"]);

	// score title label
	*((float*) gameLogic->yourScoreTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->yourScoreTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->yourScoreTitleEntry.second["text"]) = "Score";
	((UIMetrics*) gameLogic->yourScoreTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->yourScoreTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->yourScoreTitleEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->yourScoreTitleEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->yourScoreTitleEntry.second["metrics"]);

	// score label
	*((float*) gameLogic->yourScoreEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->yourScoreEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	char scoreString[8]; scoreString[0] = '\0'; if(gameState->score <= 9999999) sprintf(scoreString, "%u", gameState->score);
	*((std::string*) gameLogic->yourScoreEntry.second["text"]) = scoreString;
	((UIMetrics*) gameLogic->yourScoreEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->yourScoreEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->yourScoreEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->yourScoreEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->yourScoreEntry.second["metrics"]);

	// high scores section label
	*((float*) gameLogic->highScoresSectionEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
	*((Vector4*) gameLogic->highScoresSectionEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameLogic->highScoresSectionEntry.second["text"]) = "High Scores";
	((UIMetrics*) gameLogic->highScoresSectionEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->highScoresSectionEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->highScoresSectionEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->highScoresSectionEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->highScoresSectionEntry.second["metrics"]);

	// high scores label
	*((float*) gameLogic->highScoresLabelEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
	*((Vector4*) gameLogic->highScoresLabelEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	std::stringstream stringStream;
	for(size_t i = 0; i < gameSystem->highScores.size(); ++i)
		stringStream << (i > 0 ? "\n" : "") << gameSystem->highScores[i].second << "\t" << gameSystem->highScores[i].first;
	if(stringStream.str().length() == 0) stringStream.str("No high scores have been recorded yet.");
	*((std::string*) gameLogic->highScoresLabelEntry.second["text"]) = stringStream.str();
	((UIMetrics*) gameLogic->highScoresLabelEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
	((UIMetrics*) gameLogic->highScoresLabelEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->highScoresLabelEntry.second);
	drawingMaster->drawStack.push_back(gameLogic->highScoresLabelEntry);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->highScoresLabelEntry.second["metrics"]);

	if(gameState->score > 0 && (gameSystem->highScores.size() == 0 || gameState->score > gameSystem->highScores[0].first)) {
		// high scores label
		*((float*) gameLogic->newHighScoreLabel.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
		*((Vector4*) gameLogic->newHighScoreLabel.second["fontColor"]) = gameSystem->getColor("fontColorLight");
		*((std::string*) gameLogic->newHighScoreLabel.second["text"]) = "Enter New High Score";
		((UIMetrics*) gameLogic->newHighScoreLabel.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
		((UIMetrics*) gameLogic->newHighScoreLabel.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->newHighScoreLabel.second);
		drawingMaster->drawStack.push_back(gameLogic->newHighScoreLabel);
		drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->newHighScoreLabel.second["metrics"]);

		// name prompt label
		*((float*) gameLogic->newHighScoreNameLabel.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
		*((Vector4*) gameLogic->newHighScoreNameLabel.second["fontColor"]) = gameSystem->getColor("fontColorLight");
		*((std::string*) gameLogic->newHighScoreNameLabel.second["text"]) = "Player Name:";
		((UIMetrics*) gameLogic->newHighScoreNameLabel.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(gameLogic->newHighScoreNameLabel.second);
//FIXME FIIIIIIIIXXXXX MEEEEEEEE text doesn't line up if one but not the other goes below the baseline!

		// name prompt field
		*((float*) gameLogic->newHighScoreNameField.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
		*((Vector4*) gameLogic->newHighScoreNameField.second["fontColor"]) = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		*((Vector4*) gameLogic->newHighScoreNameField.second["boxColor"]) = gameSystem->getColor("hudFieldColor");
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) = "";
		for(size_t i = 0; i < gameSystem->getFloat("hudFieldWidth") + 1; ++i) *((std::string*) gameLogic->newHighScoreNameField.second["text"]) += "#";
		*((Vector2*) gameLogic->newHighScoreNameField.second["size"]) = ((DrawField*) drawingMaster->drawers["field"])->getSize(gameLogic->newHighScoreNameField.second);
		((UIMetrics*) gameLogic->newHighScoreNameField.second["metrics"])->size = *((Vector2*) gameLogic->newHighScoreNameField.second["size"]);
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) = "\\";
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) += gameSystem->getString("fontColorLight");
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) += gameLogic->playerName;
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) += "\\";
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) += gameSystem->getString("fontColorDark");
		*((std::string*) gameLogic->newHighScoreNameField.second["text"]) += "|";

		// name prompt container
		*((float*) gameLogic->newHighScoreContainer.second["padding"]) = gameSystem->getFloat("hudContainerPadding");
		*((float*) gameLogic->newHighScoreContainer.second["border"]) = 0.0f;
		*((float*) gameLogic->newHighScoreContainer.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
		*((Vector4*) gameLogic->newHighScoreContainer.second["insideColor"]) = gameSystem->getColor("hudContainerInsideColor");
		*((Vector4*) gameLogic->newHighScoreContainer.second["borderColor"]) = gameSystem->getColor("hudContainerOutsideColor");
		*((Vector4*) gameLogic->newHighScoreContainer.second["outsideColor"]) = gameSystem->getColor("hudContainerOutsideColor");
		((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;
		((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->size = Vector2(
				((UIMetrics*) gameLogic->newHighScoreNameLabel.second["metrics"])->size.x +
						((UIMetrics*) gameLogic->newHighScoreNameField.second["metrics"])->size.x +
						gameSystem->getFloat("hudContainerPadding") / gameGraphics->resolutionX * 4.0f +
						gameSystem->getFloat("hudElementMargin") / gameGraphics->resolutionX * 2.0f,
				(((UIMetrics*) gameLogic->newHighScoreNameLabel.second["metrics"])->size.y >
						((UIMetrics*) gameLogic->newHighScoreNameField.second["metrics"])->size.y ?
						((UIMetrics*) gameLogic->newHighScoreNameLabel.second["metrics"])->size.y :
						((UIMetrics*) gameLogic->newHighScoreNameField.second["metrics"])->size.y) +
						gameSystem->getFloat("hudContainerPadding") / gameGraphics->resolutionY * 4.0f
			);
		drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"]);
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
	((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"])->size = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameLogic->gameOverContinueButton.second);
	drawingMaster->drawStack.push_back(gameLogic->gameOverContinueButton);
	drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"]);

	// re-arrange the UI
	drawingMaster->uiLayoutAuthority->rearrange();

	if(gameState->score > 0 && (gameSystem->highScores.size() == 0 || gameState->score > gameSystem->highScores[0].first)) {
		// assemble prompt container contents
		((UIMetrics*) gameLogic->newHighScoreNameLabel.second["metrics"])->position = Vector2(
				((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->position.x -
						((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->size.x / 2.0f +
						gameSystem->getFloat("hudContainerPadding") / gameGraphics->resolutionX * 2.0f +
						((UIMetrics*) gameLogic->newHighScoreNameLabel.second["metrics"])->size.x / 2.0f,
				((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->position.y
			);
		((UIMetrics*) gameLogic->newHighScoreNameField.second["metrics"])->position = Vector2(
				((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->position.x +
						((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->size.x / 2.0f -
						gameSystem->getFloat("hudContainerPadding") / gameGraphics->resolutionX * 2.0f -
						((UIMetrics*) gameLogic->newHighScoreNameField.second["metrics"])->size.x / 2.0f,
				((UIMetrics*) gameLogic->newHighScoreContainer.second["metrics"])->position.y
			);
		drawingMaster->drawStack.push_back(gameLogic->newHighScoreContainer);
		drawingMaster->drawStack.push_back(gameLogic->newHighScoreNameLabel);
		drawingMaster->drawStack.push_back(gameLogic->newHighScoreNameField);
	}

	// set up button listener with new position
	gameLogic->gameOverContinueButtonZoneListener->ll = ((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"])->position -	((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"])->size / 2.0f;
	gameLogic->gameOverContinueButtonZoneListener->ur = ((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"])->position + ((UIMetrics*) gameLogic->gameOverContinueButton.second["metrics"])->size / 2.0f;
	inputHandler->mouse->addListener(gameLogic->gameOverContinueButtonZoneListener);
	gameLogic->gameOverContinueButtonClickListener->ll = gameLogic->gameOverContinueButtonZoneListener->ll;
	gameLogic->gameOverContinueButtonClickListener->ur = gameLogic->gameOverContinueButtonZoneListener->ur;
	inputHandler->mouse->addListener(gameLogic->gameOverContinueButtonClickListener);
}
