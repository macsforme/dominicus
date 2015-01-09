// GameLogic.cpp
// Crucible Island

#include "logic/GameLogic.h"

#include <cmath>
#include <cstdio>
#include <map>
#include <SDL.h>
#include <sstream>
#include <utility>

#include "audio/GameAudio.h"
#include "core/GameSystem.h"
#include "graphics/DrawingMaster.h"
#include "graphics/GameGraphics.h"
#include "graphics/2dgraphics/DrawButton.h"
#include "graphics/2dgraphics/DrawCircle.h"
#include "graphics/2dgraphics/DrawContainer.h"
#include "graphics/2dgraphics/DrawField.h"
#include "graphics/2dgraphics/DrawGaugePanel.h"
#include "graphics/2dgraphics/DrawGrayOut.h"
#include "graphics/2dgraphics/DrawLabel.h"
#include "graphics/2dgraphics/DrawMissileIndicators.h"
#include "graphics/2dgraphics/DrawRadar.h"
#include "graphics/2dgraphics/DrawSplash.h"
#include "graphics/2dgraphics/DrawStrikeEffect.h"
#include "graphics/2dgraphics/DrawTexture.h"
#include "graphics/3dgraphics/ExplosionRenderer.h"
#include "graphics/3dgraphics/TerrainRenderer.h"
#include "input/InputHandler.h"
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "state/GameState.h"

extern DrawingMaster* drawingMaster;
extern GameAudio* gameAudio;
extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;
extern InputHandler* inputHandler;
extern Platform* platform;
extern bool keepProgramAlive;
extern std::map<MainLoopMember*,unsigned int> mainLoopModules;

void GameLogic::syncButtonWidths(std::vector<DrawStackEntry*> buttons) {
	float maxWidth = 0.0f;

	for(size_t i = 0; i < buttons.size(); ++i)
		if(((Vector2*) buttons[i]->second["size"])->x > maxWidth)
			maxWidth = ((Vector2*) buttons[i]->second["size"])->x;

	for(size_t i = 0; i < buttons.size(); ++i) {
		((Vector2*) buttons[i]->second["size"])->x = maxWidth;
		((UIMetrics*) buttons[i]->second["metrics"])->size.x = maxWidth;
	}
}

void GameLogic::reScheme() {
	// clear the stacks
	drawingMaster->drawStack.clear();
	drawingMaster->uiLayoutAuthority->metrics.clear();
	inputHandler->keyboard.clearListeners();
	inputHandler->mouse.clearListeners();

	// big scheme logic switch
	switch (currentScheme) {
	case SCHEME_MAINMENU:
		{
			// input
			inputHandler->mouse.addListener(mouseMotionListener);
			inputHandler->keyboard.addListener(quitKeyListener);
			inputHandler->keyboard.addListener(fullScreenKeyListener);
			inputHandler->keyboard.addListener(mainMenuKeyListener);

			// splash background
			drawingMaster->drawStack.push_back(splashEntry);

			// title label
			*((float*) mainMenuTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSuper");
			((UIMetrics*) mainMenuTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(mainMenuTitleEntry.second);
			drawingMaster->drawStack.push_back(mainMenuTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) mainMenuTitleEntry.second["metrics"]);

			// game logo
			*((Vector2*) logoEntry.second["size"]) = Vector2(
					gameSystem->getFloat("logoHeight") * gameGraphics->getTexture("branding/logo")->width / gameGraphics->getTexture("branding/logo")->height / (float) gameGraphics->resolutionX * 2.0f,
					gameSystem->getFloat("logoHeight") / (float) gameGraphics->resolutionY * 2.0f
				);
			((UIMetrics*) logoEntry.second["metrics"])->size = ((DrawTexture*) drawingMaster->drawers["texture"])->getSize(logoEntry.second);
			drawingMaster->drawStack.push_back(logoEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) logoEntry.second["metrics"]);

			// spacer
			drawingMaster->uiLayoutAuthority->metrics.push_back(spacerMetrics);

			// play button
			*((float*) playButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((Vector4*) playButtonEntry.second["insideColor"]) = (
					activeMenuSelection == &playButtonEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			*((float*) playButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
			*((float*) playButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) playButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			Vector2* playButtonSize = (Vector2*) playButtonEntry.second["size"];
			playButtonEntry.second.erase(playButtonEntry.second.find("size"));
			*playButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(playButtonEntry.second);
			playButtonEntry.second["size"] = playButtonSize;
			((UIMetrics*) playButtonEntry.second["metrics"])->size = *playButtonSize;
			drawingMaster->drawStack.push_back(playButtonEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) playButtonEntry.second["metrics"]);

			// help button
			*((float*) helpButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((Vector4*) helpButtonEntry.second["insideColor"]) = (
					activeMenuSelection == &helpButtonEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			*((float*) helpButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
			*((float*) helpButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) helpButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			Vector2* helpButtonSize = (Vector2*) helpButtonEntry.second["size"];
			highScoresButtonEntry.second.erase(helpButtonEntry.second.find("size"));
			*helpButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(helpButtonEntry.second);
			helpButtonEntry.second["size"] = helpButtonSize;
			((UIMetrics*) helpButtonEntry.second["metrics"])->size = *helpButtonSize;
			drawingMaster->drawStack.push_back(helpButtonEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) helpButtonEntry.second["metrics"]);

			// high scores button
			*((float*) highScoresButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((Vector4*) highScoresButtonEntry.second["insideColor"]) = (
					activeMenuSelection == &highScoresButtonEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			*((float*) highScoresButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
			*((float*) highScoresButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) highScoresButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			Vector2* highScoresButtonSize = (Vector2*) highScoresButtonEntry.second["size"];
			highScoresButtonEntry.second.erase(highScoresButtonEntry.second.find("size"));
			*highScoresButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(highScoresButtonEntry.second);
			highScoresButtonEntry.second["size"] = highScoresButtonSize;
			((UIMetrics*) highScoresButtonEntry.second["metrics"])->size = *highScoresButtonSize;
			drawingMaster->drawStack.push_back(highScoresButtonEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) highScoresButtonEntry.second["metrics"]);

			// settings button
			*((float*) settingsButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((Vector4*) settingsButtonEntry.second["insideColor"]) = (
					activeMenuSelection == &settingsButtonEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			*((float*) settingsButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
			*((float*) settingsButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) settingsButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			Vector2* settingsButtonSize = (Vector2*) settingsButtonEntry.second["size"];
			settingsButtonEntry.second.erase(settingsButtonEntry.second.find("size"));
			*settingsButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(settingsButtonEntry.second);
			settingsButtonEntry.second["size"] = settingsButtonSize;
			((UIMetrics*) settingsButtonEntry.second["metrics"])->size = *settingsButtonSize;
			drawingMaster->drawStack.push_back(settingsButtonEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) settingsButtonEntry.second["metrics"]);

			// quit button
			*((float*) quitButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((Vector4*) quitButtonEntry.second["insideColor"]) = (
					activeMenuSelection == &quitButtonEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			*((float*) quitButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
			*((float*) quitButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) quitButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			Vector2* quitButtonSize = (Vector2*) quitButtonEntry.second["size"];
			highScoresButtonEntry.second.erase(quitButtonEntry.second.find("size"));
			*quitButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(quitButtonEntry.second);
			quitButtonEntry.second["size"] = quitButtonSize;
			((UIMetrics*) quitButtonEntry.second["metrics"])->size = *quitButtonSize;
			drawingMaster->drawStack.push_back(quitButtonEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) quitButtonEntry.second["metrics"]);

			// tips
			*((float*) menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((std::string*) menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
			((UIMetrics*) menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip1Entry.second);
			drawingMaster->drawStack.push_back(menuTip1Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip1Entry.second["metrics"]);

			*((float*) menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) menuTip2Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip2Entry.second);
			drawingMaster->drawStack.push_back(menuTip2Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip2Entry.second["metrics"]);

			*((float*) menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) menuTip3Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip3Entry.second);
			drawingMaster->drawStack.push_back(menuTip3Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip3Entry.second["metrics"]);

			// make all buttons the same width
			std::vector<DrawStackEntry*> buttonsToSync;
			buttonsToSync.push_back(&playButtonEntry);
			buttonsToSync.push_back(&settingsButtonEntry);
			buttonsToSync.push_back(&highScoresButtonEntry);
			buttonsToSync.push_back(&helpButtonEntry);
			buttonsToSync.push_back(&quitButtonEntry);
			syncButtonWidths(buttonsToSync);

			// re-arrange the UI
			drawingMaster->uiLayoutAuthority->rearrange();

			// set up the button listeners with the new width and positions
			playButtonZoneListener->ll = ((UIMetrics*) playButtonEntry.second["metrics"])->position - ((UIMetrics*) playButtonEntry.second["metrics"])->size / 2.0f;
			playButtonZoneListener->ur = ((UIMetrics*) playButtonEntry.second["metrics"])->position + ((UIMetrics*) playButtonEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(playButtonZoneListener);
			playButtonClickListener->ll = playButtonZoneListener->ll;
			playButtonClickListener->ur = playButtonZoneListener->ur;
			inputHandler->mouse.addListener(playButtonClickListener);
			helpButtonZoneListener->ll = ((UIMetrics*) helpButtonEntry.second["metrics"])->position - ((UIMetrics*) helpButtonEntry.second["metrics"])->size / 2.0f;
			helpButtonZoneListener->ur = ((UIMetrics*) helpButtonEntry.second["metrics"])->position + ((UIMetrics*) helpButtonEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(helpButtonZoneListener);
			helpButtonClickListener->ll = helpButtonZoneListener->ll;
			helpButtonClickListener->ur = helpButtonZoneListener->ur;
			inputHandler->mouse.addListener(helpButtonClickListener);
			highScoresButtonZoneListener->ll = ((UIMetrics*) highScoresButtonEntry.second["metrics"])->position - ((UIMetrics*) highScoresButtonEntry.second["metrics"])->size / 2.0f;
			highScoresButtonZoneListener->ur = ((UIMetrics*) highScoresButtonEntry.second["metrics"])->position + ((UIMetrics*) highScoresButtonEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(highScoresButtonZoneListener);
			highScoresButtonClickListener->ll = highScoresButtonZoneListener->ll;
			highScoresButtonClickListener->ur = highScoresButtonZoneListener->ur;
			inputHandler->mouse.addListener(highScoresButtonClickListener);
			settingsButtonZoneListener->ll = ((UIMetrics*) settingsButtonEntry.second["metrics"])->position - ((UIMetrics*) settingsButtonEntry.second["metrics"])->size / 2.0f;
			settingsButtonZoneListener->ur = ((UIMetrics*) settingsButtonEntry.second["metrics"])->position + ((UIMetrics*) settingsButtonEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(settingsButtonZoneListener);
			settingsButtonClickListener->ll = settingsButtonZoneListener->ll;
			settingsButtonClickListener->ur = settingsButtonZoneListener->ur;
			inputHandler->mouse.addListener(settingsButtonClickListener);
			quitButtonZoneListener->ll = ((UIMetrics*) quitButtonEntry.second["metrics"])->position - ((UIMetrics*) quitButtonEntry.second["metrics"])->size / 2.0f;
			quitButtonZoneListener->ur = ((UIMetrics*) quitButtonEntry.second["metrics"])->position + ((UIMetrics*) quitButtonEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(quitButtonZoneListener);
			quitButtonClickListener->ll = quitButtonZoneListener->ll;
			quitButtonClickListener->ur = quitButtonZoneListener->ur;
			inputHandler->mouse.addListener(quitButtonClickListener);

			break;
		}
	case SCHEME_LOADING:
		{
			// input
			inputHandler->keyboard.addListener(quitKeyListener);
			inputHandler->keyboard.addListener(fullScreenKeyListener);

			// splash background
			drawingMaster->drawStack.push_back(splashEntry);

			// loading label
			*((float*) loadingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			((UIMetrics*) loadingEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(loadingEntry.second);
			drawingMaster->drawStack.push_back(loadingEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) loadingEntry.second["metrics"]);

			// re-arrange the UI
			drawingMaster->uiLayoutAuthority->rearrange();

			break;
		}
	case SCHEME_INTRO:
		{
			// input
			inputHandler->keyboard.addListener(quitKeyListener);
			inputHandler->keyboard.addListener(fullScreenKeyListener);
			inputHandler->keyboard.addListener(introKeyListener);
			inputHandler->mouse.addListener(primaryFireClickListener1);

			// sky
			drawingMaster->drawStack.push_back(skyEntry);

			// water
			drawingMaster->drawStack.push_back(waterEntry);

			// ships
			drawingMaster->drawStack.push_back(shipEntry);

			// missiles
			drawingMaster->drawStack.push_back(missileEntry);

			// missile trails
			drawingMaster->drawStack.push_back(missileTrailEntry);

			// shells
			drawingMaster->drawStack.push_back(shellEntry);

			// terrain
			drawingMaster->drawStack.push_back(terrainEntry);

			// explosions
			drawingMaster->drawStack.push_back(explosionEntry);

			// fortress
			drawingMaster->drawStack.push_back(fortressEntry);

			// tip
			*((float*) menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((std::string*) menuTip1Entry.second["text"]) = "LEFT CLICK or press SPACE to skip introduction";
			((UIMetrics*) menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip1Entry.second);
			drawingMaster->drawStack.push_back(menuTip1Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip1Entry.second["metrics"]);

			// re-arrange the UI
			drawingMaster->uiLayoutAuthority->rearrange();

			break;
		}
	case SCHEME_PLAYING:
		{
			// input
			inputHandler->mouse.addListener(mouseMotionListener);
			inputHandler->mouse.addListener(primaryFireClickListener1);
			inputHandler->mouse.addListener(primaryFireClickListener2);
			inputHandler->mouse.addListener(primaryFireClickListener3);
			inputHandler->mouse.addListener(secondaryFireClickListener);
			inputHandler->mouse.addListener(binocularsClickListener);
			inputHandler->keyboard.addListener(quitKeyListener);
			inputHandler->keyboard.addListener(fullScreenKeyListener);
			if(gameSystem->getBool("developmentMode")) inputHandler->keyboard.addListener(playingDevelopmentModeKeyListener);
			else inputHandler->keyboard.addListener(playingKeyListener);
			inputHandler->keyboard.addListener(cameraAheadKeyListener);
			inputHandler->keyboard.addListener(turretUpKeyListener);
			inputHandler->keyboard.addListener(turretDownKeyListener);
			inputHandler->keyboard.addListener(turretLeftKeyListener);
			inputHandler->keyboard.addListener(turretRightKeyListener);

			// sky
			drawingMaster->drawStack.push_back(skyEntry);

			// water
			drawingMaster->drawStack.push_back(waterEntry);

			// ships
			drawingMaster->drawStack.push_back(shipEntry);

			// missiles
			drawingMaster->drawStack.push_back(missileEntry);

			// missile trails
			drawingMaster->drawStack.push_back(missileTrailEntry);

			// shells
			drawingMaster->drawStack.push_back(shellEntry);

			// terrain
			drawingMaster->drawStack.push_back(terrainEntry);

			// explosions
			drawingMaster->drawStack.push_back(explosionEntry);

			// fortress
			drawingMaster->drawStack.push_back(fortressEntry);

			// missile strike effect
			drawingMaster->drawStack.push_back(strikeEffectEntry);

			// missile indicators
			if(gameGraphics->currentCamera == &fortressCamera) {
				*((Vector2*) missileIndicators.second["size"]) = Vector2(
						gameSystem->getFloat("hudMissileIndicatorSize") * (gameState->binoculars ? gameSystem->getFloat("hudMissileIndicatorBinocularsFactor") : 1.0f) / (float) gameGraphics->resolutionX * 2.0f,
						gameSystem->getFloat("hudMissileIndicatorSize") * (gameState->binoculars ? gameSystem->getFloat("hudMissileIndicatorBinocularsFactor") : 1.0f) / (float) gameGraphics->resolutionY * 2.0f
					);
				*((Vector2*) missileIndicators.second["arrowSize"]) = Vector2(
						gameSystem->getFloat("hudMissileArrowWidth") / (float) gameGraphics->resolutionX * 2.0f,
						gameSystem->getFloat("hudMissileArrowHeight") / (float) gameGraphics->resolutionY * 2.0f
					);
				drawingMaster->drawStack.push_back(missileIndicators);
			}

			// score
			*((float*) scoreLabel.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			char scoreString[8]; scoreString[0] = '\0'; if(gameState->score <= 9999999) sprintf(scoreString, "%u", gameState->score);
			*((std::string*) scoreLabel.second["text"]) = scoreString;
			((UIMetrics*) scoreLabel.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(scoreLabel.second);
			drawingMaster->drawStack.push_back(scoreLabel);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) scoreLabel.second["metrics"]);

			// gauge panel
			*((float*) gaugePanelEntry.second["padding"]) = gameSystem->getFloat("hudGaugePadding");
			*((float*) gaugePanelEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) gaugePanelEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			((std::vector<Vector2>*) gaugePanelEntry.second["textureSizes"])->at(0) = Vector2(
					gameSystem->getFloat("gaugeImagesHeight") * gameGraphics->getTexture("gauge/heart")->width / gameGraphics->getTexture("gauge/heart")->height / (float) gameGraphics->resolutionX * 2.0f,
					gameSystem->getFloat("gaugeImagesHeight") / (float) gameGraphics->resolutionY * 2.0f
				);
			((std::vector<Vector2>*) gaugePanelEntry.second["textureSizes"])->at(1) = Vector2(
					gameSystem->getFloat("gaugeImagesHeight") * gameGraphics->getTexture("gauge/shell")->width / gameGraphics->getTexture("gauge/shell")->height / (float) gameGraphics->resolutionX * 2.0f,
					gameSystem->getFloat("gaugeImagesHeight") / (float) gameGraphics->resolutionY * 2.0f
				);
			((std::vector<Vector2>*) gaugePanelEntry.second["textureSizes"])->at(2) = Vector2(
					gameSystem->getFloat("gaugeImagesHeight") * gameGraphics->getTexture("gauge/bolt")->width / gameGraphics->getTexture("gauge/bolt")->height / (float) gameGraphics->resolutionX * 2.0f,
					gameSystem->getFloat("gaugeImagesHeight") / (float) gameGraphics->resolutionY * 2.0f
				);
			*((Vector2*) gaugePanelEntry.second["progressBarSize"]) = Vector2(gameSystem->getFloat("hudGaugeWidth"), gameSystem->getFloat("hudGaugeHeight"));
			(*((std::vector<float>*) gaugePanelEntry.second["progressions"]))[0] = gameState->fortress.health;
			(*((std::vector<float>*) gaugePanelEntry.second["progressions"]))[1] = gameState->fortress.ammunition;
			(*((std::vector<float>*) gaugePanelEntry.second["progressions"]))[2] = (gameState->fortress.emp > 1.0f ? 2.0f - gameState->fortress.emp : gameState->fortress.emp);
			(*((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsTop"]))[0] = gameSystem->getColor("hudGaugeHealthBarColor");
			(*((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsTop"]))[1] = gameSystem->getColor("hudGaugeAmmoBarColor");
			(*((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsTop"]))[2] = gameSystem->getColor(gameState->fortress.emp > 1.0f ? "hudGaugeEMPChargingBarColor" : "hudGaugeEMPChargedBarColor");
			(*((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsBottom"]))[0] = Vector4(
					gameSystem->getColor("hudGaugeHealthBarColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
					gameSystem->getColor("hudGaugeHealthBarColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
					gameSystem->getColor("hudGaugeHealthBarColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
					gameSystem->getColor("hudGaugeHealthBarColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
				);
			(*((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsBottom"]))[1] = Vector4(
					gameSystem->getColor("hudGaugeAmmoBarColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
					gameSystem->getColor("hudGaugeAmmoBarColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
					gameSystem->getColor("hudGaugeAmmoBarColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
					gameSystem->getColor("hudGaugeAmmoBarColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
				);
			(*((std::vector<Vector4>*) gaugePanelEntry.second["progressBarColorsBottom"]))[2] = Vector4(
					gameSystem->getColor(gameState->fortress.emp > 1.0f ? "hudGaugeEMPChargingBarColor" : "hudGaugeEMPChargedBarColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
					gameSystem->getColor(gameState->fortress.emp > 1.0f ? "hudGaugeEMPChargingBarColor" : "hudGaugeEMPChargedBarColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
					gameSystem->getColor(gameState->fortress.emp > 1.0f ? "hudGaugeEMPChargingBarColor" : "hudGaugeEMPChargedBarColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
					gameSystem->getColor(gameState->fortress.emp > 1.0f ? "hudGaugeEMPChargingBarColor" : "hudGaugeEMPChargedBarColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
				);
			((UIMetrics*) gaugePanelEntry.second["metrics"])->size = ((DrawGaugePanel*) drawingMaster->drawers["gaugePanel"])->getSize(gaugePanelEntry.second);
			drawingMaster->drawStack.push_back(gaugePanelEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gaugePanelEntry.second["metrics"]);

			// radar
			*((float*) radarEntry.second["padding"]) = gameSystem->getFloat("hudGaugePadding");
			*((float*) radarEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) radarEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			((UIMetrics*) radarEntry.second["metrics"])->size = ((DrawRadar*) drawingMaster->drawers["radar"])->getSize(radarEntry.second);
			drawingMaster->drawStack.push_back(radarEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) radarEntry.second["metrics"]);

			// tip
			*((float*) menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((std::string*) menuTip1Entry.second["text"]) = "Press ESC to pause and show options";
			((UIMetrics*) menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip1Entry.second);
			drawingMaster->drawStack.push_back(menuTip1Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip1Entry.second["metrics"]);

			// development mode controls
			if(gameSystem->getBool("developmentMode")) {
				*((float*) develControlsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
				((UIMetrics*) develControlsTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(develControlsTitleEntry.second);

				*((float*) develControlsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
				((UIMetrics*) develControlsEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(develControlsEntry.second);

				*((float*) develControlsContainerEntry.second["padding"]) = gameSystem->getFloat("hudContainerPadding");
				*((float*) develControlsContainerEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
				*((float*) develControlsContainerEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
				*((Vector2*) develControlsContainerEntry.second["size"]) = Vector2(
						(((DrawLabel*) drawingMaster->drawers["label"])->getSize(develControlsTitleEntry.second).x >
								((DrawLabel*) drawingMaster->drawers["label"])->getSize(develControlsEntry.second).x ?
								((DrawLabel*) drawingMaster->drawers["label"])->getSize(develControlsTitleEntry.second).x :
								((DrawLabel*) drawingMaster->drawers["label"])->getSize(develControlsEntry.second).x) +
								gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionX * 4.0f,
						((DrawLabel*) drawingMaster->drawers["label"])->getSize(develControlsTitleEntry.second).y +
								(float) gameGraphics->fontManager->lineHeights[gameSystem->getFloat("fontSizeSmall")] / gameGraphics->resolutionY * 2.0f +
								((DrawLabel*) drawingMaster->drawers["label"])->getSize(develControlsEntry.second).y +
								gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionY * 2.0f // half of container padding in y when rendering text
					);
				((UIMetrics*) develControlsContainerEntry.second["metrics"])->size = *((Vector2*) develControlsContainerEntry.second["size"]);
				drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) develControlsContainerEntry.second["metrics"]);
				drawingMaster->drawStack.push_back(develControlsContainerEntry);
				drawingMaster->drawStack.push_back(develControlsTitleEntry);
				drawingMaster->drawStack.push_back(develControlsEntry);
			}

			// control spot
			*((Vector2*) controlSpotEntry.second["size"]) = Vector2(
				gameSystem->getFloat("hudControlSpotSize") * 2.0f / (float) gameGraphics->resolutionX,
				gameSystem->getFloat("hudControlSpotSize") * 2.0f / (float) gameGraphics->resolutionY
			);
			*((Vector2*) controlSpotEntry.second["position"]) = Vector2(0.0f, 0.0f);
			*((float*) controlSpotEntry.second["border"]) = gameSystem->getFloat("hudControlSpotBorder");
			*((float*) controlSpotEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			*((Vector4*) controlSpotEntry.second["insideColor"]) = Vector4(
					gameSystem->getColor("hudControlSpotColor").x,
					gameSystem->getColor("hudControlSpotColor").y,
					gameSystem->getColor("hudControlSpotColor").z,
					0.0f
				);
			drawingMaster->drawStack.push_back(controlSpotEntry);

			// cursor
			if(mouseActive) {
				*((Vector2*) cursorEntry.second["size"]) = Vector2(
					gameSystem->getFloat("hudCursorSize") * 2.0f / (float) gameGraphics->resolutionX,
					gameSystem->getFloat("hudCursorSize") * 2.0f / (float) gameGraphics->resolutionY
				);
				*((Vector2*)cursorEntry.second["position"]) = inputHandler->mouse.position;
				*((float*) cursorEntry.second["border"]) = 0.0f;
				*((float*) cursorEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
				drawingMaster->drawStack.push_back(cursorEntry);
			}

			// re-arrange the UI
			drawingMaster->uiLayoutAuthority->rearrange();

			// set post-UI arrangement container content locations
			if(gameSystem->getBool("developmentMode")) {
				((UIMetrics*) develControlsTitleEntry.second["metrics"])->position = Vector2(
						((UIMetrics*) develControlsContainerEntry.second["metrics"])->position.x,
						((UIMetrics*) develControlsContainerEntry.second["metrics"])->position.y +
								((UIMetrics*) develControlsContainerEntry.second["metrics"])->size.y / 2.0f -
								gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionY - // half of container padding in y when rendering text
								((UIMetrics*) develControlsTitleEntry.second["metrics"])->size.y / 2.0f
					);

				((UIMetrics*) develControlsEntry.second["metrics"])->position = Vector2(
						((UIMetrics*) develControlsContainerEntry.second["metrics"])->position.x -
								((UIMetrics*) develControlsContainerEntry.second["metrics"])->size.x / 2.0f +
								gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionX * 2.0f +
								((UIMetrics*) develControlsEntry.second["metrics"])->size.x / 2.0f,
						((UIMetrics*) develControlsContainerEntry.second["metrics"])->position.y -
								((UIMetrics*) develControlsContainerEntry.second["metrics"])->size.y / 2.0f +
								gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionY + // half of container padding in y when rendering text
								((UIMetrics*) develControlsEntry.second["metrics"])->size.y / 2.0f
					);
			}

			break;
		}
	case SCHEME_PAUSED:
		{
			// input
			inputHandler->mouse.addListener(mouseMotionListener);
			inputHandler->keyboard.addListener(quitKeyListener);
			inputHandler->keyboard.addListener(fullScreenKeyListener);
			inputHandler->keyboard.addListener(pausedMenuKeyListener);

			// sky
			drawingMaster->drawStack.push_back(skyEntry);

			// water
			drawingMaster->drawStack.push_back(waterEntry);

			// ships
			drawingMaster->drawStack.push_back(shipEntry);

			// missiles
			drawingMaster->drawStack.push_back(missileEntry);

			// missile trails
			drawingMaster->drawStack.push_back(missileTrailEntry);

			// shells
			drawingMaster->drawStack.push_back(shellEntry);

			// terrain
			drawingMaster->drawStack.push_back(terrainEntry);

			// explosions
			drawingMaster->drawStack.push_back(explosionEntry);

			// fortress
			drawingMaster->drawStack.push_back(fortressEntry);

			// gray out
			drawingMaster->drawStack.push_back(grayOutEntry);

			// paused title label
			*((float*) menuTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((std::string*) menuTitleEntry.second["text"]) = "PAUSED";
			((UIMetrics*) menuTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTitleEntry.second);
			drawingMaster->drawStack.push_back(menuTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTitleEntry.second["metrics"]);

			// controls title label
			*((float*) controlsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			((UIMetrics*) controlsTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(controlsTitleEntry.second);
			drawingMaster->drawStack.push_back(controlsTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) controlsTitleEntry.second["metrics"]);

			// controls label
			*((float*) controlsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) controlsEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(controlsEntry.second);
			drawingMaster->drawStack.push_back(controlsEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) controlsEntry.second["metrics"]);

			// spacer
			drawingMaster->uiLayoutAuthority->metrics.push_back(spacerMetrics);

			// resume button
			*((float*) resumeButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((float*) resumeButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
			*((float*) resumeButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) resumeButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			Vector2* resumeButtonSize = (Vector2*) resumeButtonEntry.second["size"];
			resumeButtonEntry.second.erase(resumeButtonEntry.second.find("size"));
			*resumeButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(resumeButtonEntry.second);
			resumeButtonEntry.second["size"] = resumeButtonSize;
			((UIMetrics*) resumeButtonEntry.second["metrics"])->size = *resumeButtonSize;
			*((Vector4*) resumeButtonEntry.second["insideColor"]) = (
					activeMenuSelection == &resumeButtonEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			drawingMaster->drawStack.push_back(resumeButtonEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) resumeButtonEntry.second["metrics"]);

			// end game button
			*((float*) endGameButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((float*) endGameButtonEntry.second["padding"]) = gameSystem->getFloat("hudBigButtonPadding");
			*((float*) endGameButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) endGameButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			*((Vector4*) endGameButtonEntry.second["insideColor"]) = (
					activeMenuSelection == &endGameButtonEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			Vector2* endGameButtonSize = (Vector2*) endGameButtonEntry.second["size"];
			resumeButtonEntry.second.erase(endGameButtonEntry.second.find("size"));
			*endGameButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(endGameButtonEntry.second);
			endGameButtonEntry.second["size"] = endGameButtonSize;
			((UIMetrics*) endGameButtonEntry.second["metrics"])->size = *endGameButtonSize;
			drawingMaster->drawStack.push_back(endGameButtonEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) endGameButtonEntry.second["metrics"]);

			// tips
			*((float*) menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((std::string*) menuTip1Entry.second["text"]) = "Press ESC to return to the game";
			((UIMetrics*) menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip1Entry.second);
			drawingMaster->drawStack.push_back(menuTip1Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip1Entry.second["metrics"]);

			*((float*) menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) menuTip2Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip2Entry.second);
			drawingMaster->drawStack.push_back(menuTip2Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip2Entry.second["metrics"]);

			*((float*) menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) menuTip3Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip3Entry.second);
			drawingMaster->drawStack.push_back(menuTip3Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip3Entry.second["metrics"]);

			// make all buttons the same width
			std::vector<DrawStackEntry*> buttonsToSync;
			buttonsToSync.push_back(&resumeButtonEntry);
			buttonsToSync.push_back(&endGameButtonEntry);
			syncButtonWidths(buttonsToSync);

			// re-arrange the UI
			drawingMaster->uiLayoutAuthority->rearrange();

			// set up button listeners with new position
			resumeButtonZoneListener->ll = ((UIMetrics*) resumeButtonEntry.second["metrics"])->position -	((UIMetrics*) resumeButtonEntry.second["metrics"])->size / 2.0f;
			resumeButtonZoneListener->ur = ((UIMetrics*) resumeButtonEntry.second["metrics"])->position + ((UIMetrics*) resumeButtonEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(resumeButtonZoneListener);
			resumeButtonClickListener->ll = resumeButtonZoneListener->ll;
			resumeButtonClickListener->ur = resumeButtonZoneListener->ur;
			inputHandler->mouse.addListener(resumeButtonClickListener);

			endGameButtonZoneListener->ll = ((UIMetrics*) endGameButtonEntry.second["metrics"])->position -	((UIMetrics*) endGameButtonEntry.second["metrics"])->size / 2.0f;
			endGameButtonZoneListener->ur = ((UIMetrics*) endGameButtonEntry.second["metrics"])->position + ((UIMetrics*) endGameButtonEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(endGameButtonZoneListener);
			endGameButtonClickListener->ll = endGameButtonZoneListener->ll;
			endGameButtonClickListener->ur = endGameButtonZoneListener->ur;
			inputHandler->mouse.addListener(endGameButtonClickListener);

			break;
		}
	case SCHEME_GAMEOVER:
		{
			// input
			inputHandler->mouse.addListener(mouseMotionListener);
			inputHandler->keyboard.addListener(quitKeyListener);
			inputHandler->keyboard.addListener(fullScreenKeyListener);
			inputHandler->keyboard.addListener(gameOverKeyListener);
			inputHandler->keyboard.addListener(deleteKeyListener);

			// splash background
			drawingMaster->drawStack.push_back(splashEntry);

			// game over title label
			*((float*) menuTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((std::string*) menuTitleEntry.second["text"]) = "GAME OVER";
			((UIMetrics*) menuTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTitleEntry.second);
			drawingMaster->drawStack.push_back(menuTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTitleEntry.second["metrics"]);

			// score title label
			*((float*) yourScoreTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			((UIMetrics*) yourScoreTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(yourScoreTitleEntry.second);
			drawingMaster->drawStack.push_back(yourScoreTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) yourScoreTitleEntry.second["metrics"]);

			// score label
			*((float*) yourScoreEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			char scoreString[8]; scoreString[0] = '\0'; if(gameState->score <= 9999999) sprintf(scoreString, "%u", gameState->score);
			*((std::string*) yourScoreEntry.second["text"]) = (gameSystem->getFloat("gameStartingLevel") == 1.0f ? "Easy" : gameSystem->getFloat("gameStartingLevel") == 2.0f ? "Medium" : "Hard");
			*((std::string*) yourScoreEntry.second["text"]) += "\t";
			*((std::string*) yourScoreEntry.second["text"]) += scoreString;
			((UIMetrics*) yourScoreEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(yourScoreEntry.second);
			drawingMaster->drawStack.push_back(yourScoreEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) yourScoreEntry.second["metrics"]);

			// high scores section label
			*((float*) highScoresTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			((UIMetrics*) highScoresTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(highScoresTitleEntry.second);
			drawingMaster->drawStack.push_back(highScoresTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) highScoresTitleEntry.second["metrics"]);

			// high scores label
			*((float*) highScoresEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			// imperfect without a fixed-width font, but pad shorter numbers with invisible zeros so it lines up better
			std::stringstream stringStream;
			size_t scoreLength = 0;
			if(gameSystem->highScores.size() > 0) {
				stringStream << gameSystem->highScores[0].first;
				scoreLength = stringStream.str().length();
				stringStream.str("");
			}
			for(size_t i = 0; i < gameSystem->highScores.size(); ++i) {
				std::stringstream originalNumber;
				originalNumber << gameSystem->highScores[i].first;

				stringStream << (i > 0 ? "\n" : "") << gameSystem->highScores[i].second << "\t";

				for(size_t p = originalNumber.str().length(); p < scoreLength; ++p)
					stringStream << "\\ffffff000"; // zero-alpha '0' character

				stringStream << "\\" << gameSystem->getString("fontColorLight");
				stringStream << gameSystem->highScores[i].first;
			}
			if(stringStream.str().length() == 0) stringStream.str("No high scores have been recorded yet.");
			*((std::string*) highScoresEntry.second["text"]) = stringStream.str();
			((UIMetrics*) highScoresEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(highScoresEntry.second);
			drawingMaster->drawStack.push_back(highScoresEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) highScoresEntry.second["metrics"]);

			if(gameState->score > 0 && (gameSystem->highScores.size() == 0 || gameState->score > gameSystem->highScores.back().first)) {
				// high scores label
				*((float*) newHighScoreTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
				((UIMetrics*) newHighScoreTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(newHighScoreTitleEntry.second);
				drawingMaster->drawStack.push_back(newHighScoreTitleEntry);
				drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) newHighScoreTitleEntry.second["metrics"]);

				// name prompt label
				*((float*) newHighScoreNameLabel.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
				((UIMetrics*) newHighScoreNameLabel.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(newHighScoreNameLabel.second);

				// name prompt field
				*((float*) newHighScoreNameField.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
				*((std::string*) newHighScoreNameField.second["text"]) = "";
				for(size_t i = 0; i < gameSystem->getFloat("hudFieldWidth") + 1; ++i) *((std::string*) newHighScoreNameField.second["text"]) += "#";
				*((Vector2*) newHighScoreNameField.second["size"]) = ((DrawField*) drawingMaster->drawers["field"])->getSize(newHighScoreNameField.second);
				((UIMetrics*) newHighScoreNameField.second["metrics"])->size = *((Vector2*) newHighScoreNameField.second["size"]);
				*((std::string*) newHighScoreNameField.second["text"]) = "\\";
				*((std::string*) newHighScoreNameField.second["text"]) += gameSystem->getString("fontColorLight");
				*((std::string*) newHighScoreNameField.second["text"]) += playerName;
//				*((std::string*) newHighScoreNameField.second["text"]) += "\\";
//				*((std::string*) newHighScoreNameField.second["text"]) += gameSystem->getString("fontColorDark");
//				*((std::string*) newHighScoreNameField.second["text"]) += "|";

				// name prompt container
				*((float*) newHighScoreContainer.second["padding"]) = gameSystem->getFloat("hudContainerPadding");
				*((float*) newHighScoreContainer.second["border"]) = 0.0f;
				*((float*) newHighScoreContainer.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
				*((Vector2*) newHighScoreContainer.second["size"]) = Vector2(
						((UIMetrics*) newHighScoreNameLabel.second["metrics"])->size.x +
								((UIMetrics*) newHighScoreNameField.second["metrics"])->size.x +
								gameSystem->getFloat("hudContainerPadding") / gameGraphics->resolutionX * 4.0f +
								gameSystem->getFloat("hudElementMargin") / gameGraphics->resolutionX * 2.0f,
						(((UIMetrics*) newHighScoreNameLabel.second["metrics"])->size.y >
								((UIMetrics*) newHighScoreNameField.second["metrics"])->size.y ?
								((UIMetrics*) newHighScoreNameLabel.second["metrics"])->size.y :
								((UIMetrics*) newHighScoreNameField.second["metrics"])->size.y) +
								gameSystem->getFloat("hudContainerPadding") / gameGraphics->resolutionY * 4.0f
					);
				((UIMetrics*) newHighScoreContainer.second["metrics"])->size = *((Vector2*) newHighScoreContainer.second["size"]);
				drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) newHighScoreContainer.second["metrics"]);
			}

			// continue button
			*((float*) gameOverContinueButton.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((float*) gameOverContinueButton.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
			*((float*) gameOverContinueButton.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) gameOverContinueButton.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			*((Vector4*) gameOverContinueButton.second["insideColor"]) = (
					activeMenuSelection == &gameOverContinueButton ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			((UIMetrics*) gameOverContinueButton.second["metrics"])->size = ((DrawButton*) drawingMaster->drawers["button"])->getSize(gameOverContinueButton.second);
			drawingMaster->drawStack.push_back(gameOverContinueButton);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) gameOverContinueButton.second["metrics"]);

			// re-arrange the UI
			drawingMaster->uiLayoutAuthority->rearrange();

			if(gameState->score > 0 && (gameSystem->highScores.size() == 0 || gameState->score > gameSystem->highScores.back().first)) {
				// assemble prompt container contents
				((UIMetrics*) newHighScoreNameLabel.second["metrics"])->position = Vector2(
						((UIMetrics*) newHighScoreContainer.second["metrics"])->position.x -
								((UIMetrics*) newHighScoreContainer.second["metrics"])->size.x / 2.0f +
								gameSystem->getFloat("hudContainerPadding") / gameGraphics->resolutionX * 2.0f +
								((UIMetrics*) newHighScoreNameLabel.second["metrics"])->size.x / 2.0f,
						((UIMetrics*) newHighScoreContainer.second["metrics"])->position.y
					);
				((UIMetrics*) newHighScoreNameField.second["metrics"])->position = Vector2(
						((UIMetrics*) newHighScoreContainer.second["metrics"])->position.x +
								((UIMetrics*) newHighScoreContainer.second["metrics"])->size.x / 2.0f -
								gameSystem->getFloat("hudContainerPadding") / gameGraphics->resolutionX * 2.0f -
								((UIMetrics*) newHighScoreNameField.second["metrics"])->size.x / 2.0f,
						((UIMetrics*) newHighScoreContainer.second["metrics"])->position.y
					);
				drawingMaster->drawStack.push_back(newHighScoreContainer);
				drawingMaster->drawStack.push_back(newHighScoreNameLabel);
				drawingMaster->drawStack.push_back(newHighScoreNameField);
			}

			// set up button listener with new position
			gameOverContinueButtonZoneListener->ll = ((UIMetrics*) gameOverContinueButton.second["metrics"])->position -	((UIMetrics*) gameOverContinueButton.second["metrics"])->size / 2.0f;
			gameOverContinueButtonZoneListener->ur = ((UIMetrics*) gameOverContinueButton.second["metrics"])->position + ((UIMetrics*) gameOverContinueButton.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(gameOverContinueButtonZoneListener);
			gameOverContinueButtonClickListener->ll = gameOverContinueButtonZoneListener->ll;
			gameOverContinueButtonClickListener->ur = gameOverContinueButtonZoneListener->ur;
			inputHandler->mouse.addListener(gameOverContinueButtonClickListener);

			break;
		}
	case SCHEME_HELP:
		{
			// input
			inputHandler->mouse.addListener(mouseMotionListener);
			inputHandler->keyboard.addListener(quitKeyListener);
			inputHandler->keyboard.addListener(fullScreenKeyListener);
			inputHandler->keyboard.addListener(helpMenuKeyListener);

			// splash background
			drawingMaster->drawStack.push_back(splashEntry);

			// help title label
			*((float*) menuTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((std::string*) menuTitleEntry.second["text"]) = "HELP";
			((UIMetrics*) menuTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTitleEntry.second);
			drawingMaster->drawStack.push_back(menuTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTitleEntry.second["metrics"]);

			// controls title label
			*((float*) controlsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			((UIMetrics*) controlsTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(controlsTitleEntry.second);
			drawingMaster->drawStack.push_back(controlsTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) controlsTitleEntry.second["metrics"]);

			// controls label
			*((float*) controlsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) controlsEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(controlsEntry.second);
			drawingMaster->drawStack.push_back(controlsEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) controlsEntry.second["metrics"]);

			// instructions title label
			*((float*) instructionsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			((UIMetrics*) instructionsTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(instructionsTitleEntry.second);
			drawingMaster->drawStack.push_back(instructionsTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) instructionsTitleEntry.second["metrics"]);

			// instructions label
			*((float*) instructionsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((float*) instructionsEntry.second["wrap"]) = 2.0f * (gameSystem->getFloat("helpTextScreenPortion")) - (gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX);
			((UIMetrics*) instructionsEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(instructionsEntry.second);
			drawingMaster->drawStack.push_back(instructionsEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) instructionsEntry.second["metrics"]);

			// about button
			*((float*) aboutButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((float*) aboutButtonEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
			*((float*) aboutButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) aboutButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			*((Vector4*) aboutButtonEntry.second["insideColor"]) = (
					activeMenuSelection == &aboutButtonEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			Vector2* aboutButtonSize = (Vector2*) aboutButtonEntry.second["size"];
			backButtonEntry.second.erase(aboutButtonEntry.second.find("size"));
			*aboutButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(aboutButtonEntry.second);
			aboutButtonEntry.second["size"] = aboutButtonSize;
			((UIMetrics*) aboutButtonEntry.second["metrics"])->size = *aboutButtonSize;
			drawingMaster->drawStack.push_back(aboutButtonEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) aboutButtonEntry.second["metrics"]);

			// back button
			*((float*) backButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((float*) backButtonEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
			*((float*) backButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) backButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			*((Vector4*) backButtonEntry.second["insideColor"]) = (
					activeMenuSelection == &backButtonEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			Vector2* backButtonSize = (Vector2*) backButtonEntry.second["size"];
			backButtonEntry.second.erase(backButtonEntry.second.find("size"));
			*backButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(backButtonEntry.second);
			backButtonEntry.second["size"] = backButtonSize;
			((UIMetrics*) backButtonEntry.second["metrics"])->size = *backButtonSize;
			drawingMaster->drawStack.push_back(backButtonEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) backButtonEntry.second["metrics"]);

			// tips
			*((float*) menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((std::string*) menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
			((UIMetrics*) menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip1Entry.second);
			drawingMaster->drawStack.push_back(menuTip1Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip1Entry.second["metrics"]);

			*((float*) menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) menuTip2Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip2Entry.second);
			drawingMaster->drawStack.push_back(menuTip2Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip2Entry.second["metrics"]);

			*((float*) menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) menuTip3Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip3Entry.second);
			drawingMaster->drawStack.push_back(menuTip3Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip3Entry.second["metrics"]);

			// make all buttons the same width
			std::vector<DrawStackEntry*> buttonsToSync;
			buttonsToSync.push_back(&aboutButtonEntry);
			buttonsToSync.push_back(&backButtonEntry);
			syncButtonWidths(buttonsToSync);

			// re-arrange the UI
			drawingMaster->uiLayoutAuthority->rearrange();

			// set up button listeners with new position
			aboutButtonZoneListener->ll = ((UIMetrics*) aboutButtonEntry.second["metrics"])->position -	((UIMetrics*) aboutButtonEntry.second["metrics"])->size / 2.0f;
			aboutButtonZoneListener->ur = ((UIMetrics*) aboutButtonEntry.second["metrics"])->position + ((UIMetrics*) aboutButtonEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(aboutButtonZoneListener);
			aboutButtonClickListener->ll = aboutButtonZoneListener->ll;
			aboutButtonClickListener->ur = aboutButtonZoneListener->ur;
			inputHandler->mouse.addListener(aboutButtonClickListener);
			backButtonZoneListener->ll = ((UIMetrics*) backButtonEntry.second["metrics"])->position -	((UIMetrics*) backButtonEntry.second["metrics"])->size / 2.0f;
			backButtonZoneListener->ur = ((UIMetrics*) backButtonEntry.second["metrics"])->position + ((UIMetrics*) backButtonEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(backButtonZoneListener);
			backButtonClickListener->ll = backButtonZoneListener->ll;
			backButtonClickListener->ur = backButtonZoneListener->ur;
			inputHandler->mouse.addListener(backButtonClickListener);

			break;
		}
	case SCHEME_ABOUT:
		{
			// input
			inputHandler->mouse.addListener(mouseMotionListener);
			inputHandler->keyboard.addListener(quitKeyListener);
			inputHandler->keyboard.addListener(fullScreenKeyListener);
			inputHandler->keyboard.addListener(helpMenuKeyListener);

			// splash background
			drawingMaster->drawStack.push_back(splashEntry);

			// about title label
			*((float*) menuTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((std::string*) menuTitleEntry.second["text"]) = "ABOUT";
			((UIMetrics*) menuTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTitleEntry.second);
			drawingMaster->drawStack.push_back(menuTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTitleEntry.second["metrics"]);

			// version title label
			*((float*) versionTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			((UIMetrics*) versionTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(versionTitleEntry.second);
			drawingMaster->drawStack.push_back(versionTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) versionTitleEntry.second["metrics"]);

			// version label
			*((float*) versionEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((float*) versionEntry.second["wrap"]) = 2.0f * (gameSystem->getFloat("helpTextScreenPortion")) - (gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX);
//			*((std::string*) versionEntry.second["text"]) = gameSystem->versionString;
			std::stringstream sstream;
			sstream << "Version:\t" << PROGRAM_VERSION << "\n";
			sstream << "Status:\t" << PROGRAM_BUILDSTRING << "\n";
			sstream << "Architecture:\t" << PROGRAM_ARCH_STR << "\n";
			sstream << "Build Date:\t" << gameSystem->buildDate;
			*((std::string*) versionEntry.second["text"]) = sstream.str();
			((UIMetrics*) versionEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(versionEntry.second);
			drawingMaster->drawStack.push_back(versionEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) versionEntry.second["metrics"]);

			// credits title label
			*((float*) creditsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			((UIMetrics*) creditsTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(creditsTitleEntry.second);
			drawingMaster->drawStack.push_back(creditsTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) creditsTitleEntry.second["metrics"]);

			// credits label
			*((float*) creditsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((float*) creditsEntry.second["wrap"]) = 2.0f * (gameSystem->getFloat("helpTextScreenPortion")) - (gameSystem->getFloat("hudElementMargin") * 2.0f / (float) gameGraphics->resolutionX);
			((UIMetrics*) creditsEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(creditsEntry.second);
			drawingMaster->drawStack.push_back(creditsEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) creditsEntry.second["metrics"]);

			// back button
			*((float*) backButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((float*) backButtonEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
			*((float*) backButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) backButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			*((Vector4*) backButtonEntry.second["insideColor"]) = (
					activeMenuSelection == &backButtonEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			Vector2* backButtonSize = (Vector2*) backButtonEntry.second["size"];
			backButtonEntry.second.erase(backButtonEntry.second.find("size"));
			*backButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(backButtonEntry.second);
			backButtonEntry.second["size"] = backButtonSize;
			((UIMetrics*) backButtonEntry.second["metrics"])->size = *backButtonSize;
			drawingMaster->drawStack.push_back(backButtonEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) backButtonEntry.second["metrics"]);

			// tips
			*((float*) menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((std::string*) menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
			((UIMetrics*) menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip1Entry.second);
			drawingMaster->drawStack.push_back(menuTip1Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip1Entry.second["metrics"]);

			*((float*) menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) menuTip2Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip2Entry.second);
			drawingMaster->drawStack.push_back(menuTip2Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip2Entry.second["metrics"]);

			*((float*) menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) menuTip3Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip3Entry.second);
			drawingMaster->drawStack.push_back(menuTip3Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip3Entry.second["metrics"]);

			// re-arrange the UI
			drawingMaster->uiLayoutAuthority->rearrange();

			// set up button listener with new position
			backButtonZoneListener->ll = ((UIMetrics*) backButtonEntry.second["metrics"])->position -	((UIMetrics*) backButtonEntry.second["metrics"])->size / 2.0f;
			backButtonZoneListener->ur = ((UIMetrics*) backButtonEntry.second["metrics"])->position + ((UIMetrics*) backButtonEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(backButtonZoneListener);
			backButtonClickListener->ll = backButtonZoneListener->ll;
			backButtonClickListener->ur = backButtonZoneListener->ur;
			inputHandler->mouse.addListener(backButtonClickListener);

			break;
		}
	case SCHEME_HIGHSCORES:
		{
			// input
			inputHandler->mouse.addListener(mouseMotionListener);
			inputHandler->keyboard.addListener(quitKeyListener);
			inputHandler->keyboard.addListener(fullScreenKeyListener);
			inputHandler->keyboard.addListener(highScoresMenuKeyListener);

			// splash background
			drawingMaster->drawStack.push_back(splashEntry);

			// high scores title label
			*((float*) menuTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((std::string*) menuTitleEntry.second["text"]) = "HIGH SCORES";
			((UIMetrics*) menuTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTitleEntry.second);
			drawingMaster->drawStack.push_back(menuTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTitleEntry.second["metrics"]);

			// high scores label
			*((float*) highScoresEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			// imperfect without a fixed-width font, but pad shorter numbers with invisible zeros so it lines up better
			std::stringstream stringStream;
			size_t scoreLength = 0;
			if(gameSystem->highScores.size() > 0) {
				stringStream << gameSystem->highScores[0].first;
				scoreLength = stringStream.str().length();
				stringStream.str("");
			}
			for(size_t i = 0; i < gameSystem->highScores.size(); ++i) {
				std::stringstream originalNumber;
				originalNumber << gameSystem->highScores[i].first;

				stringStream << (i > 0 ? "\n" : "") << gameSystem->highScores[i].second << "\t";

				for(size_t p = originalNumber.str().length(); p < scoreLength; ++p)
					stringStream << "\\ffffff000"; // zero-alpha '0' character

				stringStream << "\\" << gameSystem->getString("fontColorLight");
				stringStream << gameSystem->highScores[i].first;
			}
			if(stringStream.str().length() == 0) stringStream.str("No high scores have been recorded yet.");
			*((std::string*) highScoresEntry.second["text"]) = stringStream.str();
			((UIMetrics*) highScoresEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(highScoresEntry.second);
			drawingMaster->drawStack.push_back(highScoresEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) highScoresEntry.second["metrics"]);


			// back button
			*((float*) backButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((float*) backButtonEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
			*((float*) backButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) backButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			*((Vector4*) backButtonEntry.second["insideColor"]) = (
					activeMenuSelection == &backButtonEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			Vector2* backButtonSize = (Vector2*) backButtonEntry.second["size"];
			backButtonEntry.second.erase(backButtonEntry.second.find("size"));
			*backButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(backButtonEntry.second);
			backButtonEntry.second["size"] = backButtonSize;
			((UIMetrics*) backButtonEntry.second["metrics"])->size = *backButtonSize;
			drawingMaster->drawStack.push_back(backButtonEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) backButtonEntry.second["metrics"]);

			// tips
			*((float*) menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((std::string*) menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
			((UIMetrics*) menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip1Entry.second);
			drawingMaster->drawStack.push_back(menuTip1Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip1Entry.second["metrics"]);

			*((float*) menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) menuTip2Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip2Entry.second);
			drawingMaster->drawStack.push_back(menuTip2Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip2Entry.second["metrics"]);

			*((float*) menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) menuTip3Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip3Entry.second);
			drawingMaster->drawStack.push_back(menuTip3Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip3Entry.second["metrics"]);

			// re-arrange the UI
			drawingMaster->uiLayoutAuthority->rearrange();

			// set up button listener with new position
			backButtonZoneListener->ll = ((UIMetrics*) backButtonEntry.second["metrics"])->position -	((UIMetrics*) backButtonEntry.second["metrics"])->size / 2.0f;
			backButtonZoneListener->ur = ((UIMetrics*) backButtonEntry.second["metrics"])->position + ((UIMetrics*) backButtonEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(backButtonZoneListener);
			backButtonClickListener->ll = backButtonZoneListener->ll;
			backButtonClickListener->ur = backButtonZoneListener->ur;
			inputHandler->mouse.addListener(backButtonClickListener);

			break;
		}
	case SCHEME_SETTINGS:
		{
			// input
			inputHandler->mouse.addListener(mouseMotionListener);
			inputHandler->keyboard.addListener(quitKeyListener);
			inputHandler->keyboard.addListener(fullScreenKeyListener);
			inputHandler->keyboard.addListener(settingsMenuKeyListener);

			// splash background
			drawingMaster->drawStack.push_back(splashEntry);

			// settings title label
			*((float*) menuTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeLarge");
			*((std::string*) menuTitleEntry.second["text"]) = "SETTINGS";
			((UIMetrics*) menuTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTitleEntry.second);
			drawingMaster->drawStack.push_back(menuTitleEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTitleEntry.second["metrics"]);

			// level setting control label
			*((float*) levelSettingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			*((Vector4*) levelSettingEntry.second["fontColor"]) = (activeMenuSelection == &levelSettingEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
			std::stringstream ss; ss << "Starting Level: ";
			if(gameSystem->getFloat("gameStartingLevel") == 1)
				ss << "Easy";
			else if(gameSystem->getFloat("gameStartingLevel") == 2)
				ss << "Medium";
			else
				ss << "Hard";
			*((std::string*) levelSettingEntry.second["text"]) = ss.str().c_str();
			((UIMetrics*) levelSettingEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(levelSettingEntry.second);
			drawingMaster->drawStack.push_back(levelSettingEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) levelSettingEntry.second["metrics"]);

			// music setting control label
			*((float*) musicSettingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			*((Vector4*) musicSettingEntry.second["fontColor"]) = (activeMenuSelection == &musicSettingEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
			ss.str(""); ss << "Music Volume: ";
			int musicLevelInt = (int) (gameSystem->getFloat("audioMusicVolume") * 10.0f);
			std::string musicLevelString = "Off";
			if(musicLevelInt > 0.0f) {
				musicLevelString = "-----------";
				musicLevelString.replace((size_t) (musicLevelInt), 1, "|");
			}
			ss << "[ " << musicLevelString << " ]";
			*((std::string*) musicSettingEntry.second["text"]) = ss.str().c_str();
			((UIMetrics*) musicSettingEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(musicSettingEntry.second);
			drawingMaster->drawStack.push_back(musicSettingEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) musicSettingEntry.second["metrics"]);

			// audio effects setting control label
			*((float*) audioEffectsSettingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			*((Vector4*) audioEffectsSettingEntry.second["fontColor"]) = (activeMenuSelection == &audioEffectsSettingEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
			ss.str(""); ss << "Audio Effects Volume: ";
			int audioEffectsLevelInt = (int) (gameSystem->getFloat("audioEffectsVolume") * 10.0f);
			std::string audioEffectsLevelString = "Off";
			if(audioEffectsLevelInt > 0.0f) {
				audioEffectsLevelString = "-----------";
				audioEffectsLevelString.replace((size_t) (audioEffectsLevelInt), 1, "|");
			}
			ss << "[ " << audioEffectsLevelString << " ]";
			*((std::string*) audioEffectsSettingEntry.second["text"]) = ss.str().c_str();
			((UIMetrics*) audioEffectsSettingEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(audioEffectsSettingEntry.second);
			drawingMaster->drawStack.push_back(audioEffectsSettingEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) audioEffectsSettingEntry.second["metrics"]);

			// fullscreen setting control label
			*((float*) fullscreenSettingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			*((Vector4*) fullscreenSettingEntry.second["fontColor"]) = (activeMenuSelection == &fullscreenSettingEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
			*((std::string*) fullscreenSettingEntry.second["text"]) = (gameSystem->getBool("displayStartFullscreen") ? "Start Fullscreen: Yes" : "Start Fullscreen: No");
			((UIMetrics*) fullscreenSettingEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(fullscreenSettingEntry.second);
			drawingMaster->drawStack.push_back(fullscreenSettingEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) fullscreenSettingEntry.second["metrics"]);

			// windowed screen resolution setting control label
			*((float*) windowedScreenResolutionEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			*((Vector4*) windowedScreenResolutionEntry.second["fontColor"]) = (activeMenuSelection == &windowedScreenResolutionEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
			std::stringstream windowedResolutionSettingText; windowedResolutionSettingText << "Window Resolution: " << gameSystem->getString("displayWindowedResolution");
			*((std::string*) windowedScreenResolutionEntry.second["text"]) = windowedResolutionSettingText.str();
			((UIMetrics*) windowedScreenResolutionEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(windowedScreenResolutionEntry.second);
			drawingMaster->drawStack.push_back(windowedScreenResolutionEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) windowedScreenResolutionEntry.second["metrics"]);

			// framerate limiting control label
			*((float*) framerateLimitingEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			*((Vector4*) framerateLimitingEntry.second["fontColor"]) = (activeMenuSelection == &framerateLimitingEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
			*((std::string*) framerateLimitingEntry.second["text"]) = (
					(int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_VSYNC ? "Framerate Limiting: Vsync" :
					(int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_30 ? "Framerate Limiting: 30 FPS" :
					(int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_60 ? "Framerate Limiting: 60 FPS" :
					(int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_120 ? "Framerate Limiting: 120 FPS" :
					"Framerate Limiting: Off"
				);
			((UIMetrics*) framerateLimitingEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(framerateLimitingEntry.second);
			drawingMaster->drawStack.push_back(framerateLimitingEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) framerateLimitingEntry.second["metrics"]);

			// multisampling level control label
			if(gameGraphics->supportsMultisampling) {
				*((float*) multisamplingLevelEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
				*((Vector4*) multisamplingLevelEntry.second["fontColor"]) = (activeMenuSelection == &multisamplingLevelEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
				*((std::string*) multisamplingLevelEntry.second["text"]) = (
						gameSystem->getFloat("displayMultisamplingLevel") == 2.0f ? "Multisampling Level: Low" :
						gameSystem->getFloat("displayMultisamplingLevel") == 4.0f ? "Multisampling Level: High" :
						"Multisampling Level: Off"
					);
				((UIMetrics*) multisamplingLevelEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(multisamplingLevelEntry.second);
				drawingMaster->drawStack.push_back(multisamplingLevelEntry);
				drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) multisamplingLevelEntry.second["metrics"]);
			}

			// island terrain detail control label
			*((float*) terrainDetailEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			*((Vector4*) terrainDetailEntry.second["fontColor"]) = (activeMenuSelection == &terrainDetailEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
			*((std::string*) terrainDetailEntry.second["text"]) = (
					gameSystem->getFloat("islandTerrainDetail") == 2.0f ? "Terrain Detail: Medium" :
					gameSystem->getFloat("islandTerrainDetail") == 3.0f ? "Terrain Detail: High" :
					"Terrain Detail: Low"
				);
			((UIMetrics*) terrainDetailEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(terrainDetailEntry.second);
			drawingMaster->drawStack.push_back(terrainDetailEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) terrainDetailEntry.second["metrics"]);

			// development mode control label
			*((float*) developmentModeEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeMedium");
			*((Vector4*) developmentModeEntry.second["fontColor"]) = (activeMenuSelection == &developmentModeEntry ? gameSystem->getColor("fontColorLight") : gameSystem->getColor("fontColorDark"));
			*((std::string*) developmentModeEntry.second["text"]) = (gameSystem->getBool("developmentMode") ? "Development Mode: On" : "Development Mode: Off");
			((UIMetrics*) developmentModeEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(developmentModeEntry.second);
			drawingMaster->drawStack.push_back(developmentModeEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) developmentModeEntry.second["metrics"]);

			// high scores reset button
			*((float*) resetHighScoresEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((float*) resetHighScoresEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
			*((float*) resetHighScoresEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) resetHighScoresEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			*((Vector4*) resetHighScoresEntry.second["insideColor"]) = (
					activeMenuSelection == &resetHighScoresEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			Vector2* resetButtonSize = (Vector2*) resetHighScoresEntry.second["size"];
			resetHighScoresEntry.second.erase(resetHighScoresEntry.second.find("size"));
			*resetButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(resetHighScoresEntry.second);
			resetHighScoresEntry.second["size"] = resetButtonSize;
			((UIMetrics*) resetHighScoresEntry.second["metrics"])->size = *resetButtonSize;
			drawingMaster->drawStack.push_back(resetHighScoresEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) resetHighScoresEntry.second["metrics"]);

			// back button
			*((float*) backButtonEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((float*) backButtonEntry.second["padding"]) = gameSystem->getFloat("hudButtonPadding");
			*((float*) backButtonEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
			*((float*) backButtonEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
			*((Vector4*) backButtonEntry.second["insideColor"]) = (
					activeMenuSelection == &backButtonEntry ?
					gameSystem->getColor("hudContainerHighlightColor") :
					gameSystem->getColor("hudContainerInsideColor")
				);
			Vector2* backButtonSize = (Vector2*) backButtonEntry.second["size"];
			backButtonEntry.second.erase(backButtonEntry.second.find("size"));
			*backButtonSize = ((DrawButton*) drawingMaster->drawers["button"])->getSize(backButtonEntry.second);
			backButtonEntry.second["size"] = backButtonSize;
			((UIMetrics*) backButtonEntry.second["metrics"])->size = *backButtonSize;
			drawingMaster->drawStack.push_back(backButtonEntry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) backButtonEntry.second["metrics"]);

			// tips
			*((float*) menuTip1Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			*((std::string*) menuTip1Entry.second["text"]) = "Press ESC to return to the previous menu";
			((UIMetrics*) menuTip1Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip1Entry.second);
			drawingMaster->drawStack.push_back(menuTip1Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip1Entry.second["metrics"]);

			*((float*) menuTip2Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) menuTip2Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip2Entry.second);
			drawingMaster->drawStack.push_back(menuTip2Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip2Entry.second["metrics"]);

			*((float*) menuTip3Entry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
			((UIMetrics*) menuTip3Entry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(menuTip3Entry.second);
			drawingMaster->drawStack.push_back(menuTip3Entry);
			drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) menuTip3Entry.second["metrics"]);

			// make all buttons the same width
			std::vector<DrawStackEntry*> buttonsToSync;
			buttonsToSync.push_back(&resumeButtonEntry);
			buttonsToSync.push_back(&endGameButtonEntry);
			syncButtonWidths(buttonsToSync);

			// re-arrange the UI
			drawingMaster->uiLayoutAuthority->rearrange();

			// set up button listeners with new positions
			levelButtonZoneListener->ll = ((UIMetrics*) levelSettingEntry.second["metrics"])->position - ((UIMetrics*) levelSettingEntry.second["metrics"])->size / 2.0f;
			levelButtonZoneListener->ur = ((UIMetrics*) levelSettingEntry.second["metrics"])->position + ((UIMetrics*) levelSettingEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(levelButtonZoneListener);
			levelButtonClickListener->ll = levelButtonZoneListener->ll;
			levelButtonClickListener->ur = levelButtonZoneListener->ur;
			inputHandler->mouse.addListener(levelButtonClickListener);

			musicButtonZoneListener->ll = ((UIMetrics*) musicSettingEntry.second["metrics"])->position - ((UIMetrics*) musicSettingEntry.second["metrics"])->size / 2.0f;
			musicButtonZoneListener->ur = ((UIMetrics*) musicSettingEntry.second["metrics"])->position + ((UIMetrics*) musicSettingEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(musicButtonZoneListener);
			musicButtonClickListener->ll = musicButtonZoneListener->ll;
			musicButtonClickListener->ur = musicButtonZoneListener->ur;
			inputHandler->mouse.addListener(musicButtonClickListener);

			audioEffectsButtonZoneListener->ll = ((UIMetrics*) audioEffectsSettingEntry.second["metrics"])->position - ((UIMetrics*) audioEffectsSettingEntry.second["metrics"])->size / 2.0f;
			audioEffectsButtonZoneListener->ur = ((UIMetrics*) audioEffectsSettingEntry.second["metrics"])->position + ((UIMetrics*) audioEffectsSettingEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(audioEffectsButtonZoneListener);
			audioEffectsButtonClickListener->ll = audioEffectsButtonZoneListener->ll;
			audioEffectsButtonClickListener->ur = audioEffectsButtonZoneListener->ur;
			inputHandler->mouse.addListener(audioEffectsButtonClickListener);

			fullscreenButtonZoneListener->ll = ((UIMetrics*) fullscreenSettingEntry.second["metrics"])->position - ((UIMetrics*) fullscreenSettingEntry.second["metrics"])->size / 2.0f;
			fullscreenButtonZoneListener->ur = ((UIMetrics*) fullscreenSettingEntry.second["metrics"])->position + ((UIMetrics*) fullscreenSettingEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(fullscreenButtonZoneListener);
			fullscreenButtonClickListener->ll = fullscreenButtonZoneListener->ll;
			fullscreenButtonClickListener->ur = fullscreenButtonZoneListener->ur;
			inputHandler->mouse.addListener(fullscreenButtonClickListener);

			windowedScreenResolutionButtonZoneListener->ll = ((UIMetrics*) windowedScreenResolutionEntry.second["metrics"])->position - ((UIMetrics*) windowedScreenResolutionEntry.second["metrics"])->size / 2.0f;
			windowedScreenResolutionButtonZoneListener->ur = ((UIMetrics*) windowedScreenResolutionEntry.second["metrics"])->position + ((UIMetrics*) windowedScreenResolutionEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(windowedScreenResolutionButtonZoneListener);
			windowedScreenResolutionButtonClickListener->ll = windowedScreenResolutionButtonZoneListener->ll;
			windowedScreenResolutionButtonClickListener->ur = windowedScreenResolutionButtonZoneListener->ur;
			inputHandler->mouse.addListener(windowedScreenResolutionButtonClickListener);

			framerateLimitingButtonZoneListener->ll = ((UIMetrics*) framerateLimitingEntry.second["metrics"])->position - ((UIMetrics*) framerateLimitingEntry.second["metrics"])->size / 2.0f;
			framerateLimitingButtonZoneListener->ur = ((UIMetrics*) framerateLimitingEntry.second["metrics"])->position + ((UIMetrics*) framerateLimitingEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(framerateLimitingButtonZoneListener);
			framerateLimitingButtonClickListener->ll = framerateLimitingButtonZoneListener->ll;
			framerateLimitingButtonClickListener->ur = framerateLimitingButtonZoneListener->ur;
			inputHandler->mouse.addListener(framerateLimitingButtonClickListener);

			if(gameGraphics->supportsMultisampling) {
				multisamplingButtonZoneListener->ll = ((UIMetrics*) multisamplingLevelEntry.second["metrics"])->position - ((UIMetrics*) multisamplingLevelEntry.second["metrics"])->size / 2.0f;
				multisamplingButtonZoneListener->ur = ((UIMetrics*) multisamplingLevelEntry.second["metrics"])->position + ((UIMetrics*) multisamplingLevelEntry.second["metrics"])->size / 2.0f;
				inputHandler->mouse.addListener(multisamplingButtonZoneListener);
				multisamplingButtonClickListener->ll = multisamplingButtonZoneListener->ll;
				multisamplingButtonClickListener->ur = multisamplingButtonZoneListener->ur;
				inputHandler->mouse.addListener(multisamplingButtonClickListener);
			}

			terrainDetailButtonZoneListener->ll = ((UIMetrics*) terrainDetailEntry.second["metrics"])->position - ((UIMetrics*) terrainDetailEntry.second["metrics"])->size / 2.0f;
			terrainDetailButtonZoneListener->ur = ((UIMetrics*) terrainDetailEntry.second["metrics"])->position + ((UIMetrics*) terrainDetailEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(terrainDetailButtonZoneListener);
			terrainDetailButtonClickListener->ll = terrainDetailButtonZoneListener->ll;
			terrainDetailButtonClickListener->ur = terrainDetailButtonZoneListener->ur;
			inputHandler->mouse.addListener(terrainDetailButtonClickListener);

			developmentModeButtonZoneListener->ll = ((UIMetrics*) developmentModeEntry.second["metrics"])->position - ((UIMetrics*) developmentModeEntry.second["metrics"])->size / 2.0f;
			developmentModeButtonZoneListener->ur = ((UIMetrics*) developmentModeEntry.second["metrics"])->position + ((UIMetrics*) developmentModeEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(developmentModeButtonZoneListener);
			developmentModeButtonClickListener->ll = developmentModeButtonZoneListener->ll;
			developmentModeButtonClickListener->ur = developmentModeButtonZoneListener->ur;
			inputHandler->mouse.addListener(developmentModeButtonClickListener);

			backButtonZoneListener->ll = ((UIMetrics*) backButtonEntry.second["metrics"])->position - ((UIMetrics*) backButtonEntry.second["metrics"])->size / 2.0f;
			backButtonZoneListener->ur = ((UIMetrics*) backButtonEntry.second["metrics"])->position + ((UIMetrics*) backButtonEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(backButtonZoneListener);
			backButtonClickListener->ll = backButtonZoneListener->ll;
			backButtonClickListener->ur = backButtonZoneListener->ur;
			inputHandler->mouse.addListener(backButtonClickListener);

			resetHighScoresButtonZoneListener->ll = ((UIMetrics*) resetHighScoresEntry.second["metrics"])->position - ((UIMetrics*) resetHighScoresEntry.second["metrics"])->size / 2.0f;
			resetHighScoresButtonZoneListener->ur = ((UIMetrics*) resetHighScoresEntry.second["metrics"])->position + ((UIMetrics*) resetHighScoresEntry.second["metrics"])->size / 2.0f;
			inputHandler->mouse.addListener(resetHighScoresButtonZoneListener);
			resetHighScoresButtonClickListener->ll = resetHighScoresButtonZoneListener->ll;
			resetHighScoresButtonClickListener->ur = resetHighScoresButtonZoneListener->ur;
			inputHandler->mouse.addListener(resetHighScoresButtonClickListener);

			break;
		}
	}

	// add development statistics to every scheme if enabled
	if(gameSystem->getBool("developmentMode")) {
		*((float*) develStatsTitleEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
		((UIMetrics*) develStatsTitleEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(develStatsTitleEntry.second);

		*((float*) develStatsEntry.second["fontSize"]) = gameSystem->getFloat("fontSizeSmall");
		std::stringstream stringStream;
		stringStream << "Uptime:\t";
		stringStream << (platform->getExecMills() / 3600000);
		stringStream << ":";
		char minuteSecondString[8];
		sprintf(
				minuteSecondString,
				"%.2u:%.2u.%u",
				(platform->getExecMills() % 3600000) / 60000,
				(platform->getExecMills() % 60000) / 1000,
				(platform->getExecMills() % 1000) / 100
			);
		stringStream << minuteSecondString;
		stringStream << "\n";
		stringStream << "Game Time:\t";
		if(gameState == NULL) {
			stringStream << "NULL";
		} else {
			stringStream << (gameState->lastUpdateGameTime / 3600000);
			stringStream << ":";
			sprintf(
					minuteSecondString,
					"%.2u:%.2u.%u",
					(gameState->lastUpdateGameTime % 3600000) / 60000,
					(gameState->lastUpdateGameTime % 60000) / 1000,
					(gameState->lastUpdateGameTime % 1000) / 100
				);
			stringStream << minuteSecondString;
		}
		stringStream << "\n";
		stringStream << "Orbiting Ships:\t";
		unsigned int activeShips = 0;
		if(gameState == NULL) {
			stringStream << "NULL";
		} else {
			for(size_t i = 0; i < gameState->ships.size(); ++i)
				if((float) (gameState->lastUpdateGameTime - gameState->ships[i].originTime) / 1000.0f > gameSystem->getFloat("stateShipEntryTime"))
					++activeShips;
			stringStream << activeShips;
		}
		stringStream << "\n";
		stringStream << "Live Missiles:\t";
		if(gameState == NULL) {
			stringStream << "NULL";
		} else {
			unsigned int liveMissileCount = 0;
			for(size_t i = 0; i < gameState->missiles.size(); ++i)
				if(gameState->missiles[i].alive)
					++liveMissileCount;
			stringStream << liveMissileCount;
		}
		stringStream << "\n";
		stringStream << "Missiles Fired:\t";
		if(gameState == NULL)
			stringStream << "NULL";
		else
			stringStream << gameState->missiles.size();
		stringStream << "\n";
		stringStream << "Firing Interval:\t";
		if(gameState == NULL)
			stringStream << "NULL";
		else
			stringStream << (float) ((unsigned int) ((gameState->getFiringInterval() / 1000.0f / (float) activeShips) * 100.0f)) / 100.0f;
		stringStream << "\n";
		stringStream << "FPS:\t";
		stringStream << drawingMaster->runRate;
		*((std::string*) develStatsEntry.second["text"]) = stringStream.str().c_str();
		((UIMetrics*) develStatsEntry.second["metrics"])->size = ((DrawLabel*) drawingMaster->drawers["label"])->getSize(develStatsEntry.second);

		*((float*) develStatsContainerEntry.second["padding"]) = gameSystem->getFloat("hudContainerPadding");
		*((float*) develStatsContainerEntry.second["border"]) = gameSystem->getFloat("hudContainerBorder");
		*((float*) develStatsContainerEntry.second["softEdge"]) = gameSystem->getFloat("hudContainerSoftEdge");
		*((Vector2*) develStatsContainerEntry.second["size"]) = Vector2(
				(((DrawLabel*) drawingMaster->drawers["label"])->getSize(develStatsTitleEntry.second).x >
						((DrawLabel*) drawingMaster->drawers["label"])->getSize(develStatsEntry.second).x ?
						((DrawLabel*) drawingMaster->drawers["label"])->getSize(develStatsTitleEntry.second).x :
						((DrawLabel*) drawingMaster->drawers["label"])->getSize(develStatsEntry.second).x) +
						gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionX * 4.0f,
				((DrawLabel*) drawingMaster->drawers["label"])->getSize(develStatsTitleEntry.second).y +
						(float) gameGraphics->fontManager->lineHeights[gameSystem->getFloat("fontSizeSmall")] / gameGraphics->resolutionY * 2.0f +
						((DrawLabel*) drawingMaster->drawers["label"])->getSize(develStatsEntry.second).y +
						gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionY * 2.0f // half of container padding in y when rendering text
			);
		((UIMetrics*) develStatsContainerEntry.second["metrics"])->size = *((Vector2*) develStatsContainerEntry.second["size"]);
		drawingMaster->uiLayoutAuthority->metrics.push_back((UIMetrics*) develStatsContainerEntry.second["metrics"]);
		drawingMaster->drawStack.push_back(develStatsContainerEntry);
		drawingMaster->drawStack.push_back(develStatsTitleEntry);
		drawingMaster->drawStack.push_back(develStatsEntry);

		drawingMaster->uiLayoutAuthority->rearrange();

		((UIMetrics*) develStatsTitleEntry.second["metrics"])->position = Vector2(
				((UIMetrics*) develStatsContainerEntry.second["metrics"])->position.x,
				((UIMetrics*) develStatsContainerEntry.second["metrics"])->position.y +
						((UIMetrics*) develStatsContainerEntry.second["metrics"])->size.y / 2.0f -
						gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionY - // half of container padding in y when rendering text
						((UIMetrics*) develStatsTitleEntry.second["metrics"])->size.y / 2.0f
			);

		((UIMetrics*) develStatsEntry.second["metrics"])->position = Vector2(
				((UIMetrics*) develStatsContainerEntry.second["metrics"])->position.x -
						((UIMetrics*) develStatsContainerEntry.second["metrics"])->size.x / 2.0f +
						gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionX * 2.0f +
						((UIMetrics*) develStatsEntry.second["metrics"])->size.x / 2.0f,
				((UIMetrics*) develStatsContainerEntry.second["metrics"])->position.y -
						((UIMetrics*) develStatsContainerEntry.second["metrics"])->size.y / 2.0f +
						gameSystem->getFloat("hudContainerPadding") / (float) gameGraphics->resolutionY + // half of container padding in y when rendering text
						((UIMetrics*) develStatsEntry.second["metrics"])->size.y / 2.0f
			);
	}
}

void GameLogic::startNewGame() {
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
	missileCache.clear();

	currentScheme = SCHEME_INTRO;
	activeMenuSelection = NULL;
	reScheme();

	gameAudio->setBackgroundMusic("playingSong");

	turretUpKeyListener->isDown = false;
	turretDownKeyListener->isDown = false;
	turretLeftKeyListener->isDown = false;
	turretRightKeyListener->isDown = false;
	cameraAheadKeyListener->isDown = false;
}

void GameLogic::bumpStartFromIntro() {
	gameState->bumpStart();
	gameGraphics->currentCamera = &fortressCamera;
	currentScheme = SCHEME_PLAYING;
	reScheme();

	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
	SDL_ShowCursor(0);
	mouseActive = false;

	inputHandler->execute();
	mouseMotionListener->wasMoved();
}

void GameLogic::pauseGame() {
	gameState->pause();
	mainLoopModules.erase(mainLoopModules.find(drawingMaster));

	currentScheme = SCHEME_PAUSED;
	activeMenuSelection = &resumeButtonEntry;
	reScheme();
	drawingMaster->execute(true);

	SDL_WM_GrabInput(SDL_GRAB_OFF);
	SDL_ShowCursor(1);

	gameAudio->playSound("backEffect");
}

void GameLogic::resumeGame() {
	if(gameGraphics->currentCamera == &introCamera) {
		currentScheme = SCHEME_INTRO;
	} else {
		currentScheme = SCHEME_PLAYING;
		SDL_WM_GrabInput(SDL_GRAB_ON);
		SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
		SDL_ShowCursor(0);
		mouseActive = false;

		inputHandler->execute();
		mouseMotionListener->wasMoved();
	}

	activeMenuSelection = NULL;
	reScheme();

	mainLoopModules[drawingMaster] = 0;

	gameState->resume();

	gameAudio->playSound("selectEffect");
}

void GameLogic::endGameFromPause() {
	inputHandler->keyboard.listenUnicode = true;
	inputHandler->keyboard.unicodeChars = "";

	playerName = "";

	currentScheme = SCHEME_GAMEOVER;
	activeMenuSelection = &gameOverContinueButton;
	reScheme();
	drawingMaster->execute(true);

	gameAudio->playSound("selectEffect");

	gameAudio->setBackgroundMusic("menuSong");
}

void GameLogic::continueFromGameOver() {
	if(gameState->score > 0 && (gameSystem->highScores.size() == 0 || gameState->score > gameSystem->highScores.back().first)) {
		if(playerName != "") {
			// trim
			while(playerName.size() > 0 && playerName.substr(playerName.size() - 1, 1) == " ")
				playerName = playerName.substr(0, playerName.size() - 1);
			while(playerName.size() > 0 && playerName.substr(0, 1) == " ")
				playerName = playerName.substr(1);

			// add difficulty level
			playerName += "\t";
			playerName += (gameSystem->getFloat("gameStartingLevel") == 1.0f ? "Easy" : gameSystem->getFloat("gameStartingLevel") == 2.0f ? "Medium" : "Hard");

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
	drawingMaster->execute(true);

	gameAudio->playSound("selectEffect");
}

void GameLogic::alterGameLevel(bool increase) {
	float gameLevel = gameSystem->getFloat("gameStartingLevel") + (increase ? 1.0f : -1.0f);
	if(gameLevel > 3.0f)
		gameLevel = 1.0f;
	else if(gameLevel < 1.0f)
		gameLevel = 3.0f;

	gameSystem->setStandard("gameStartingLevel", gameLevel);
	gameSystem->flushPreferences();

	reScheme();
	drawingMaster->execute(true);

	if(increase)
		gameAudio->playSound("alterUpEffect");
	else
		gameAudio->playSound("alterDownEffect");
}

void GameLogic::alterMusicLevel(bool increase) {
	float value = gameSystem->getFloat("audioMusicVolume");
	value += (increase ? 0.1f : -0.1f);
	if(value > 1.0f) value = 0.0f;
	if(value < 0.0f) value = 1.0f;
	SDL_LockAudio();
	gameSystem->setStandard("audioMusicVolume", value, "");
	SDL_UnlockAudio();
	gameSystem->flushPreferences();

	reScheme();
	drawingMaster->execute(true);

	if(increase)
		gameAudio->playSound("alterUpEffect");
	else
		gameAudio->playSound("alterDownEffect");
}

void GameLogic::alterAudioEffectsLevel(bool increase) {
	float value = gameSystem->getFloat("audioEffectsVolume");
	value += (increase ? 0.1f : -0.1f);
	if(value > 1.0f) value = 0.0f;
	if(value < 0.0f) value = 1.0f;
	gameSystem->setStandard("audioEffectsVolume", value, "");
	gameSystem->flushPreferences();

	reScheme();
	drawingMaster->execute(true);

	if(increase)
		gameAudio->playSound("alterUpEffect");
	else
		gameAudio->playSound("alterDownEffect");
}

void GameLogic::alterFullscreenSetting(bool increase) {
	gameSystem->setStandard("displayStartFullscreen", ! gameSystem->getBool("displayStartFullscreen"));
	gameSystem->flushPreferences();

	reScheme();
	drawingMaster->execute();

	if(increase)
		gameAudio->playSound("alterUpEffect");
	else
		gameAudio->playSound("alterDownEffect");
}

void GameLogic::alterWindowedResolution(bool increase) {
	std::vector< std::pair<unsigned int, unsigned int> > allowedResolutions = gameSystem->getAllowedWindowResolutions();

	size_t i = 0;
	while(i < allowedResolutions.size()) {
		std::stringstream resolutionText;
		resolutionText << allowedResolutions[i].first << "x" << allowedResolutions[i].second;

		if(resolutionText.str() == gameSystem->getString("displayWindowedResolution"))
			break;

		++i;
	}

	if(increase) {
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
		drawingMaster->execute(true);
	}

	if(increase)
		gameAudio->playSound("alterUpEffect");
	else
		gameAudio->playSound("alterDownEffect");
}

void GameLogic::alterFramerateLimiting(bool increase) {
	if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_VSYNC) {
		if(increase)
			gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_30);
		else
			gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_OFF);
	} else if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_30) {
		if(increase)
			gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_60);
		else
			gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_VSYNC);
	} else if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_60) {
		if(increase)
			gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_120);
		else
			gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_30);
	} else if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_120) {
		if(increase)
			gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_OFF);
		else
			gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_60);
	} else {
		if(increase)
			gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_VSYNC);
		else
			gameSystem->setStandard("displayFramerateLimiting", (float) GameSystem::LIMIT_120);
	}

	gameSystem->flushPreferences();

	bool fullScreen = gameGraphics->fullScreen;
	delete gameGraphics;
	gameGraphics = new GameGraphics(fullScreen);
	drawingMaster->newGraphics();
	inputHandler->execute();

	mouseMotionListener->wasMoved();
	reScheme();
	drawingMaster->execute(true);

	if(increase)
		gameAudio->playSound("alterUpEffect");
	else
		gameAudio->playSound("alterDownEffect");
}

void GameLogic::alterMultiSamplingLevel(bool increase) {
	if(gameSystem->getFloat("displayMultisamplingLevel") == 0.0f) {
		if(increase)
			gameSystem->setStandard("displayMultisamplingLevel", 2.0f);
		else
			gameSystem->setStandard("displayMultisamplingLevel", 4.0f);
	} else if(gameSystem->getFloat("displayMultisamplingLevel") == 2.0f) {
		if(increase)
			gameSystem->setStandard("displayMultisamplingLevel", 4.0f);
		else
			gameSystem->setStandard("displayMultisamplingLevel", 0.0f);
	} else {
		if(increase)
			gameSystem->setStandard("displayMultisamplingLevel", 0.0f);
		else
			gameSystem->setStandard("displayMultisamplingLevel", 2.0f);
	}

	gameSystem->flushPreferences();

	bool fullScreen = gameGraphics->fullScreen;
	delete gameGraphics;
	gameGraphics = new GameGraphics(fullScreen);
	drawingMaster->newGraphics();
	inputHandler->execute();
	mouseMotionListener->wasMoved();

	reScheme();
	drawingMaster->execute(true);

	if(increase)
		gameAudio->playSound("alterUpEffect");
	else
		gameAudio->playSound("alterDownEffect");
}

void GameLogic::alterTerrainDetail(bool increase) {
	if(gameSystem->getFloat("islandTerrainDetail") == 1.0f) {
		if(increase)
			gameSystem->setStandard("islandTerrainDetail", 2.0f);
		else
			gameSystem->setStandard("islandTerrainDetail", 3.0f);
	} else if(gameSystem->getFloat("islandTerrainDetail") == 2.0f) {
		if(increase)
			gameSystem->setStandard("islandTerrainDetail", 3.0f);
		else
			gameSystem->setStandard("islandTerrainDetail", 1.0f);
	} else {
		if(increase)
			gameSystem->setStandard("islandTerrainDetail", 1.0f);
		else
			gameSystem->setStandard("islandTerrainDetail", 2.0f);
	}

	gameSystem->flushPreferences();

	reScheme();
	drawingMaster->execute(true);

	if(increase)
		gameAudio->playSound("alterUpEffect");
	else
		gameAudio->playSound("alterDownEffect");
}

void GameLogic::alterDevelopmentMode(bool increase) {
	gameSystem->setStandard("developmentMode", ! gameSystem->getBool("developmentMode"), "");
	gameSystem->flushPreferences();

	reScheme();
	drawingMaster->execute(true);

	if(increase)
		gameAudio->playSound("alterUpEffect");
	else
		gameAudio->playSound("alterDownEffect");
}

void GameLogic::resetHighScores() {
	gameSystem->highScores.clear();
	gameSystem->flushPreferences();
	gameAudio->playSound("selectEffect");
}

void GameLogic::playEffectAtDistance(std::string effect, float distance) {
	float maxDistance = gameSystem->getFloat("audioVolumeDropOffDistance");

	if(distance > maxDistance)
		return;

	gameAudio->playSound(
			effect,
			(maxDistance - distance) / maxDistance
		);
}

GameLogic::GameLogic() :
		MainLoopMember((unsigned int) gameSystem->getFloat("logicUpdateFrequency")),
		currentScheme(SCHEME_MAINMENU),
		activeMenuSelection(&playButtonEntry),
		mouseActive(false),
		playerName(""),
		deleteKeyPressTime(-1),
		lastCharacterDeletionTime(0),
		leftArrowPressTime(0),
		rightArrowPressTime(0),
		upArrowPressTime(0),
		downArrowPressTime(0),
		lastDevelInfoUpdate(0),
		lastGameTimeUpdate(0),
		lastUpdate(0) {
	// initialize draw info/listeners for all schemes
	std::vector<SDLKey> keysVector;
	keysVector.push_back(SDLK_F12);
	quitKeyListener = new KeyListener(keysVector);

	keysVector.clear();
	keysVector.push_back(SDLK_F1);
	fullScreenKeyListener = new KeyListener(keysVector);

	keysVector.clear();
	keysVector.push_back(SDLK_UP);
	keysVector.push_back(SDLK_DOWN);
	keysVector.push_back(SDLK_RETURN);
	mainMenuKeyListener = new KeyListener(keysVector);

	mouseMotionListener = new MouseMotionListener();

	splashEntry.first = "splash";
	splashEntry.second = ((DrawSplash*) drawingMaster->drawers["splash"])->instantiateArgList();

	mainMenuTitleEntry.first = "label";
	mainMenuTitleEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) mainMenuTitleEntry.second["wrap"];
	mainMenuTitleEntry.second.erase(mainMenuTitleEntry.second.find("wrap"));
	*((Vector4*) mainMenuTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) mainMenuTitleEntry.second["text"]) = "CRUCIBLE ISLAND";
	((UIMetrics*) mainMenuTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	logoEntry.first = "texture";
	logoEntry.second = ((DrawTexture*) drawingMaster->drawers["texture"])->instantiateArgList();
	*((std::string*) logoEntry.second["texture"]) = "branding/logo";
	((UIMetrics*) logoEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	spacerMetrics = new UIMetrics;
	spacerMetrics->bearing1 = UIMetrics::BEARING_TOP;

	playButtonEntry.first = "button";
	playButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
	playButtonZoneListener = new MouseZoneListener();
	playButtonClickListener = new MouseButtonListener();
	*((Vector4*) playButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) playButtonEntry.second["text"]) = "Play";
	*((Vector4*) playButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) playButtonEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerBorderColor").x,
			gameSystem->getColor("hudContainerBorderColor").y,
			gameSystem->getColor("hudContainerBorderColor").z,
			0.0f
		);
	((UIMetrics*) playButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	helpButtonEntry.first = "button";
	helpButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
	helpButtonZoneListener = new MouseZoneListener();
	helpButtonClickListener = new MouseButtonListener();
	*((Vector4*) helpButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) helpButtonEntry.second["text"]) = "Help";
	*((Vector4*) helpButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) helpButtonEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerBorderColor").x,
			gameSystem->getColor("hudContainerBorderColor").y,
			gameSystem->getColor("hudContainerBorderColor").z,
			0.0f
		);
	((UIMetrics*) helpButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	highScoresButtonEntry.first = "button";
	highScoresButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
	highScoresButtonZoneListener = new MouseZoneListener();
	highScoresButtonClickListener = new MouseButtonListener();
	*((Vector4*) highScoresButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) highScoresButtonEntry.second["text"]) = "High Scores";
	*((Vector4*) highScoresButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) highScoresButtonEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerBorderColor").x,
			gameSystem->getColor("hudContainerBorderColor").y,
			gameSystem->getColor("hudContainerBorderColor").z,
			0.0f
		);
	((UIMetrics*) highScoresButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	settingsButtonEntry.first = "button";
	settingsButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
	settingsButtonZoneListener = new MouseZoneListener();
	settingsButtonClickListener = new MouseButtonListener();
	*((Vector4*) settingsButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) settingsButtonEntry.second["text"]) = "Settings";
	*((Vector4*) settingsButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) settingsButtonEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerBorderColor").x,
			gameSystem->getColor("hudContainerBorderColor").y,
			gameSystem->getColor("hudContainerBorderColor").z,
			0.0f
		);
	((UIMetrics*) settingsButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	quitButtonEntry.first = "button";
	quitButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
	quitButtonZoneListener = new MouseZoneListener();
	quitButtonClickListener = new MouseButtonListener();
	*((Vector4*) quitButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) quitButtonEntry.second["text"]) = "Quit";
	*((Vector4*) quitButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) quitButtonEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerBorderColor").x,
			gameSystem->getColor("hudContainerBorderColor").y,
			gameSystem->getColor("hudContainerBorderColor").z,
			0.0f
		);
	((UIMetrics*) quitButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	menuTip1Entry.first = "label";
	menuTip1Entry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) menuTip1Entry.second["wrap"];
	menuTip1Entry.second.erase(menuTip1Entry.second.find("wrap"));
		*((Vector4*) menuTip1Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
		((UIMetrics*) menuTip1Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;

	menuTip2Entry.first = "label";
	menuTip2Entry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) menuTip2Entry.second["wrap"];
	menuTip2Entry.second.erase(menuTip2Entry.second.find("wrap"));
	*((Vector4*) menuTip2Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) menuTip2Entry.second["text"]) = "Use ARROW KEYS to navigate or modify";
	((UIMetrics*) menuTip2Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;

	menuTip3Entry.first = "label";
	menuTip3Entry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) menuTip3Entry.second["wrap"];
	menuTip3Entry.second.erase(menuTip3Entry.second.find("wrap"));
		*((Vector4*) menuTip3Entry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
		*((std::string*) menuTip3Entry.second["text"]) = "Press ENTER to select";
		((UIMetrics*) menuTip3Entry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;

	develStatsContainerEntry.first = "container";
	develStatsContainerEntry.second = ((DrawContainer*) drawingMaster->drawers["container"])->instantiateArgList();
	*((Vector4*) develStatsContainerEntry.second["insideColor"]) = gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) develStatsContainerEntry.second["borderColor"]) = gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) develStatsContainerEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerInsideColor").x,
			gameSystem->getColor("hudContainerInsideColor").y,
			gameSystem->getColor("hudContainerInsideColor").z,
			0.0f
		);
	((UIMetrics*) develStatsContainerEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
	((UIMetrics*) develStatsContainerEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_RIGHT;

	develStatsTitleEntry.first = "label";
	develStatsTitleEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) develStatsTitleEntry.second["wrap"];
	develStatsTitleEntry.second.erase(develStatsTitleEntry.second.find("wrap"));
	*((Vector4*) develStatsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) develStatsTitleEntry.second["text"]) = "DEVELOPMENT MODE INFO";

	develStatsEntry.first = "label";
	develStatsEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) develStatsEntry.second["wrap"];
	develStatsEntry.second.erase(develStatsEntry.second.find("wrap"));
	*((Vector4*) develStatsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");

	loadingEntry.first = "label";
	loadingEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) loadingEntry.second["wrap"];
	loadingEntry.second.erase(loadingEntry.second.find("wrap"));
	*((Vector4*) loadingEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) loadingEntry.second["text"]) = "Loading...";

	keysVector.clear();
	keysVector.push_back(SDLK_SPACE);
	keysVector.push_back(SDLK_ESCAPE);
	introKeyListener = new KeyListener(keysVector);

	skyEntry.first = "skyRenderer";

	waterEntry.first = "waterRenderer";

	shipEntry.first = "shipRenderer";

	missileEntry.first = "missileRenderer";

	missileTrailEntry.first = "missileTrailRenderer";

	shellEntry.first = "shellRenderer";

	terrainEntry.first = "terrainRenderer";

	explosionEntry.first = "explosionRenderer";

	fortressEntry.first = "fortressRenderer";

	keysVector.clear();
	keysVector.push_back(SDLK_UP);
	keysVector.push_back(SDLK_DOWN);
	keysVector.push_back(SDLK_RETURN);
	keysVector.push_back(SDLK_ESCAPE);
	pausedMenuKeyListener = new KeyListener(keysVector);

	grayOutEntry.first = "grayOut";
	grayOutEntry.second = ((DrawGrayOut*) drawingMaster->drawers["grayOut"])->instantiateArgList();
	*((Vector4*) grayOutEntry.second["color"]) = gameSystem->getColor("hudGrayOutColor");

	menuTitleEntry.first = "label";
	menuTitleEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) menuTitleEntry.second["wrap"];
	menuTitleEntry.second.erase(menuTitleEntry.second.find("wrap"));
		*((Vector4*) menuTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
		((UIMetrics*) menuTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	controlsTitleEntry.first = "label";
	controlsTitleEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) controlsTitleEntry.second["wrap"];
	controlsTitleEntry.second.erase(controlsTitleEntry.second.find("wrap"));
	*((Vector4*) controlsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) controlsTitleEntry.second["text"]) = "Controls";
	((UIMetrics*) controlsTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	controlsEntry.first = "label";
	controlsEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) controlsEntry.second["wrap"];
	controlsEntry.second.erase(controlsEntry.second.find("wrap"));
	*((Vector4*) controlsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) controlsEntry.second["text"]) = gameSystem->getString("textControls");
	((UIMetrics*) controlsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	resumeButtonEntry.first = "button";
	resumeButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
	resumeButtonZoneListener = new MouseZoneListener();
	resumeButtonClickListener = new MouseButtonListener();
	*((Vector4*) resumeButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) resumeButtonEntry.second["text"]) = "Resume";
	*((Vector4*) resumeButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) resumeButtonEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerBorderColor").x,
			gameSystem->getColor("hudContainerBorderColor").y,
			gameSystem->getColor("hudContainerBorderColor").z,
			0.0f
		);
	((UIMetrics*) resumeButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	endGameButtonEntry.first = "button";
	endGameButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
	endGameButtonZoneListener = new MouseZoneListener();
	endGameButtonClickListener = new MouseButtonListener();
	*((Vector4*) endGameButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) endGameButtonEntry.second["text"]) = "End Game";
	*((Vector4*) endGameButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) endGameButtonEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerBorderColor").x,
			gameSystem->getColor("hudContainerBorderColor").y,
			gameSystem->getColor("hudContainerBorderColor").z,
			0.0f
		);
	((UIMetrics*) endGameButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	keysVector.clear();
	keysVector.push_back(SDLK_ESCAPE);
	keysVector.push_back(SDLK_SPACE);
	keysVector.push_back(SDLK_TAB);
	keysVector.push_back(SDLK_LSHIFT);
	keysVector.push_back(SDLK_RSHIFT);
	playingKeyListener = new KeyListener(keysVector);

	keysVector.push_back(SDLK_RETURN);
	keysVector.push_back(SDLK_BACKQUOTE);
	keysVector.push_back(SDLK_BACKSLASH);
	playingDevelopmentModeKeyListener = new KeyListener(keysVector);

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

	strikeEffectEntry.first = "strikeEffect";
	strikeEffectEntry.second = ((DrawStrikeEffect*) drawingMaster->drawers["strikeEffect"])->instantiateArgList();

	missileIndicators.first = "missileIndicators";
	missileIndicators.second = ((DrawMissileIndicators*) drawingMaster->drawers["missileIndicators"])->instantiateArgList();
	*((Vector4*) missileIndicators.second["color"]) = gameSystem->getColor("hudMissileIndicatorColor");
	*((Vector4*) missileIndicators.second["arrowColor"]) = gameSystem->getColor("hudMissileArrowColor");

	scoreLabel.first = "label";
	scoreLabel.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) scoreLabel.second["wrap"];
	menuTitleEntry.second.erase(scoreLabel.second.find("wrap"));
	*((Vector4*) scoreLabel.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	((UIMetrics*) scoreLabel.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

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
	*((Vector4*) gaugePanelEntry.second["insideColor"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) gaugePanelEntry.second["borderColor"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) gaugePanelEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudGaugeBackgroundColor").x,
			gameSystem->getColor("hudGaugeBackgroundColor").y,
			gameSystem->getColor("hudGaugeBackgroundColor").z,
			0.0f
		);
	*((size_t*) gaugePanelEntry.second["elements"]) = 3;
	(*((std::vector<std::string>*) gaugePanelEntry.second["textureNames"]))[0] = "gauge/heart";
	(*((std::vector<std::string>*) gaugePanelEntry.second["textureNames"]))[1] = "gauge/shell";
	(*((std::vector<std::string>*) gaugePanelEntry.second["textureNames"]))[2] = "gauge/bolt";
	*((Vector4*) gaugePanelEntry.second["backgroundColorTop"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) gaugePanelEntry.second["backgroundColorBottom"]) = Vector4(
			gameSystem->getColor("hudGaugeBackgroundColor").x * gameSystem->getColor("hudGaugeColorFalloff").x,
			gameSystem->getColor("hudGaugeBackgroundColor").y * gameSystem->getColor("hudGaugeColorFalloff").y,
			gameSystem->getColor("hudGaugeBackgroundColor").z * gameSystem->getColor("hudGaugeColorFalloff").z,
			gameSystem->getColor("hudGaugeBackgroundColor").w * gameSystem->getColor("hudGaugeColorFalloff").w
		);
	((UIMetrics*) gaugePanelEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_LEFT;
	((UIMetrics*) gaugePanelEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;

	radarEntry.first = "radar";
	radarEntry.second = ((DrawRadar*) drawingMaster->drawers["radar"])->instantiateArgList();
	*((Vector4*) radarEntry.second["insideColor"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) radarEntry.second["borderColor"]) = gameSystem->getColor("hudGaugeBackgroundColor");
	*((Vector4*) radarEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudGaugeBackgroundColor").x,
			gameSystem->getColor("hudGaugeBackgroundColor").y,
			gameSystem->getColor("hudGaugeBackgroundColor").z,
			0.0f
		);
	((UIMetrics*) radarEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_RIGHT;
	((UIMetrics*) radarEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_TOP;

	develControlsContainerEntry.first = "container";
	develControlsContainerEntry.second = ((DrawContainer*) drawingMaster->drawers["container"])->instantiateArgList();
	*((Vector4*) develControlsContainerEntry.second["insideColor"]) = gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) develControlsContainerEntry.second["borderColor"]) = gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) develControlsContainerEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerInsideColor").x,
			gameSystem->getColor("hudContainerInsideColor").y,
			gameSystem->getColor("hudContainerInsideColor").z,
			0.0f
		);
		((UIMetrics*) develControlsContainerEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_BOTTOM;
		((UIMetrics*) develControlsContainerEntry.second["metrics"])->bearing2 = UIMetrics::BEARING_LEFT;

	develControlsTitleEntry.first = "label";
	develControlsTitleEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) develControlsTitleEntry.second["wrap"];
	develControlsTitleEntry.second.erase(develControlsTitleEntry.second.find("wrap"));
	*((Vector4*) develControlsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) develControlsTitleEntry.second["text"]) = "DEVELOPMENT MODE CONTROLS";

	develControlsEntry.first = "label";
	develControlsEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) develControlsEntry.second["wrap"];
	develControlsEntry.second.erase(develControlsEntry.second.find("wrap"));
		*((Vector4*) develControlsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
		*((std::string*) develControlsEntry.second["text"]) = "Freeze/Unfreeze:\t`\nReset Game:\tReturn\nChange View:\t\\\nTilt/Rotate Camera:\tArrow Keys\nAdvance Camera:\tSpace";

	controlSpotEntry.first = "circle";
	controlSpotEntry.second = ((DrawCircle*) drawingMaster->drawers["circle"])->instantiateArgList();
	*((Vector4*) controlSpotEntry.second["borderColor"]) = gameSystem->getColor("hudControlSpotColor");
	*((Vector4*) controlSpotEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudControlSpotColor").x,
			gameSystem->getColor("hudControlSpotColor").y,
			gameSystem->getColor("hudControlSpotColor").z,
			0.0f
		);

	cursorEntry.first = "circle";
	cursorEntry.second = ((DrawCircle*) drawingMaster->drawers["circle"])->instantiateArgList();
	*((Vector4*) cursorEntry.second["insideColor"]) = gameSystem->getColor("hudCursorColor");
	*((Vector4*) cursorEntry.second["borderColor"]) = Vector4(
			gameSystem->getColor("hudCursorColor").x,
			gameSystem->getColor("hudCursorColor").y,
			gameSystem->getColor("hudCursorColor").z,
			0.0f
		);
	*((Vector4*) cursorEntry.second["outsideColor"]) = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	keysVector.clear();
	keysVector.push_back(SDLK_UP);
	keysVector.push_back(SDLK_DOWN);
	keysVector.push_back(SDLK_RETURN);
	keysVector.push_back(SDLK_ESCAPE);
	gameOverKeyListener = new KeyListener(keysVector);

	deleteKeyListener = new KeyAbsoluteListener(SDLK_BACKSPACE);

	yourScoreTitleEntry.first = "label";
	yourScoreTitleEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) yourScoreTitleEntry.second["wrap"];
	yourScoreTitleEntry.second.erase(yourScoreTitleEntry.second.find("wrap"));
	*((Vector4*) yourScoreTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) yourScoreTitleEntry.second["text"]) = "Score";
	((UIMetrics*) yourScoreTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	yourScoreEntry.first = "label";
	yourScoreEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) yourScoreEntry.second["wrap"];
	yourScoreEntry.second.erase(yourScoreEntry.second.find("wrap"));
	*((Vector4*) yourScoreEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	((UIMetrics*) yourScoreEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	highScoresTitleEntry.first = "label";
	highScoresTitleEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) highScoresTitleEntry.second["wrap"];
	highScoresTitleEntry.second.erase(highScoresTitleEntry.second.find("wrap"));
	*((Vector4*) highScoresTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) highScoresTitleEntry.second["text"]) = "High Scores";
	((UIMetrics*) highScoresTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	highScoresEntry.first = "label";
	highScoresEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) highScoresEntry.second["wrap"];
	highScoresEntry.second.erase(highScoresEntry.second.find("wrap"));
	*((Vector4*) highScoresEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	((UIMetrics*) highScoresEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	newHighScoreTitleEntry.first = "label";
	newHighScoreTitleEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) newHighScoreTitleEntry.second["wrap"];
	newHighScoreTitleEntry.second.erase(newHighScoreTitleEntry.second.find("wrap"));
	*((Vector4*) newHighScoreTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) newHighScoreTitleEntry.second["text"]) = "Enter New High Score";
	((UIMetrics*) newHighScoreTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	newHighScoreContainer.first = "container";
	newHighScoreContainer.second = ((DrawContainer*) drawingMaster->drawers["container"])->instantiateArgList();
	*((Vector4*) newHighScoreContainer.second["insideColor"]) = gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) newHighScoreContainer.second["borderColor"]) = gameSystem->getColor("hudContainerInsideColor");
	*((Vector4*) newHighScoreContainer.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerBorderColor").x,
			gameSystem->getColor("hudContainerBorderColor").y,
			gameSystem->getColor("hudContainerBorderColor").z,
			0.0f
		);
	((UIMetrics*) newHighScoreContainer.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	newHighScoreNameLabel.first = "label";
	newHighScoreNameLabel.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) newHighScoreNameLabel.second["wrap"];
	newHighScoreNameLabel.second.erase(newHighScoreNameLabel.second.find("wrap"));
	*((Vector4*) newHighScoreNameLabel.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) newHighScoreNameLabel.second["text"]) = "Name:";

	newHighScoreNameField.first = "field";
	newHighScoreNameField.second = ((DrawField*) drawingMaster->drawers["field"])->instantiateArgList();
	*((Vector4*) newHighScoreNameField.second["fontColor"]) = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	*((Vector4*) newHighScoreNameField.second["boxColor"]) = gameSystem->getColor("hudFieldColor");

	gameOverContinueButton.first = "button";
	gameOverContinueButton.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
	delete (Vector2*) gameOverContinueButton.second["size"];
	gameOverContinueButton.second.erase(gameOverContinueButton.second.find("size"));
	gameOverContinueButtonZoneListener = new MouseZoneListener();
	gameOverContinueButtonClickListener = new MouseButtonListener();
	*((Vector4*) gameOverContinueButton.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) gameOverContinueButton.second["text"]) = "Continue";
	*((Vector4*) gameOverContinueButton.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) gameOverContinueButton.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerBorderColor").x,
			gameSystem->getColor("hudContainerBorderColor").y,
			gameSystem->getColor("hudContainerBorderColor").z,
			0.0f
		);
	((UIMetrics*) gameOverContinueButton.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	keysVector.clear();
	keysVector.push_back(SDLK_UP);
	keysVector.push_back(SDLK_DOWN);
	keysVector.push_back(SDLK_ESCAPE);
	keysVector.push_back(SDLK_RETURN);
	helpMenuKeyListener = new KeyListener(keysVector);

	instructionsTitleEntry.first = "label";
	instructionsTitleEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) instructionsTitleEntry.second["wrap"];
	instructionsTitleEntry.second.erase(instructionsTitleEntry.second.find("wrap"));
	*((Vector4*) instructionsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) instructionsTitleEntry.second["text"]) = "Instructions";
	((UIMetrics*) instructionsTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	instructionsEntry.first = "label";
	instructionsEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	*((Vector4*) instructionsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) instructionsEntry.second["text"]) = gameSystem->getString("textInstructions");
	((UIMetrics*) instructionsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	aboutButtonEntry.first = "button";
	aboutButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
	aboutButtonZoneListener = new MouseZoneListener();
	aboutButtonClickListener = new MouseButtonListener();
	*((Vector4*) aboutButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) aboutButtonEntry.second["text"]) = "About";
	*((Vector4*) aboutButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) aboutButtonEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerBorderColor").x,
			gameSystem->getColor("hudContainerBorderColor").y,
			gameSystem->getColor("hudContainerBorderColor").z,
			0.0f
		);
	((UIMetrics*) aboutButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	backButtonEntry.first = "button";
	backButtonEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
	backButtonZoneListener = new MouseZoneListener();
	backButtonClickListener = new MouseButtonListener();
	*((Vector4*) backButtonEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) backButtonEntry.second["text"]) = "Back";
	*((Vector4*) backButtonEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) backButtonEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerBorderColor").x,
			gameSystem->getColor("hudContainerBorderColor").y,
			gameSystem->getColor("hudContainerBorderColor").z,
			0.0f
		);
	((UIMetrics*) backButtonEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	keysVector.clear();
	keysVector.push_back(SDLK_UP);
	keysVector.push_back(SDLK_DOWN);
	keysVector.push_back(SDLK_ESCAPE);
	keysVector.push_back(SDLK_RETURN);
	aboutMenuKeyListener = new KeyListener(keysVector);

	versionTitleEntry.first = "label";
	versionTitleEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) versionTitleEntry.second["wrap"];
	versionTitleEntry.second.erase(versionTitleEntry.second.find("wrap"));
	*((Vector4*) versionTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) versionTitleEntry.second["text"]) = "Version Information";
	((UIMetrics*) versionTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	versionEntry.first = "label";
	versionEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	*((Vector4*) versionEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	((UIMetrics*) versionEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	creditsTitleEntry.first = "label";
	creditsTitleEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) creditsTitleEntry.second["wrap"];
	creditsTitleEntry.second.erase(creditsTitleEntry.second.find("wrap"));
	*((Vector4*) creditsTitleEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) creditsTitleEntry.second["text"]) = "Credits";
	((UIMetrics*) creditsTitleEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	creditsEntry.first = "label";
	creditsEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	*((Vector4*) creditsEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) creditsEntry.second["text"]) = gameSystem->getString("textCredits");
	((UIMetrics*) creditsEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	keysVector.clear();
	keysVector.push_back(SDLK_UP);
	keysVector.push_back(SDLK_DOWN);
	keysVector.push_back(SDLK_ESCAPE);
	keysVector.push_back(SDLK_RETURN);
	highScoresMenuKeyListener = new KeyListener(keysVector);

	keysVector.clear();
	keysVector.push_back(SDLK_UP);
	keysVector.push_back(SDLK_DOWN);
	keysVector.push_back(SDLK_RIGHT);
	keysVector.push_back(SDLK_LEFT);
	keysVector.push_back(SDLK_ESCAPE);
	keysVector.push_back(SDLK_RETURN);
	settingsMenuKeyListener = new KeyListener(keysVector);

	levelSettingEntry.first = "label";
	levelSettingEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) levelSettingEntry.second["wrap"];
	levelSettingEntry.second.erase(levelSettingEntry.second.find("wrap"));
	levelButtonZoneListener = new MouseZoneListener();
	levelButtonClickListener = new MouseButtonListener();
	((UIMetrics*) levelSettingEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	musicSettingEntry.first = "label";
	musicSettingEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) musicSettingEntry.second["wrap"];
	musicSettingEntry.second.erase(musicSettingEntry.second.find("wrap"));
	musicButtonZoneListener = new MouseZoneListener();
	musicButtonClickListener = new MouseButtonListener();
	((UIMetrics*) musicSettingEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	audioEffectsSettingEntry.first = "label";
	audioEffectsSettingEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) audioEffectsSettingEntry.second["wrap"];
	audioEffectsSettingEntry.second.erase(audioEffectsSettingEntry.second.find("wrap"));
	audioEffectsButtonZoneListener = new MouseZoneListener();
	audioEffectsButtonClickListener = new MouseButtonListener();
	((UIMetrics*) audioEffectsSettingEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	fullscreenSettingEntry.first = "label";
	fullscreenSettingEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) fullscreenSettingEntry.second["wrap"];
	fullscreenSettingEntry.second.erase(fullscreenSettingEntry.second.find("wrap"));
	fullscreenButtonZoneListener = new MouseZoneListener();
	fullscreenButtonClickListener = new MouseButtonListener();
	((UIMetrics*) fullscreenSettingEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	windowedScreenResolutionEntry.first = "label";
	windowedScreenResolutionEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) windowedScreenResolutionEntry.second["wrap"];
	windowedScreenResolutionEntry.second.erase(windowedScreenResolutionEntry.second.find("wrap"));
	windowedScreenResolutionButtonZoneListener = new MouseZoneListener();
	windowedScreenResolutionButtonClickListener = new MouseButtonListener();
	((UIMetrics*) windowedScreenResolutionEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	framerateLimitingEntry.first = "label";
	framerateLimitingEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) framerateLimitingEntry.second["wrap"];
	framerateLimitingEntry.second.erase(framerateLimitingEntry.second.find("wrap"));
	framerateLimitingButtonZoneListener = new MouseZoneListener();
	framerateLimitingButtonClickListener = new MouseButtonListener();
	((UIMetrics*) framerateLimitingEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	multisamplingLevelEntry.first = "label";
	multisamplingLevelEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) multisamplingLevelEntry.second["wrap"];
	multisamplingLevelEntry.second.erase(multisamplingLevelEntry.second.find("wrap"));
	multisamplingButtonZoneListener = new MouseZoneListener();
	multisamplingButtonClickListener = new MouseButtonListener();
	((UIMetrics*) multisamplingLevelEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	terrainDetailEntry.first = "label";
	terrainDetailEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) terrainDetailEntry.second["wrap"];
	terrainDetailEntry.second.erase(terrainDetailEntry.second.find("wrap"));
	terrainDetailButtonZoneListener = new MouseZoneListener();
	terrainDetailButtonClickListener = new MouseButtonListener();
	((UIMetrics*) terrainDetailEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	developmentModeEntry.first = "label";
	developmentModeEntry.second = ((DrawLabel*) drawingMaster->drawers["label"])->instantiateArgList();
	delete (float*) developmentModeEntry.second["wrap"];
	developmentModeEntry.second.erase(developmentModeEntry.second.find("wrap"));
	developmentModeButtonZoneListener = new MouseZoneListener();
	developmentModeButtonClickListener = new MouseButtonListener();
	((UIMetrics*) developmentModeEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	resetHighScoresEntry.first = "button";
	resetHighScoresEntry.second = ((DrawButton*) drawingMaster->drawers["button"])->instantiateArgList();
	resetHighScoresButtonZoneListener = new MouseZoneListener();
	resetHighScoresButtonClickListener = new MouseButtonListener();
	*((Vector4*) resetHighScoresEntry.second["fontColor"]) = gameSystem->getColor("fontColorLight");
	*((std::string*) resetHighScoresEntry.second["text"]) = "Reset High Scores";
	*((Vector4*) resetHighScoresEntry.second["borderColor"]) = gameSystem->getColor("hudContainerBorderColor");
	*((Vector4*) resetHighScoresEntry.second["outsideColor"]) = Vector4(
			gameSystem->getColor("hudContainerBorderColor").x,
			gameSystem->getColor("hudContainerBorderColor").y,
			gameSystem->getColor("hudContainerBorderColor").z,
			0.0f
		);
	((UIMetrics*) resetHighScoresEntry.second["metrics"])->bearing1 = UIMetrics::BEARING_TOP;

	// clear the motion listener
	inputHandler->execute();
	mouseMotionListener->wasMoved();

	// draw the initial frame
	reScheme();
	drawingMaster->execute(true);

	// start audio
	gameAudio->setBackgroundMusic("menuSong");
}

GameLogic::~GameLogic() {
	// deallocate draw info/listeners
	delete quitKeyListener;
	delete fullScreenKeyListener;
	delete mainMenuKeyListener;
	delete mouseMotionListener;
	((DrawSplash*) drawingMaster->drawers["splash"])->deleteArgList(splashEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(mainMenuTitleEntry.second);
	((DrawTexture*) drawingMaster->drawers["texture"])->deleteArgList(logoEntry.second);
	delete spacerMetrics;
	((DrawButton*) drawingMaster->drawers["button"])->deleteArgList(playButtonEntry.second);
	delete playButtonZoneListener;
	delete playButtonClickListener;
	((DrawButton*) drawingMaster->drawers["button"])->deleteArgList(helpButtonEntry.second);
	delete helpButtonZoneListener;
	delete helpButtonClickListener;
	((DrawButton*) drawingMaster->drawers["button"])->deleteArgList(highScoresButtonEntry.second);
	delete highScoresButtonZoneListener;
	delete highScoresButtonClickListener;
	((DrawButton*) drawingMaster->drawers["button"])->deleteArgList(settingsButtonEntry.second);
	delete settingsButtonZoneListener;
	delete settingsButtonClickListener;
	((DrawButton*) drawingMaster->drawers["button"])->deleteArgList(quitButtonEntry.second);
	delete quitButtonZoneListener;
	delete quitButtonClickListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(menuTip1Entry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(menuTip2Entry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(menuTip3Entry.second);
	((DrawContainer*) drawingMaster->drawers["container"])->deleteArgList(develStatsContainerEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(develStatsTitleEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(develStatsEntry.second);

	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(loadingEntry.second);

	delete introKeyListener;

	delete pausedMenuKeyListener;
	((DrawGrayOut*) drawingMaster->drawers["grayOut"])->deleteArgList(grayOutEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(menuTitleEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(controlsTitleEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(controlsEntry.second);
	((DrawButton*) drawingMaster->drawers["button"])->deleteArgList(resumeButtonEntry.second);
	delete resumeButtonZoneListener;
	delete resumeButtonClickListener;
	((DrawButton*) drawingMaster->drawers["button"])->deleteArgList(endGameButtonEntry.second);
	delete endGameButtonZoneListener;
	delete endGameButtonClickListener;

	delete playingKeyListener;
	delete playingDevelopmentModeKeyListener;
	delete turretUpKeyListener;
	delete turretDownKeyListener;
	delete turretLeftKeyListener;
	delete turretRightKeyListener;
	delete cameraAheadKeyListener;
	delete primaryFireClickListener1;
	delete primaryFireClickListener2;
	delete primaryFireClickListener3;
	delete secondaryFireClickListener;
	delete binocularsClickListener;
	((DrawStrikeEffect*) drawingMaster->drawers["strikeEffect"])->deleteArgList(strikeEffectEntry.second);
	((DrawMissileIndicators*) drawingMaster->drawers["missileIndicators"])->deleteArgList(missileIndicators.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(scoreLabel.second);
	((DrawGaugePanel*) drawingMaster->drawers["gaugePanel"])->deleteArgList(gaugePanelEntry.second);
	((DrawRadar*) drawingMaster->drawers["radar"])->deleteArgList(radarEntry.second);
	((DrawContainer*) drawingMaster->drawers["container"])->deleteArgList(develControlsContainerEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(develControlsTitleEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(develControlsEntry.second);
	((DrawCircle*) drawingMaster->drawers["circle"])->deleteArgList(controlSpotEntry.second);
	((DrawCircle*) drawingMaster->drawers["circle"])->deleteArgList(cursorEntry.second);

	delete gameOverKeyListener;
	delete deleteKeyListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(yourScoreTitleEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(yourScoreEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(highScoresTitleEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(highScoresEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(newHighScoreTitleEntry.second);
	((DrawContainer*) drawingMaster->drawers["container"])->deleteArgList(newHighScoreContainer.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(newHighScoreNameLabel.second);
	((DrawField*) drawingMaster->drawers["label"])->deleteArgList(newHighScoreNameField.second);
	((DrawButton*) drawingMaster->drawers["button"])->deleteArgList(gameOverContinueButton.second);
	delete gameOverContinueButtonZoneListener;
	delete gameOverContinueButtonClickListener;

	delete helpMenuKeyListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(instructionsTitleEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(instructionsEntry.second);
	((DrawButton*) drawingMaster->drawers["button"])->deleteArgList(aboutButtonEntry.second);
	delete aboutButtonZoneListener;
	delete aboutButtonClickListener;
	((DrawButton*) drawingMaster->drawers["button"])->deleteArgList(backButtonEntry.second);
	delete backButtonZoneListener;
	delete backButtonClickListener;

	delete aboutMenuKeyListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(versionTitleEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(versionEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(creditsTitleEntry.second);
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(creditsEntry.second);

	delete highScoresMenuKeyListener;

	delete settingsMenuKeyListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(levelSettingEntry.second);
	delete levelButtonZoneListener;
	delete levelButtonClickListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(musicSettingEntry.second);
	delete musicButtonZoneListener;
	delete musicButtonClickListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(audioEffectsSettingEntry.second);
	delete audioEffectsButtonZoneListener;
	delete audioEffectsButtonClickListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(fullscreenSettingEntry.second);
	delete fullscreenButtonZoneListener;
	delete fullscreenButtonClickListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(windowedScreenResolutionEntry.second);
	delete windowedScreenResolutionButtonZoneListener;
	delete windowedScreenResolutionButtonClickListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(framerateLimitingEntry.second);
	delete framerateLimitingButtonZoneListener;
	delete framerateLimitingButtonClickListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(multisamplingLevelEntry.second);
	delete multisamplingButtonZoneListener;
	delete multisamplingButtonClickListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(terrainDetailEntry.second);
	delete terrainDetailButtonZoneListener;
	delete terrainDetailButtonClickListener;
	((DrawLabel*) drawingMaster->drawers["label"])->deleteArgList(developmentModeEntry.second);
	delete developmentModeButtonZoneListener;
	delete developmentModeButtonClickListener;
	((DrawButton*) drawingMaster->drawers["button"])->deleteArgList(resetHighScoresEntry.second);
	delete resetHighScoresButtonZoneListener;
	delete resetHighScoresButtonClickListener;
}

unsigned int GameLogic::execute(bool unScheduled) {
	// get a delta time for schemes that need it
	float deltaTime = 0.0f;
	if(gameState != NULL && ! gameState->isPaused && currentScheme == SCHEME_PLAYING) {
		unsigned int currentGameTime = gameState->lastUpdateGameTime;
		deltaTime = (float) (currentGameTime - lastUpdate) / 1000.0f;
		lastUpdate = currentGameTime;
	}

	// rescheme/redraw once per frame only (except when we need to draw immediately)
	bool needReScheme = false;
	bool needRedraw = false;

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

		needReScheme = true;
		needRedraw = true;
	}

	if(
			gameSystem->getBool("developmentMode") &&
			(lastDevelInfoUpdate / 100 != platform->getExecMills() / 100 ||
					(gameState != NULL && lastGameTimeUpdate / 100 != gameState->lastUpdateGameTime / 100) ||
					lastDevelInfoUpdate + 1000 < platform->getExecMills())
		) {
		needReScheme = true;
		needRedraw = true;

		lastDevelInfoUpdate = platform->getExecMills();
		if(gameState != NULL) lastGameTimeUpdate = gameState->lastUpdateGameTime;
	}

	// scheme-specific logic
	if(currentScheme == SCHEME_MAINMENU) {
		// button highlight
		if(mouseMotionListener->wasMoved()) {
			if(playButtonZoneListener->isEntered) {
				if(activeMenuSelection != &playButtonEntry) {
					activeMenuSelection = &playButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(settingsButtonZoneListener->isEntered) {
				if(activeMenuSelection != &settingsButtonEntry) {
					activeMenuSelection = &settingsButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(helpButtonZoneListener->isEntered) {
				if(activeMenuSelection != &helpButtonEntry) {
					activeMenuSelection = &helpButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(highScoresButtonZoneListener->isEntered) {
				if(activeMenuSelection != &highScoresButtonEntry) {
					activeMenuSelection = &highScoresButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(quitButtonZoneListener->isEntered) {
				if(activeMenuSelection != &quitButtonEntry) {
					activeMenuSelection = &quitButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				needReScheme = true;
				needRedraw = true;
			}
		}

		// button clicks
		if(playButtonClickListener->wasClicked()) {
			startNewGame();
		} else if(settingsButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_SETTINGS;
			activeMenuSelection = &levelSettingEntry;
			needReScheme = true;
			needRedraw = true;
			gameAudio->playSound("selectEffect");
		} else if(helpButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_HELP;
			activeMenuSelection = &backButtonEntry;
			needReScheme = true;
			needRedraw = true;
			gameAudio->playSound("selectEffect");
		} else if(highScoresButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_HIGHSCORES;
			activeMenuSelection = &backButtonEntry;
			needReScheme = true;
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
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &helpButtonEntry) {
					activeMenuSelection = &playButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &highScoresButtonEntry) {
					activeMenuSelection = &helpButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &settingsButtonEntry) {
					activeMenuSelection = &highScoresButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &quitButtonEntry) {
					activeMenuSelection = &settingsButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				}
			} else if(key == SDLK_DOWN) {
				if(activeMenuSelection == NULL || activeMenuSelection == &quitButtonEntry) {
					activeMenuSelection = &playButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &playButtonEntry) {
					activeMenuSelection = &helpButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &helpButtonEntry) {
					activeMenuSelection = &highScoresButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &highScoresButtonEntry) {
					activeMenuSelection = &settingsButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &settingsButtonEntry) {
					activeMenuSelection = &quitButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(key == SDLK_RETURN) {
				if(activeMenuSelection == &playButtonEntry) {
					startNewGame();
				} else if(activeMenuSelection == &settingsButtonEntry) {
					currentScheme = SCHEME_SETTINGS;
					activeMenuSelection = &levelSettingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("selectEffect");
				} else if(activeMenuSelection == &helpButtonEntry) {
					currentScheme = SCHEME_HELP;
					activeMenuSelection = &backButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("selectEffect");
				} else if(activeMenuSelection == &highScoresButtonEntry) {
					currentScheme = SCHEME_HIGHSCORES;
					activeMenuSelection = &backButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("selectEffect");
				} else if(activeMenuSelection == &quitButtonEntry) {
					keepProgramAlive = false;
				}
			}
		}
	} else if(currentScheme == SCHEME_INTRO) {
		// button clicks
		if(primaryFireClickListener1->wasClicked()) {
			bumpStartFromIntro();
		}

		// key hits
		for(SDLKey key = introKeyListener->popKey(); key != SDLK_UNKNOWN; key = introKeyListener->popKey()) {
			if(key == SDLK_SPACE) {
				bumpStartFromIntro();
			} else if(key == SDLK_ESCAPE) {
				pauseGame();
			}
		}

		// expiration of intro time
		if((float) gameState->lastUpdateGameTime / 1000.0f > gameSystem->getFloat("stateShipEntryTime")) {
			gameGraphics->currentCamera = &fortressCamera;
			currentScheme = SCHEME_PLAYING;
			needReScheme = true;
			SDL_WarpMouse(gameGraphics->resolutionX / 2, gameGraphics->resolutionY / 2);
			inputHandler->execute();
			mouseMotionListener->wasMoved();
			SDL_WM_GrabInput(SDL_GRAB_ON);
			SDL_ShowCursor(0);
			mouseActive = false;
		}
	} else if(currentScheme == SCHEME_PLAYING) {
		// check score
		if(atoi(((std::string*) scoreLabel.second["text"])->c_str()) != gameState->score)
			needReScheme = true;

		// check gauges
		if(
				(*((std::vector<float>*) gaugePanelEntry.second["progressions"]))[0] != gameState->fortress.health ||
				(*((std::vector<float>*) gaugePanelEntry.second["progressions"]))[1] != gameState->fortress.ammunition ||
				(*((std::vector<float>*) gaugePanelEntry.second["progressions"]))[2] != gameState->fortress.emp
			) {
			needReScheme = true;
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
				needReScheme = true;
			}

			// constrain to control radius
			Vector2 correctAspectMousePosition(inputHandler->mouse.position.x * gameGraphics->aspectRatio, inputHandler->mouse.position.y);
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
			Vector2 correctAspectMousePosition(inputHandler->mouse.position.x * gameGraphics->aspectRatio, inputHandler->mouse.position.y);
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

		// update mouse position for the cursor element if active
		if(mouseActive)
			*((Vector2*)cursorEntry.second["position"]) = inputHandler->mouse.position;

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
				Vector3 movement(0.0f, 0.0f, gameSystem->getFloat("cameraMovementSpeed") * deltaTime);
				roamingCamera.position = roamingCamera.position + movement * directionMatrix;
			}
		} else if(gameGraphics->currentCamera == &fortressCamera) {
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

		// update missile cache and play effects if necessary
		for(size_t i = 0; i < gameState->missiles.size(); ++i) {
			if(! gameState->missiles[i].alive)
				continue;

			bool isInCache = false;

			for(size_t p = 0; p < missileCache.size(); ++p) {
				if(p == i) {
					isInCache = true;

					break;
				}
			}

			if(! isInCache) {
				missileCache.push_back(i);

				playEffectAtDistance("missileEffect", distance(gameState->fortress.position, gameState->missiles[i].position));
			}
		}

		size_t i = 0;
		while(i < missileCache.size()) {
			if(! gameState->missiles[missileCache[i]].alive) {
				playEffectAtDistance("explosionEffect", distance(gameState->fortress.position, gameState->missiles[missileCache[i]].position));

				missileCache.erase(missileCache.begin() + i);
			} else {
				++i;
			}
		}

		// see if we're dead
		if(gameState->fortress.health == 0.0f) {
			gameState->pause();

			mainLoopModules.erase(mainLoopModules.find(drawingMaster));

			playerName = "";

			currentScheme = SCHEME_GAMEOVER;
			activeMenuSelection = &gameOverContinueButton;
			inputHandler->keyboard.listenUnicode = true;
			inputHandler->keyboard.unicodeChars = "";

			needReScheme = true;
			needRedraw = true;
			SDL_WM_GrabInput(SDL_GRAB_OFF);
			SDL_ShowCursor(1);

			gameAudio->setBackgroundMusic("menuSong");
		}

		// button clicks
		if(
				primaryFireClickListener1->wasClicked() ||
				primaryFireClickListener2->wasClicked() ||
				primaryFireClickListener3->wasClicked()
			) {
			if(gameState->fortress.ammunition >= gameSystem->getFloat("stateAmmoFiringCost")) {
				gameState->fireShell();
				gameAudio->playSound("shellEffect");
			}
		}
		if(secondaryFireClickListener->wasClicked()) {
			gameState->empIsCharging = ! gameState->empIsCharging;

			if(! gameState->empIsCharging && gameState->fortress.emp == 1.0f)
				gameAudio->playSound("empEffect");
		}

		if(binocularsClickListener->wasClicked()) {
			gameState->binoculars = ! gameState->binoculars;
			needReScheme = true;
		}

		// key hits
		KeyListener* activeKeyListener = gameSystem->getBool("developmentMode") ? playingDevelopmentModeKeyListener : playingKeyListener;
		for(SDLKey key = activeKeyListener->popKey(); key != SDLK_UNKNOWN; key = activeKeyListener->popKey()) {
			if(key == SDLK_SPACE && gameGraphics->currentCamera == &fortressCamera) {
				if(gameState->fortress.ammunition >= gameSystem->getFloat("stateAmmoFiringCost")) {
					gameState->fireShell();
					gameAudio->playSound("shellEffect");
				}
			} else if(key == SDLK_TAB) {
				gameState->empIsCharging = ! gameState->empIsCharging;

				if(! gameState->empIsCharging && gameState->fortress.emp == 1.0f)
					gameAudio->playSound("empEffect");
			} else if(key == SDLK_RETURN) {
				mainLoopModules.erase(mainLoopModules.find(gameState));
				delete gameState;
				gameState = new GameState();
				mainLoopModules[gameState] = 0;

				((DrawRadar*) drawingMaster->drawers["radar"])->reloadState();
				((ExplosionRenderer*) drawingMaster->drawers["explosionRenderer"])->reloadState();
				((TerrainRenderer*) drawingMaster->drawers["terrainRenderer"])->reloadState();
				missileCache.clear();
			} else if(key == SDLK_BACKSLASH) {
				if(gameGraphics->currentCamera == &fortressCamera) {
					gameGraphics->currentCamera = &orbitCamera;
					needReScheme = true;
				} else if(gameGraphics->currentCamera == &orbitCamera) {
					gameGraphics->currentCamera = &presentationCamera;
					needReScheme = true;
				} else if(gameGraphics->currentCamera == &presentationCamera) {
					gameGraphics->currentCamera = &roamingCamera;
					needReScheme = true;
				} else if(gameGraphics->currentCamera == &roamingCamera) {
					gameGraphics->currentCamera = &fortressCamera;
					needReScheme = true;
				}
			} else if(key == SDLK_ESCAPE) {
				pauseGame();
			} else if(key == SDLK_BACKQUOTE) {
				if(gameState->isPaused)
					gameState->resume();
				else
					gameState->pause();
			} else if((key == SDLK_LSHIFT || key == SDLK_RSHIFT) && gameGraphics->currentCamera == &fortressCamera) {
				gameState->binoculars = ! gameState->binoculars;
				needReScheme = true;
			}
		}
	} else if(currentScheme == SCHEME_PAUSED) {
		// button highlight
		if(mouseMotionListener->wasMoved()) {
			if(resumeButtonZoneListener->isEntered) {
				if(activeMenuSelection != &resumeButtonEntry) {
					activeMenuSelection = &resumeButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(endGameButtonZoneListener->isEntered) {
				if(activeMenuSelection != &endGameButtonEntry) {
					activeMenuSelection = &endGameButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				needReScheme = true;
				needRedraw = true;
			}
		}

		// button clicks
		if(resumeButtonClickListener->wasClicked()) {
			resumeGame();
		} else if(endGameButtonClickListener->wasClicked()) {
			endGameFromPause();
		}

		// key hits
		for(SDLKey key = pausedMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = pausedMenuKeyListener->popKey()) {
			if(key == SDLK_UP) {
				if(activeMenuSelection == NULL || activeMenuSelection == &resumeButtonEntry) {
					activeMenuSelection = &endGameButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &endGameButtonEntry) {
					activeMenuSelection = &resumeButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				}
			} else if(key == SDLK_DOWN) {
				if(activeMenuSelection == NULL || activeMenuSelection == &endGameButtonEntry) {
					activeMenuSelection = &resumeButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &resumeButtonEntry) {
					activeMenuSelection = &endGameButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(key == SDLK_RETURN || key == SDLK_ESCAPE) {
				if(activeMenuSelection == &resumeButtonEntry || key == SDLK_ESCAPE) {
					resumeGame();
				} else if(activeMenuSelection == &endGameButtonEntry) {
					endGameFromPause();
				}
			}
		}
	} else if(currentScheme == SCHEME_GAMEOVER) {
		// typing
		if(inputHandler->keyboard.unicodeChars != "") {
			if(playerName.length() < gameSystem->getFloat("hudFieldWidth")) {
				for(size_t i = 0; i < inputHandler->keyboard.unicodeChars.length(); ++i) {
					if(gameSystem->getString("inputAllowedNameChars").find(inputHandler->keyboard.unicodeChars.substr(i, 1)) != std::string::npos) {
						playerName += inputHandler->keyboard.unicodeChars.substr(i, 1);

						activeMenuSelection = &gameOverContinueButton;

						needReScheme = true;
						needRedraw = true;
					}
				}
			}

			inputHandler->keyboard.unicodeChars = "";
		}

		if(deleteKeyListener->isDown && playerName.length() > 0) {
			if(deleteKeyPressTime > platform->getExecMills()) {
				playerName.erase(playerName.length() - 1);
				deleteKeyPressTime = platform->getExecMills();

				activeMenuSelection = &gameOverContinueButton;

				needReScheme = true;
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

				needReScheme = true;
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
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				needReScheme = true;
				needRedraw = true;
			}
		}

		// button clicks
		if(gameOverContinueButtonClickListener->wasClicked()) {
			continueFromGameOver();
		}

		// key hits
		for(SDLKey key = gameOverKeyListener->popKey(); key != SDLK_UNKNOWN; key = gameOverKeyListener->popKey()) {
			if(key == SDLK_UP || key == SDLK_DOWN) {
				if(activeMenuSelection != &gameOverContinueButton) {
					activeMenuSelection = &gameOverContinueButton;
					needReScheme = true;
					needRedraw = true;
					if(key == SDLK_UP)
						gameAudio->playSound("alterUpEffect");
					else
						gameAudio->playSound("alterDownEffect");
				}
			} else if(key == SDLK_RETURN) {
				continueFromGameOver();
			} else if(key == SDLK_ESCAPE) {
				mainLoopModules.erase(mainLoopModules.find(gameState));
				delete gameState;
				gameState = NULL;

				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				needReScheme = true;
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
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(musicButtonZoneListener->isEntered) {
				if(activeMenuSelection != &musicSettingEntry) {
					activeMenuSelection = &musicSettingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(audioEffectsButtonZoneListener->isEntered) {
				if(activeMenuSelection != &audioEffectsSettingEntry) {
					activeMenuSelection = &audioEffectsSettingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(fullscreenButtonZoneListener->isEntered) {
				if(activeMenuSelection != &fullscreenSettingEntry) {
					activeMenuSelection = &fullscreenSettingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(windowedScreenResolutionButtonZoneListener->isEntered) {
				if(activeMenuSelection != &windowedScreenResolutionEntry) {
					activeMenuSelection = &windowedScreenResolutionEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(framerateLimitingButtonZoneListener->isEntered) {
				if(activeMenuSelection != &framerateLimitingEntry) {
					activeMenuSelection = &framerateLimitingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(multisamplingButtonZoneListener->isEntered) {
				if(activeMenuSelection != &multisamplingLevelEntry) {
					activeMenuSelection = &multisamplingLevelEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(terrainDetailButtonZoneListener->isEntered) {
				if(activeMenuSelection != &terrainDetailEntry) {
					activeMenuSelection = &terrainDetailEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(developmentModeButtonZoneListener->isEntered) {
				if(activeMenuSelection != &developmentModeEntry) {
					activeMenuSelection = &developmentModeEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(backButtonZoneListener->isEntered) {
				if(activeMenuSelection != &backButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(resetHighScoresButtonZoneListener->isEntered) {
				if(activeMenuSelection != &resetHighScoresEntry) {
					activeMenuSelection = &resetHighScoresEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				needReScheme = true;
				needRedraw = true;
			}
		}

		// button clicks
		if(levelButtonClickListener->wasClicked()) {
			alterGameLevel();
		} else if(musicButtonClickListener->wasClicked()) {
			alterMusicLevel();
		} else if(audioEffectsButtonClickListener->wasClicked()) {
			alterAudioEffectsLevel();
		} else if(fullscreenButtonClickListener->wasClicked()) {
			alterFullscreenSetting();
		} else if(windowedScreenResolutionButtonClickListener->wasClicked()) {
			alterWindowedResolution();
		} else if(framerateLimitingButtonClickListener->wasClicked()) {
			alterFramerateLimiting();
		} else if(multisamplingButtonClickListener->wasClicked()) {
			alterMultiSamplingLevel();
		} else if(terrainDetailButtonClickListener->wasClicked()) {
			alterTerrainDetail();
		} else if(developmentModeButtonClickListener->wasClicked()) {
			alterDevelopmentMode();
		} else if(backButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_MAINMENU;
			activeMenuSelection = NULL;
			needReScheme = true;
			needRedraw = true;

			gameAudio->playSound("selectEffect");
		} else if(resetHighScoresButtonClickListener->wasClicked()) {
			resetHighScores();
		}

		// key hits
		for(SDLKey key = settingsMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = settingsMenuKeyListener->popKey()) {
			if(key == SDLK_UP) {
				if(activeMenuSelection == NULL || activeMenuSelection == &levelSettingEntry) {
					activeMenuSelection = &backButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &backButtonEntry) {
					activeMenuSelection = &resetHighScoresEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &resetHighScoresEntry) {
					activeMenuSelection = &developmentModeEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &developmentModeEntry) {
					activeMenuSelection = &terrainDetailEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &terrainDetailEntry) {
					if(gameGraphics->supportsMultisampling)
						activeMenuSelection = &multisamplingLevelEntry;
					else
						activeMenuSelection = &framerateLimitingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &multisamplingLevelEntry) {
					activeMenuSelection = &framerateLimitingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &framerateLimitingEntry) {
					activeMenuSelection = &windowedScreenResolutionEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &windowedScreenResolutionEntry) {
					activeMenuSelection = &fullscreenSettingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &fullscreenSettingEntry) {
					activeMenuSelection = &audioEffectsSettingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &audioEffectsSettingEntry) {
					activeMenuSelection = &musicSettingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &musicSettingEntry) {
					activeMenuSelection = &levelSettingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				}
			} else if(key == SDLK_DOWN) {
				if(activeMenuSelection == NULL || activeMenuSelection == &backButtonEntry) {
					activeMenuSelection = &levelSettingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &levelSettingEntry) {
					activeMenuSelection = &musicSettingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &musicSettingEntry) {
					activeMenuSelection = &audioEffectsSettingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &audioEffectsSettingEntry) {
					activeMenuSelection = &fullscreenSettingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &fullscreenSettingEntry) {
					activeMenuSelection = &windowedScreenResolutionEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &windowedScreenResolutionEntry) {
					activeMenuSelection = &framerateLimitingEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &framerateLimitingEntry) {
					if(gameGraphics->supportsMultisampling)
						activeMenuSelection = &multisamplingLevelEntry;
					else
						activeMenuSelection = &terrainDetailEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &multisamplingLevelEntry) {
					activeMenuSelection = &terrainDetailEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &terrainDetailEntry) {
					activeMenuSelection = &developmentModeEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &developmentModeEntry) {
					activeMenuSelection = &resetHighScoresEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &resetHighScoresEntry) {
					activeMenuSelection = &backButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(key == SDLK_LEFT || key == SDLK_RIGHT) {
				if(activeMenuSelection == &levelSettingEntry) {
					alterGameLevel(key == SDLK_RIGHT ? true : false);
				} else if(activeMenuSelection == &musicSettingEntry) {
					alterMusicLevel(key == SDLK_RIGHT ? true : false);
				} else if(activeMenuSelection == &audioEffectsSettingEntry) {
					alterAudioEffectsLevel(key == SDLK_RIGHT ? true : false);
				} else if(activeMenuSelection == &fullscreenSettingEntry) {
					alterFullscreenSetting(key == SDLK_RIGHT ? true : false);
				} else if(activeMenuSelection == &windowedScreenResolutionEntry) {
					alterWindowedResolution(key == SDLK_RIGHT ? true : false);
				} else if(activeMenuSelection == &framerateLimitingEntry) {
					alterFramerateLimiting(key == SDLK_RIGHT ? true : false);
				} else if(activeMenuSelection == &multisamplingLevelEntry) {
					alterMultiSamplingLevel(key == SDLK_RIGHT ? true : false);
				} else if(activeMenuSelection == &terrainDetailEntry) {
					alterTerrainDetail(key == SDLK_RIGHT ? true : false);
				} else if(activeMenuSelection == &developmentModeEntry) {
					alterDevelopmentMode(key == SDLK_RIGHT ? true : false);
				}
			} else if(key == SDLK_RETURN) {
				if(activeMenuSelection == &resetHighScoresEntry) {
					resetHighScores();
				} else if(activeMenuSelection == &backButtonEntry) {
					currentScheme = SCHEME_MAINMENU;
					activeMenuSelection = &playButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("selectEffect");
				}
			} else if(key == SDLK_ESCAPE) {
				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				needReScheme = true;
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
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				needReScheme = true;
				needRedraw = true;
			}
		}

		// button clicks
		if(backButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_MAINMENU;
			activeMenuSelection = &playButtonEntry;
			needReScheme = true;
			needRedraw = true;
			gameAudio->playSound("selectEffect");
		}

		// key hits
		for(SDLKey key = highScoresMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = highScoresMenuKeyListener->popKey()) {
			if((key == SDLK_UP || key == SDLK_DOWN) && activeMenuSelection != &backButtonEntry) {
				activeMenuSelection = &backButtonEntry;
				needReScheme = true;
				needRedraw = true;
				gameAudio->playSound("alterDownEffect");
			} else if((key == SDLK_RETURN && activeMenuSelection == &backButtonEntry) || key == SDLK_ESCAPE) {
				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				needReScheme = true;
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
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(backButtonZoneListener->isEntered) {
				if(activeMenuSelection != &backButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				needReScheme = true;
				needRedraw = true;
			}
		}

		// button clicks
		if(aboutButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_ABOUT;
			activeMenuSelection = &backButtonEntry;
			needReScheme = true;
			needRedraw = true;
			gameAudio->playSound("selectEffect");
		} else if(backButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_MAINMENU;
			activeMenuSelection = &playButtonEntry;
			needReScheme = true;
			needRedraw = true;
			gameAudio->playSound("selectEffect");
		}

		// key hits
		for(SDLKey key = helpMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = helpMenuKeyListener->popKey()) {
			if(key == SDLK_UP) {
				if(activeMenuSelection == NULL || activeMenuSelection == &aboutButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				} else if(activeMenuSelection == &backButtonEntry) {
					activeMenuSelection = &aboutButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterUpEffect");
				}
			} else if(key == SDLK_DOWN) {
				if(activeMenuSelection == NULL || activeMenuSelection == &backButtonEntry) {
					activeMenuSelection = &aboutButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				} else if(activeMenuSelection == &aboutButtonEntry) {
					activeMenuSelection = &backButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(key == SDLK_RETURN) {
				if(activeMenuSelection == &aboutButtonEntry) {
					currentScheme = SCHEME_ABOUT;
					activeMenuSelection = &backButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("selectEffect");
				} else if(activeMenuSelection == &backButtonEntry) {
					currentScheme = SCHEME_MAINMENU;
					activeMenuSelection = &playButtonEntry;
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("selectEffect");
				}
			} else if(key == SDLK_ESCAPE) {
				currentScheme = SCHEME_MAINMENU;
				activeMenuSelection = &playButtonEntry;
				needReScheme = true;
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
					needReScheme = true;
					needRedraw = true;
					gameAudio->playSound("alterDownEffect");
				}
			} else if(activeMenuSelection != NULL) {
				activeMenuSelection = NULL;
				needReScheme = true;
				needRedraw = true;
			}
		}

		// button clicks
		if(backButtonClickListener->wasClicked()) {
			currentScheme = SCHEME_HELP;
			activeMenuSelection = &backButtonEntry;
			needReScheme = true;
			needRedraw = true;
			gameAudio->playSound("selectEffect");
		}

		// key hits
		for(SDLKey key = helpMenuKeyListener->popKey(); key != SDLK_UNKNOWN; key = helpMenuKeyListener->popKey()) {
			if((key == SDLK_UP || key == SDLK_DOWN) && activeMenuSelection != &backButtonEntry) {
				activeMenuSelection = &backButtonEntry;
				needReScheme = true;
				needRedraw = true;
				if(key == SDLK_UP)
					gameAudio->playSound("alterUpEffect");
				else
					gameAudio->playSound("alterDownEffect");
			} else if((key == SDLK_RETURN && activeMenuSelection == &backButtonEntry) || key == SDLK_ESCAPE) {
				currentScheme = SCHEME_HELP;
				activeMenuSelection = &backButtonEntry;
				needReScheme = true;
				needRedraw = true;
				if(key == SDLK_RETURN)
					gameAudio->playSound("selectEffect");
				else
					gameAudio->playSound("backEffect");
			}
		}
	}

	// rescheme once per execution if we need to (helper functions don't participate in this)
	if(needReScheme)
		reScheme();

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
