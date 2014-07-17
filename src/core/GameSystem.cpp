// GameSystem.cpp
// Crucible Island

#include "core/GameSystem.h"

bool GameSystem::isStandard(const char* key) const {
	if(standards.find(key) != standards.end())
		return true;
	else
		return false;
}

GameSystem::StandardEntry GameSystem::getStandard(const char* key) {
	if(standards.find(key) == standards.end()) {
		std::stringstream logMessage;
		logMessage << "Non-existent standards key requested: " << key << ".";

		this->log(LOG_FATAL, logMessage.str().c_str());
	}

	return standards[key];
}

std::string GameSystem::getString(const char* key) {
	return getStandard(key).value;
}

Vector4 GameSystem::getColor(const char* key) {
	int colors[4];
	sscanf(getString(key).c_str(), "%2x%2x%2x%2x", &colors[0], &colors[1], &colors[2], &colors[3]);

	return Vector4(
			(float) colors[0] / 255.0f,
			(float) colors[1] / 255.0f,
			(float) colors[2] / 255.0f,
			(float) colors[3] / 255.0f
		);
}

float GameSystem::getFloat(const char* key) {
	return (float) atof(getString(key).c_str());
}

bool GameSystem::getBool(const char* key) {
	return getString(key) != "false";
}

void GameSystem::setStandard(const char* key, const char* value,
		const char* description, bool locked) {
	StandardEntry entry;

	entry.value = value;
	if(strcmp(description, "") != 0) entry.description = description;
	entry.locked = locked;

	standards[key] = entry;
}

void GameSystem::setStandard(const char* key, Vector4 value,
		const char* description, bool locked) {
	char str[9];
	sprintf(
			str,
			"%02x%02x%02x%02x",
			(int) (value.x * 255.0f),
			(int) (value.y * 255.0f),
			(int) (value.z * 255.0f),
			(int) (value.w * 255.0f)
		);

	setStandard(key, str, description, locked);
}

void GameSystem::setStandard(const char* key, float value,
		const char* description, bool locked) {
	std::stringstream str;
	str << value;

	setStandard(key, str.str().c_str(), description, locked);
}

void GameSystem::setStandard(const char* key, bool value,
		const char* description, bool locked) {
	setStandard(key, (value != false ? "true" : "false"), description, locked);
}

void GameSystem::flushPreferences() {
	platform->setPreference("preferencesVersion", 3.0f);
	platform->setPreference("displayWindowedResolution", getString("displayWindowedResolution").c_str());
	platform->setPreference("displayStartFullscreen", (getBool("displayStartFullscreen") == true ? 1.0f : 0.0f));
	platform->setPreference("displayFramerateLimiting", getFloat("displayFramerateLimiting"));
	platform->setPreference("displayMultisamplingLevel", getFloat("displayMultisamplingLevel"));
	platform->setPreference("audioMusicVolume", getFloat("audioMusicVolume"));
	platform->setPreference("audioEffectsVolume", getFloat("audioEffectsVolume"));
	platform->setPreference("gameStartingLevel", getString("gameStartingLevel").c_str());
	platform->setPreference("islandTerrainDetail", getFloat("islandTerrainDetail"));
	if(highScores.size() == 0) {
		platform->setPreference("highScores", "");
	} else {
		std::stringstream stringStream;
		stringStream << "\"";
		for(size_t i = 0; i < highScores.size(); ++i)
			stringStream << (i > 0 ? "\n" : "") << highScores[i].second << "\t" << highScores[i].first;
		stringStream << "\"";
		platform->setPreference("highScores", stringStream.str().c_str());
	}
}

void GameSystem::log(LogDetail detail, std::string report) {
	std::stringstream fullReport;

	if(platform != NULL)
		fullReport << platform->getExecMills() << " ";
	else
		fullReport << "0 ";

	if(detail == LOG_INFO)
		fullReport << "INFO: " << report;
	else if(detail == LOG_VERBOSE)
		fullReport << "VERBOSE: " << report;
	else
		fullReport << "FATAL: " << report;

		logLines.push_back(fullReport.str());

	if(detail == LOG_FATAL) {
		Platform::consoleOut(fullReport.str() + "\n");

		exit(1);
	}
}

std::vector< std::pair<unsigned int, unsigned int> > GameSystem::getAllowedWindowResolutions() {
	// return all resolutions within limit of screen resolution factor
	std::string resolutionsString = getString("displayWindowedResolutions");
	std::vector< std::pair<unsigned int, unsigned int> > resolutions;

	size_t stringOffset = 0;
	while(stringOffset < resolutionsString.length()) {
		std::string resolution = resolutionsString.substr(stringOffset, resolutionsString.find(',', stringOffset) - stringOffset);
		std::pair<unsigned int, unsigned int> resolutionPair;
		resolutionPair.first = (unsigned int) atoi(resolution.substr(0, resolution.find('x')).c_str());
		resolutionPair.second = (unsigned int) atoi(resolution.substr(resolution.find('x') + 1, std::string::npos).c_str());

		if(resolutionPair.second <= (unsigned int) ((float) displayResolutionY * getFloat("displayWindowedMaxPortion")))
			resolutions.push_back(resolutionPair);

		stringOffset += resolution.length() + 1;
	}

	return resolutions;
}

void GameSystem::applyScreenResolution(std::string resolution) {
	std::string resolutionsString = getString("displayWindowedResolutions");
	float scalingFactor = atof(resolution.substr(resolution.find('x') + 1, std::string::npos).c_str()) /
			atof(resolutionsString.substr(resolutionsString.find('x') + 1, resolutionsString.find(',') - resolutionsString.find('x') + 1).c_str());

	// round sizes to nearest even integer
	setStandard("hudElementMargin", (float) (roundToInt(getFloat("hudBaseElementMargin") * scalingFactor / 2.0f) * 2));
	setStandard("hudContainerPadding", (float) (roundToInt(getFloat("hudBaseContainerPadding") * scalingFactor / 2.0f) * 2));
	setStandard("hudButtonPadding", (float) (roundToInt(getFloat("hudBaseButtonPadding") * scalingFactor / 2.0f) * 2));
	setStandard("hudBigButtonPadding", (float) (roundToInt(getFloat("hudBaseBigButtonPadding") * scalingFactor / 2.0f) * 2));
	setStandard("hudGaugePadding", (float) (roundToInt(getFloat("hudBaseGaugePadding") * scalingFactor / 2.0f) * 2));
	setStandard("fontSizeSmall", (float) (roundToInt(getFloat("fontBaseSizeSmall") * scalingFactor / 2.0f) * 2));
	setStandard("fontSizeMedium", (float) (roundToInt(getFloat("fontBaseSizeMedium") * scalingFactor / 2.0f) * 2));
	setStandard("fontSizeLarge", (float) (roundToInt(getFloat("fontBaseSizeLarge") * scalingFactor / 2.0f) * 2));
	setStandard("fontSizeSuper", (float) (roundToInt(getFloat("fontBaseSizeSuper") * scalingFactor / 2.0f) * 2));
	setStandard("logoHeight", (float) (roundToInt(getFloat("logoBaseHeight") * scalingFactor / 2.0f) * 2));
	setStandard("gaugeImagesHeight", (float) (roundToInt(getFloat("gaugeImagesBaseHeight") * scalingFactor / 2.0f) * 2));
}

GameSystem::GameSystem() {
	// set the build version string
	std::stringstream versionStream;
	versionStream <<
			PROGRAM_IDENTIFIER << " " <<
			PROGRAM_VERSION << " " <<
			"(" << PROGRAM_BUILDSTRING << ") " <<
			PROGRAM_ARCH_STR << " ";

	const char* dateString = __DATE__;

	std::string monthString = std::string(dateString).substr(0, 3);
	int month = 0;
	if(strcmp(monthString.c_str(), "Jan") == 0) month = 1;
	else if(strcmp(monthString.c_str(), "Feb") == 0) month = 2;
	else if(strcmp(monthString.c_str(), "Mar") == 0) month = 3;
	else if(strcmp(monthString.c_str(), "Apr") == 0) month = 4;
	else if(strcmp(monthString.c_str(), "May") == 0) month = 5;
	else if(strcmp(monthString.c_str(), "Jun") == 0) month = 6;
	else if(strcmp(monthString.c_str(), "Jul") == 0) month = 7;
	else if(strcmp(monthString.c_str(), "Aug") == 0) month = 8;
	else if(strcmp(monthString.c_str(), "Sep") == 0) month = 9;
	else if(strcmp(monthString.c_str(), "Oct") == 0) month = 10;
	else if(strcmp(monthString.c_str(), "Nov") == 0) month = 11;
	else month = 12;

	const int day = atoi(std::string(dateString).substr(4, 2).c_str());
	const int year = atoi(std::string(dateString).substr(7, 4).c_str());

	char fullDateString[11];
	sprintf(fullDateString, "%04i-%02i-%02i", year, month, day);
	versionStream << fullDateString;
	versionString = versionStream.str();

	// get the display resolution
	SDL_VideoInfo* vidInfo = (SDL_VideoInfo*) SDL_GetVideoInfo();
	if(vidInfo == NULL)
		this->log(LOG_FATAL, "Could not obtain screen resolution from SDL.");

	displayResolutionX = (unsigned short int) vidInfo->current_w;
	displayResolutionY = (unsigned short int) vidInfo->current_h;

	// window and element scaling
	setStandard("displayWindowedResolutions", "800x600,1024x768,1152x864,1280x1024,1600x1200", "Supported resolutions for windowed mode.");
	setStandard("displayWindowedMaxPortion", 0.9f, "Maximum portion of vertical screen resolution to take up in windowed mode.");

	std::vector< std::pair<unsigned int, unsigned int> > allowedResolutions = getAllowedWindowResolutions();

	std::stringstream allowedResolutionsText;
	for(size_t i = 0; i < allowedResolutions.size(); ++i)
		allowedResolutionsText << (i > 0 ? "," : "") << allowedResolutions[i].first << "x" << allowedResolutions[i].second;
	setStandard("displayWindowedResolutions", allowedResolutionsText.str().c_str());

	std::stringstream maximumResolutionText;
	maximumResolutionText << allowedResolutions[allowedResolutions.size() - 1].first << "x" <<
			allowedResolutions[allowedResolutions.size() - 1].second;
	setStandard("displayWindowedResolution", maximumResolutionText.str().c_str());

	setStandard("hudBaseElementMargin", 28.0f, "Base value for space between HUD elements in pixels (must be even number).");
	setStandard("hudBaseContainerPadding", 10.0f, "Base value for space between HUD elements' external border and content in pixels.");
	setStandard("hudBaseButtonPadding", 10.0f, "Base value for space between HUD buttons' external border and content in pixels.");
	setStandard("hudBaseBigButtonPadding", 12.0f, "Base value for space between large HUD buttons' external border and content in pixels.");
	setStandard("hudBaseGaugePadding", 20.0f, "Base value for gauge panel padding in pixels.");
	setStandard("fontBaseSizeSmall", 12.0f, "Font size for small display in points (1/72 inch).");
	setStandard("fontBaseSizeMedium", 18.0f, "Font size for standard display in points (1/72 inch).");
	setStandard("fontBaseSizeLarge", 26.0f, "Font size for enlarged display in points (1/72 inch).");
	setStandard("fontBaseSizeSuper", 36.0f, "Font size for title display in points (1/72 inch).");
	setStandard("logoBaseHeight", 40.0f, "Base logo height in pixels");
	setStandard("gaugeImagesBaseHeight", 40.0f, "Base height of gauge images.");

	// state standards
	setStandard("stateUpdateFrequency", 120.0f, "Number of times per second the core state updates.");
	setStandard("stateShipOrbitMargin", 500.0f, "Radius of margin between maximum edge of island and first ship orbit.");
	setStandard("stateShipMargin", 150.0f, "Lateral distance between ships orbiting island.");
	setStandard("stateShipSpeed", 100.0f, "Ship speed in world units per second.");
	setStandard("stateShipEntryTime", 20.0f, "Time it takes between ship introduction and beginning of orbit.");
	setStandard("stateShipAddRate", 17.0f, "Time between ships being added to the world.");
	setStandard("stateMissileSpeed", 100.0f, "Missile speed in world units per second.");
	setStandard("stateMissileFiringRate", 12.0f, "Wait time in between missile firings for each ship.");
	setStandard("stateMissileRadiusMultiplier", 1.5f, "Multiplier of missile radius for actual collision area.");
	setStandard("stateFortressMinimumTilt", 0.0f, "Minimum tilt angle of fortress turret.");
	setStandard("stateFortressMaximumTilt", 45.0f, "Maximum tilt angle of fortress turret.");
	setStandard("stateTurretTurnSpeed", 90.0f, "Turning speed of turret in degrees per second.");
	setStandard("stateHealthRegenerationRate", 0.0625f, "Portion of fortress health capacity regenerated each second.");
	setStandard("stateMissileStrikeDepletion", 0.5f, "Portion of fortress health depleted by one missile strike.");
	setStandard("stateAmmoFiringCost", 0.1f, "Portion of total ammunition capacity depleted by firing one shell.");
	setStandard("stateAmmoReloadMultiplier", 4.0f, "Multiplier of reload rate for amount of ammo actually needed to counter missile firing rate.");
	setStandard("stateShellSpeed", 500.0f, "Shell speed in world units per second.");
	setStandard("stateShellExpirationDistance", 1500.0f, "Distance at which shells are deleted.");
	setStandard("stateEMPFiringCost", 0.5f, "Portion of total ammunition capacity depleted by firing one EMP.");
	setStandard("stateEMPHealthCost", 0.5f, "Portion of total health capacity depleted by firing one EMP.");
	setStandard("stateEMPChargingTime", 3.0f, "Time in seconds required for EMP charge.");
	setStandard("stateEMPRange", 350.0f, "Radius in world units of EMP blast.");
	setStandard("stateEMPDuration", 1.5f, "Time required for one full EMP discharge.");
	setStandard("stateTurretRecoilSpeed", 0.125f, "Time it takes for turret to recoil after shot.");
	setStandard("stateTurretRecoilRecoverySpeed", 1.0f, "Time it takes for turret to recover from recoil after shot.");
	setStandard("stateTurretRecoilDistance", 3.0f, "Distance in world units of turret recoil.");
	setStandard("stateKeyDampeningBasePortion", 0.0f, "Portion of movement to make when key initially pressed.");
	setStandard("stateKeyDampeningTime", 0.5f, "Time in seconds over which to dampen arrow key presses");
	setStandard("stateKeyDampeningExponent", 1.0f, "Power to raise fractional key movement values to when dampening key input");

	// input standards
	setStandard("inputPollingFrequency", 120.0f, "Number of times per second to poll the input devices.");
	setStandard("inputAllowedNameChars", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-_ ", "Characters allowed to be used in names on high scores list.");
	setStandard("inputDeleteKeyRepeatRate", 0.125f, "Wait time in between deletion of characters when backspace key is held down.");
	setStandard("inputDeleteKeyRepeatWait", 0.5f, "Wait time before commencing rapid delete when backspace key is held down.");

	// logic standards
	setStandard("logicUpdateFrequency", 120.0f, "Number of times per second to update game logic.");

	// display and drawing standards
	setStandard("displayFramerateLimiting", (float) LIMIT_VSYNC, "How to limit framerate (vsync, fps count, or off).");
	setStandard("displayStartFullscreen", false, "Whether or not to start the program in full screen mode.");
	setStandard("displayColorDepth", 32.0f, "Color depth of display (may only affect full screen mode).");
	setStandard("displayMultisamplingLevel", 4.0f, "Multisampling level (0, 2, or 4).");
	setStandard("colorClear", Vector4(174.0f / 255.0f, 187.0f / 255.0f, 224.0f / 255.0f, 1.0f), "Color of empty space.");

	// scene rendering standards
	setStandard("renderingPerspectiveFOV", 30.0f, "Field-of-view angle for perspective projection.");
	setStandard("renderingPerspectiveBinocularsFOV", 10.0f, "Field-of-view angle for perspective projection while binoculars enabled.");
	setStandard("renderingPerspectiveNearClip", 0.5f, "Near clip distance for perspective projection.");
	setStandard("renderingPerspectiveFarClip", 9000.0f, "Far clip distance for perspective projection.");
	setStandard("waterColor", Vector4(0.025f, 0.05f, 0.15f, 1.0f), "Water color.");
	setStandard("horizonColor", Vector4(0.88f, 0.88f, 0.88f, 1.0f), "Horizon color.");
	setStandard("baseSkyColor", Vector4(0.76f, 0.88f, 1.0f, 1.0f), "Sky color at approximately halfway up.");
	setStandard("apexColor", Vector4(0.08f, 0.24f, 0.4f, 1.0f), "Sky color at apex.");
	setStandard("empColorMultiplier", 4.0f, "Values to multiply colors by while under EMP.");
	setStandard("terrainDepth", 10.0f, "How far below the water the ground extends.");
	setStandard("terrainTextureRepeat", 50.0f, "Number of times to repeat the ground texture over the maximum surface area.");
	setStandard("terrainNoiseTextureDensity", 512.0f, "Terrain mixing noise texture resolution.");
	setStandard("terrainNoiseTextureRoughness", 0.6f, "Terrain mixing noise texture roughness factor.");
	setStandard("terrainNoiseTextureDepth", 4.0f, "Terrain mixing noise texture color depth.");
	setStandard("shellDensity", 32.0f, "Number of segments for shell sphere.");

	// HUD standards
	setStandard("hudFPSTestFrequency", 1.0f, "Frequency per second of the FPS test.");
	setStandard("hudControlSpotSize", 80.0f, "Width and height of control spot in pixels.");
	setStandard("hudControlSpotBorder", 5.0f, "Thickness of control spot border in pixels.");
	setStandard("hudControlSpotColor", Vector4(0.3f, 0.3f, 0.3f, 0.6f), "Color of control spot.");
	setStandard("hudCursorSize", 40.0f, "Width and height of cursor in pixels.");
	setStandard("hudCursorColor", Vector4(0.0f, 0.0f, 0.0f, 0.5f), "Color of cursor.");
	setStandard("hudCursorRange", 0.75f, "Portion of screen height which is radius of cursor range.");
	setStandard("hudCursorPositionExponent", 1.5f, "Raise the cursor fractional position to this exponent for more precision toward middle.");
	setStandard("radarSize", 35.0f, "Size of radar panel in percentage of screen height.");
	setStandard("radarRefreshSpeed", 1.0f, "Time in seconds for a full radar turn.");
	setStandard("radarSpotSize", 6.0f, "Size in pixels of radar missile spots.");
	setStandard("radarCenterSpotSize", 8.0f, "Size in pixels of radar missile spots.");
	setStandard("radarSpotColor", Vector4(1.0f, 0.0f, 0.0f, 1.0f), "Color of radar missile spots.");
	setStandard("radarEMPColor", Vector4(1.0f, 1.0f, 1.0f, 0.5f), "Color of radar missile spots.");
	setStandard("radarRadius", 1500.0f, "Radius of radar coverage.");
	setStandard("hudContainerBorder", 2.0f, "Thickness in pixels of HUD container element borders.");
	setStandard("hudContainerSoftEdge", 2.0f, "Thickness in pixels of HUD container element border antialiased portion.");
	setStandard("hudContainerInsideColor", Vector4(0.15f, 0.15f, 0.15f, 0.75f), "Background color of HUD container elements.");
	setStandard("hudContainerHighlightColor", Vector4(0.863f, 0.863f, 0.863f, 0.247f), "Highlight background color of HUD container elements.");
	setStandard("hudContainerBorderColor", Vector4(0.918f, 1.0f, 0.945f, 0.714f), "Border color of HUD container elements.");
	setStandard("hudContainerOutsideColor", Vector4(0.15f, 0.15f, 0.15f, 0.0f), "Outside color of HUD container elements.");
	setStandard("hudFieldWidth", 20.0f, "Standard field width (in number of '#' characters).");
	setStandard("hudFieldColor", Vector4(0.031f, 0.075f, 0.184f, 0.752f), "Background color for inactive text fields.");
	setStandard("hudGaugeWidth", 200.0f, "Width of gauges in pixels.");
	setStandard("hudGaugeHeight", 30.0f, "Height of gauges in pixels.");
	setStandard("hudGaugeBackgroundColor", Vector4(0.3f, 0.3f, 0.3f, 0.6f), "Background color of gauges.");
	setStandard("hudGaugeColorFalloff", Vector4(0.3f, 0.3f, 0.3f, 0.75f), "Factor to be multiplied into gauge color for falloff at bottom.");
	setStandard("hudGaugeHealthBarColor", Vector4(0.0f, 1.0f, 0.0f, 1.0f), "Color of health gauge.");
	setStandard("hudGaugeAmmoBarColor", Vector4(0.0f, 1.0f, 1.0f, 1.0f), "Color of ammunition gauge.");
	setStandard("hudGaugeEMPChargingBarColor", Vector4(1.0f, 1.0f, 0.0f, 1.0f), "Color of EMP gauge while charging.");
	setStandard("hudGaugeEMPChargedBarColor", Vector4(1.0f, 0.0f, 0.0f, 1.0f), "Color of EMP gauge when charged.");
	setStandard("hudGrayOutColor", Vector4(0.0f, 0.0f, 0.05f, 0.75f), "Color of gray screen during pause.");
	setStandard("hudStrikeEffectTime", 1.0f, "Time in seconds to display strike effect.");
	setStandard("hudMissileIndicatorSize", 50.0f, "Size in pixels of missile indicator box.");
	setStandard("hudMissileIndicatorBinocularsFactor", 3.0f, "Factor to multiply missile indicator size by when in binoculars mode.");
	setStandard("hudMissileIndicatorColor", Vector4(1.0f, 0.0f, 0.0f, 1.0f), "Color of missile indicator box.");

	// font standards
	setStandard("fontFile", "TitilliumWeb-Bold.ttf", "Font file to load for use by HUD and menus.");
	setStandard("fontColorLight", Vector4(1.0f, 1.0f, 1.0f, 1.0f), "Light font color.");
	setStandard("fontColorDark", Vector4(0.5f, 0.5f, 0.5f, 1.0f), "Medium font color.");
	setStandard("helpTextScreenPortion", 0.9f, "Horizontal portion of screen taken up by help text.");

	// audio standards
	setStandard("audioTickRate", 50.0f, "Audio manager tick rate.");
	setStandard("audioMusicVolume", 0.5f, "Music volume.");
	setStandard("audioEffectsVolume", 0.5f, "Audio effects volume.");

	// general game standards
	setStandard("gameStartingLevel", "Easy", "Starting difficulty level.");
	setStandard("gameMaximumHighScores", 10.0f, "Maximum number of high scores to track.");
	setStandard("islandMaximumWidth", 1000.0f, "Maximum island width.");
	setStandard("islandMaximumHeight", 100.0f, "Maximum island height.");
	setStandard("islandTerrainBaseDensity", 128.0f, "Density of island terrain tessellation.");
	setStandard("islandTerrainDetail", 2.0f, "Island detail level (1, 2, or 3) beyond base density, to be specified by user.");
	setStandard("islandTerrainRoughness", 0.5f, "Roughness of island terrain randomization.");
	setStandard("islandTerrainGradDist", 0.5f, "Island terrain generation gradual distance factor.");
	setStandard("islandTerrainBlends", 4.0f, "Island terrain generation blending factor.");
	setStandard("islandTerrainSink", 0.5f, "Island terrain generation sink to sea level factor.");

	// text strings
	setStandard("textControls", "Move Turret:\tArrow Keys / Mouse Movement\nFire Cannon:\tSpace / Left Mouse Button\nCharge / Fire EMP:\tTab / Right Mouse Button\nToggle Binoculars:\tShift / Middle Mouse Button\nPause / Resume:\tesc\nToggle Fullscreen:\tF1\nFast Quit:\tF12", "Controls help text.");
	setStandard("textInstructions", "Welcome to Crucible Island. You occupy a tower atop an island mountain range. Enemy ships circle the island firing missiles at you. You must shoot down these missiles using your cannon or by firing electromagnetic pulses. Firing cannon shells depletes your ammunition reservoir, and firing electromagnetic pulses requires a charging period and depletes both your ammunition and health reservoirs. A missile impact will deplete your health reservoir. Your ammunition and health reservoirs will constantly recharge. As the game goes on, additional enemy ships will join the others, causing the rate of fire to increase. Your health level, ammunition level, and electromagnetic pulse charging indicators are shown on gauges on your screen. Your radar and heads-up display show enemy ship and incoming missile positions. You gain one point for every enemy missile you destroy. When your health level reaches zero, the game is over. Good luck!", "Instruction text.");
	setStandard("textCredits", "Dedicated to Sergeant Sean Drenth #6894 of the Phoenix Police Department, EOW October 18, 2010.\n\nCreated by Joshua Bodine.\n\nMusic and sound effects by Michael Birch.\n\nThis software uses the Titillium Web font by Accademia di Belle Arti di Urbino and students of MA course of Visual design.\n\nThis software uses the Simple DirectMedia Layer library (http://www.libsdl.org/).\n\nPortions of this software are copyright (c) 2014 The FreeType Project (www.freetype.org). All rights reserved.", "Credits text.");

	// load standards from preferences (or save standard preferences if no file)
	if(platform->getPreferenceFloat("preferencesVersion") == 3.0f) {
		if(getString("displayWindowedResolutions").find(platform->getPreferenceString("displayWindowedResolution")) != std::string::npos)
			setStandard("displayWindowedResolution", platform->getPreferenceString("displayWindowedResolution").c_str());
		setStandard("displayFramerateLimiting", platform->getPreferenceFloat("displayFramerateLimiting"));
		setStandard("displayMultisamplingLevel", platform->getPreferenceFloat("displayMultisamplingLevel"));
		setStandard("displayStartFullscreen", (platform->getPreferenceFloat("displayStartFullscreen") == 1.0f ? true : false));
		setStandard("audioMusicVolume", platform->getPreferenceFloat("audioMusicVolume"));
		setStandard("audioEffectsVolume", platform->getPreferenceFloat("audioEffectsVolume"));
		setStandard("gameStartingLevel", platform->getPreferenceString("gameStartingLevel").c_str());
		setStandard("islandTerrainDetail", platform->getPreferenceFloat("islandTerrainDetail"));
		std::string highScoresString = platform->getPreferenceString("highScores");
		size_t i = highScoresString.find('\t');
		while(i != std::string::npos) {
			std::string highScoreName = highScoresString.substr(0, i);
			highScoresString = highScoresString.substr(i + 1);
			i = highScoresString.find('\n');
			std::string highScoreScore = highScoresString.substr(0, i);
			highScoresString = highScoresString.substr(i + 1);
			i = highScoresString.find('\t');
			highScores.push_back(std::make_pair((unsigned int) atoi(highScoreScore.c_str()), highScoreName));
		}
	} else {
		flushPreferences();
	}

	// log the build version
	std::stringstream buildInfo;
	buildInfo << "Game Version: " << versionString;
	this->log(LOG_INFO, buildInfo.str().c_str());
}
