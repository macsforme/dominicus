// DrawingMaster.cpp
// Dominicus

#include "graphics/DrawingMaster.h"

#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "graphics/2dgraphics/DrawButton.h"
#include "graphics/2dgraphics/DrawCircle.h"
#include "graphics/2dgraphics/DrawContainer.h"
#include "graphics/2dgraphics/DrawField.h"
#include "graphics/2dgraphics/DrawGaugePanel.h"
#include "graphics/2dgraphics/DrawGrayOut.h"
#include "graphics/2dgraphics/DrawLabel.h"
#include "graphics/2dgraphics/DrawMissileIndicators.h"
#include "graphics/2dgraphics/DrawProgressBar.h"
#include "graphics/2dgraphics/DrawRadar.h"
#include "graphics/2dgraphics/DrawRoundedTriangle.h"
#include "graphics/2dgraphics/DrawSplash.h"
#include "graphics/2dgraphics/DrawStrikeEffect.h"
#include "graphics/2dgraphics/DrawTexture.h"
#include "graphics/3dgraphics/ExplosionRenderer.h"
#include "graphics/3dgraphics/FortressRenderer.h"
#include "graphics/3dgraphics/MissileRenderer.h"
#include "graphics/3dgraphics/MissileTrailRenderer.h"
#include "graphics/3dgraphics/ShellRenderer.h"
#include "graphics/3dgraphics/ShipRenderer.h"
#include "graphics/3dgraphics/SkyRenderer.h"
#include "graphics/3dgraphics/TerrainRenderer.h"
#include "graphics/3dgraphics/WaterRenderer.h"
#include "math/VectorMath.h"

extern GameSystem* gameSystem;
extern GameGraphics* gameGraphics;

DrawingMaster::DrawingMaster() : MainLoopMember(0) {
	uiLayoutAuthority = new UILayoutAuthority(
			Vector2(gameSystem->getFloat("hudElementMargin") / gameGraphics->resolutionX,
					gameSystem->getFloat("hudElementMargin") / gameGraphics->resolutionY)
		);

	buildDrawers();
}

DrawingMaster::~DrawingMaster() {
	destroyDrawers();

	delete uiLayoutAuthority;
}

void DrawingMaster::buildDrawers() {
	drawers["circle"] = new DrawCircle();
	drawers["container"] = new DrawContainer();
	drawers["grayOut"] = new DrawGrayOut();
	drawers["label"] = new DrawLabel();
	drawers["progressBar"] = new DrawProgressBar();
	drawers["roundedTriangle"] = new DrawRoundedTriangle();
	drawers["splash"] = new DrawSplash();
	drawers["strikeEffect"] = new DrawStrikeEffect();
	drawers["texture"] = new DrawTexture();
	drawers["button"] = new DrawButton((DrawContainer*) drawers["container"], (DrawLabel*) drawers["label"]);
	drawers["field"] = new DrawField((DrawLabel*) drawers["label"]);
	drawers["gaugePanel"] = new DrawGaugePanel(
			(DrawContainer*) drawers["container"],
			(DrawProgressBar*) drawers["progressBar"],
			(DrawTexture*) drawers["texture"]
		);
	drawers["missileIndicators"] = new DrawMissileIndicators((DrawRoundedTriangle*) drawers["roundedTriangle"]);
	drawers["radar"] = new DrawRadar(
			(DrawContainer*) drawers["container"],
			(DrawCircle*) drawers["circle"],
			(DrawRoundedTriangle*) drawers["roundedTriangle"]
		);

	drawers["explosionRenderer"] = new ExplosionRenderer();
	drawers["fortressRenderer"] = new FortressRenderer();
	drawers["missileRenderer"] = new MissileRenderer();
	drawers["missileTrailRenderer"] = new MissileTrailRenderer();
	drawers["shellRenderer"] = new ShellRenderer();
	drawers["shipRenderer"] = new ShipRenderer();
	drawers["skyRenderer"] = new SkyRenderer();
	drawers["terrainRenderer"] = new TerrainRenderer();
	drawers["waterRenderer"] = new WaterRenderer();
}

void DrawingMaster::destroyDrawers() {
	delete (DrawButton*) drawers["button"];
	delete (DrawCircle*) drawers["circle"];
	delete (DrawContainer*) drawers["container"];
	delete (DrawField*) drawers["field"];
	delete (DrawGaugePanel*) drawers["gaugePanel"];
	delete (DrawGrayOut*) drawers["grayOut"];
	delete (DrawLabel*) drawers["label"];
	delete (DrawLabel*) drawers["missileIndicators"];
	delete (DrawProgressBar*) drawers["progressBar"];
	delete (DrawRadar*) drawers["radar"];
	delete (DrawRoundedTriangle*) drawers["roundedTriangle"];
	delete (DrawSplash*) drawers["splash"];
	delete (DrawStrikeEffect*) drawers["strikeEffect"];
	delete (DrawTexture*) drawers["texture"];

	delete (ExplosionRenderer*) drawers["explosionRenderer"];
	delete (FortressRenderer*) drawers["fortressRenderer"];
	delete (MissileRenderer*) drawers["missileRenderer"];
	delete (MissileTrailRenderer*) drawers["missileTrailRenderer"];
	delete (ShellRenderer*) drawers["shellRenderer"];
	delete (ShipRenderer*) drawers["shipRenderer"];
	delete (TerrainRenderer*) drawers["terrainRenderer"];
	delete (WaterRenderer*) drawers["waterRenderer"];
}

void DrawingMaster::newGraphics() {
	uiLayoutAuthority->elementMargin = Vector2(
			gameSystem->getFloat("hudElementMargin") / gameGraphics->resolutionX,
			gameSystem->getFloat("hudElementMargin") / gameGraphics->resolutionY
		);

	destroyDrawers();
	buildDrawers();
}

unsigned int DrawingMaster::execute(bool unScheduled) {
	// check that our framerate limiting is on the correct schedule (can't set this in the constructor because it may change)
	if(
			(int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_VSYNC ||
			(int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_OFF
		)
		targetSleepMills = 0;
	if(
			(int) gameSystem->getFloat("displayFramerateLimiting") != GameSystem::LIMIT_VSYNC &&
			(int) gameSystem->getFloat("displayFramerateLimiting") != GameSystem::LIMIT_OFF
		) {
		if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_30)
			targetSleepMills = 1000 / 30;
		else if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_60)
			targetSleepMills = 1000 / 60;
		else if((int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_120)
			targetSleepMills = 1000 / 120;
	}

	// draw everything
	gameGraphics->startFrame();

	for(
			DrawStack::iterator itr = drawStack.begin();
			itr != drawStack.end();
			++itr
		)
		drawers[itr->first]->execute(itr->second);

	gameGraphics->finishFrame();

	// track runcount
	trackRunCount();

	// calculate and return sleep time from superclass unless we aren't capping it
	if(
			unScheduled ||
			(int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_VSYNC ||
			(int) gameSystem->getFloat("displayFramerateLimiting") == GameSystem::LIMIT_OFF
		)
		return 0;
	else
		return getSleepTime();
}
